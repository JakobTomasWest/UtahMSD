package com.example.augmentedreality.ui

import android.app.Application
import android.content.Context
import android.graphics.Bitmap
import android.net.Uri
import android.util.Rational
import android.view.Surface
import android.hardware.display.DisplayManager
import android.os.Build
import android.util.Log
import androidx.camera.core.AspectRatio
import androidx.lifecycle.AndroidViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import androidx.camera.core.ImageCapture
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.Preview
import java.util.concurrent.Executors
import androidx.camera.core.CameraSelector
import androidx.camera.core.UseCaseGroup
import androidx.camera.core.ViewPort
import androidx.camera.lifecycle.awaitInstance
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.launch
import com.example.augmentedreality.data.MediaStoreSaver
import com.example.augmentedreality.net.ApiClient
import com.example.augmentedreality.net.TokenStore
import androidx.camera.core.resolutionselector.ResolutionSelector
import androidx.camera.core.resolutionselector.AspectRatioStrategy
import androidx.lifecycle.LifecycleOwner

import kotlin.getValue
// ML Kit
import com.google.mlkit.vision.objects.ObjectDetection
import com.google.mlkit.vision.objects.defaults.ObjectDetectorOptions as MlKitODOptions
// TFLite Task Vision (EfficientDet)
import org.tensorflow.lite.task.vision.detector.ObjectDetector
import org.tensorflow.lite.task.vision.detector.ObjectDetector.ObjectDetectorOptions as TfLiteODOptions
import android.view.WindowManager
import io.ktor.http.ContentType

// RawDetection
data class RawDetection(
    val rectPx: androidx.compose.ui.geometry.Rect,
    val label: String,
    val score: Float
)

private val COCO80 = listOf(
    "person","bicycle","car","motorcycle","airplane","bus","train","truck","boat","traffic light",
    "fire hydrant","stop sign","parking meter","bench","bird","cat","dog","horse","sheep","cow",
    "elephant","bear","zebra","giraffe","backpack","umbrella","handbag","tie","suitcase",
    "frisbee","skis","snowboard","sports ball","kite","baseball bat","baseball glove","skateboard",
    "surfboard","tennis racket","bottle","wine glass","cup","fork","knife","spoon","bowl",
    "banana","apple","sandwich","orange","broccoli","carrot","hot dog","pizza","donut","cake",
    "chair","couch","potted plant","bed","dining table","toilet","tv","laptop","mouse","remote",
    "keyboard","cell phone","microwave","oven","toaster","sink","refrigerator","book","clock",
    "vase","scissors","teddy bear","hair drier","toothbrush"
)

// ViewModel holds UI state and business logic for the camera screen.
class CameraViewModel(app: Application) : AndroidViewModel(app) {

    // Server client & auth storage
    private val api = ApiClient()
    private val tokenStore = TokenStore(getApplication())
    @Volatile
    private var token: String? = null

    private var username: String? = null
    // Ensure that we have JTW in memory  and only signup once
    private suspend fun ensureToken(username: String = "user", password: String = "pass"): String {
        token?.let {return it}
        // try to create user
        runCatching { api.signUp(username, password)}
        this.username = username
        val t = api.login(username, password)
        token = t
        runCatching { tokenStore.save(t)}
        return t
    }
    suspend fun listPhotos(): List<String> {
        val t = ensureToken()
        return api.listPhotos(t)
    }

    fun photoUrl(name: String): String = api.photoUrl(name)

    fun currentTokenOrNull(): String? = token
    fun currentUsernameOrNull(): String? = username


    sealed interface AuthState {
        data object SignedOut : AuthState
        data class SignedIn(val username: String) : AuthState
    }

    private val _authState = MutableStateFlow<AuthState>(AuthState.SignedOut)
    suspend fun login(username: String, password: String){
        this.username = username
        runCatching {api.signUp(username,password)}
        val t = api.login(username,password)
        token = t
        tokenStore.save(t)
        _authState.value = AuthState.SignedIn(username)
    }
    fun logout(){
        token = null
        username = null
        _authState.value = AuthState.SignedOut
        viewModelScope.launch {
            runCatching { tokenStore.clear() }
                .onFailure { Log.w("CameraVM", "clear() failed", it) }

        }
    }


    // Backing, mutable state inside the VM
    private val _state = MutableStateFlow(CameraUiState())
    // RO view for Ui to collectAsState without being able to modify
    val state = _state.asStateFlow()

    // Matrix that maps analyzer buffer → sensor (really: inverse of sensorToBuffer for analyzer)
    private val _analyzerToSensor = MutableStateFlow(android.graphics.Matrix())  // analysis-buffer → sensor
    val analyzerToSensor = _analyzerToSensor.asStateFlow()
    private val _rawDetections = MutableStateFlow<List<RawDetection>>(emptyList())
    val rawDetections = _rawDetections.asStateFlow()

    private var cameraProvider: ProcessCameraProvider? = null
    private var preview: Preview? = null
    private var imageCapture: ImageCapture? = null
    private var imageAnalysis: ImageAnalysis? = null
    private var currentOwner: LifecycleOwner? = null
    private var lastRotation: Int = Surface.ROTATION_0
    // Single background thread for image analysis
    private val analysisExecutor = Executors.newSingleThreadExecutor()
    private val mlkitObjectDetector by lazy {
        val options = MlKitODOptions.Builder()
            .setDetectorMode(MlKitODOptions.STREAM_MODE) //live throughput
            .enableMultipleObjects()
            .enableClassification()
            .build()
        ObjectDetection.getClient(options)
    }


    private val efficientDet by lazy {
        val opts = TfLiteODOptions.builder()
            .setScoreThreshold(0.25f)
            .setMaxResults(10)
            .build()
        ObjectDetector.createFromFileAndOptions(
            getApplication(),
            "efficientdet_lite4.tflite",
            opts
        )
    }

    private val orientationListener =
        object : android.view.OrientationEventListener(getApplication()) {
            override fun onOrientationChanged(orientation: Int) {
                val rot = displayRotation()
                if (rot == lastRotation) return
                lastRotation = rot

                // keep frames correct immediately
                preview?.targetRotation = rot
                imageCapture?.targetRotation = rot
                imageAnalysis?.targetRotation = rot

                }
        }


    fun startPreview(owner: LifecycleOwner) {
        try {
            currentOwner = owner

            // Get or create the ProcessCameraProvider
            val provider = cameraProvider ?: run {
                val future = ProcessCameraProvider.getInstance(getApplication())
                val p = future.get()
                cameraProvider = p
                p
            }

            val selector = ResolutionSelector.Builder()
                .setAspectRatioStrategy(
                    AspectRatioStrategy(
                        AspectRatio.RATIO_4_3,
                        AspectRatioStrategy.FALLBACK_RULE_AUTO
                    )
                )
                .build()


            val p = Preview.Builder()
                .setResolutionSelector(selector)
                .build()
                .also { preview = it }


            val ic = ImageCapture.Builder()
                .setResolutionSelector(selector)
                .setCaptureMode(ImageCapture.CAPTURE_MODE_MINIMIZE_LATENCY)
                .build()
                .also { imageCapture = it }

            val ia = ImageAnalysis.Builder()
                .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST) // drop old frames
                .setOutputImageFormat(ImageAnalysis.OUTPUT_IMAGE_FORMAT_YUV_420_888) // better for ML Kit
                .setOutputImageRotationEnabled(false)
                .build()
                .also { imageAnalysis = it }

            // choose analyzer
            when (_state.value.detectorModel) {
                DetectorModel.MLKIT -> setMLKObjectAnalyzer(ia)
                DetectorModel.EFFICIENTDET -> setEfficientDetAnalyzer(ia)
            }
            // initialize rotation and apply to all use-cases
            val rotation = displayRotation()
            lastRotation = rotation
            p.targetRotation = rotation
            ic.targetRotation = rotation
            ia.targetRotation = rotation

            // Build the initial ViewPort and group for the CURRENT rotation so all use cases see the same region of the sesor
            val vp = ViewPort.Builder(Rational(4, 3), rotation)
                .setScaleType(ViewPort.FIT)
                .build()

            val group = UseCaseGroup.Builder()
                .setViewPort(vp)
                .addUseCase(p)
                .addUseCase(ic)
                .addUseCase(ia)
                .build()

            provider.unbindAll()
            // Bind all to screen
            provider.bindToLifecycle(owner, cameraSelector(), group)


            val previous = _state.value.surfaceRequest
            previous?.willNotProvideSurface()
            p.setSurfaceProvider { request ->
                // Push the request to UI; CameraXViewfinder will consume it
                _state.value = _state.value.copy(surfaceRequest = request)
            }
            // finally, enable the listener so future rotations rebind correctly
            orientationListener.enable()

        } catch (t: Throwable) {
            _state.value = _state.value.copy(message = "startPreview failed: ${t.message}")
        }
    }


    // Stop the camera preview and clear the SurfaceRequest shown by the UI
    fun stopPreview(){
        // Use the VM scope so this cancels automatically if the VM is cleared
        viewModelScope.launch {
            val cameraProvider = ProcessCameraProvider.awaitInstance(getApplication())
            cameraProvider.unbindAll()
            orientationListener.disable()
            // Clear the SurfaceRequest from state to remove the CameraXViewfinder - tell Ui there's no surface to render
            _state.value = _state.value.copy(surfaceRequest = null)
        }
    }

    // Add intent handler to hellp UI talk to VM, handle events/process intents by updating state here
    fun handle(intent: CameraIntent) {
        when (intent) {

            //When the user taps flip button, toggle the lensFacing state
            CameraIntent.ToggleLens -> {
                val current = _state.value.lensFacing
                val next =
                    if (current == CameraSelector.LENS_FACING_BACK)
                        CameraSelector.LENS_FACING_FRONT
                    else
                        CameraSelector.LENS_FACING_BACK

                // Update the lensFacing in immutable state to trigger recomposition
                _state.value = _state.value.copy(lensFacing = next)
            }

            // When the user taps the capture button, take and save a photo
            CameraIntent.CapturePhoto -> takePhoto()

            CameraIntent.ClearMessage -> {
                _state.value = _state.value.copy(message = null)
            }
            // Permissions result came back from teh Activity Result API
            is CameraIntent.OnPermissionsResult -> {
                _state.value = _state.value.copy(permissionsGranted = intent.granted)

            }

            is CameraIntent.SetModel -> {
                if (_state.value.detectorModel != intent.model) {
                    _state.value = _state.value.copy(
                        detectorModel = intent.model,
                        detections = emptyList(),
                        poi = null
                    )
                    imageAnalysis?.clearAnalyzer()
                    when (intent.model) {
                        DetectorModel.MLKIT -> imageAnalysis?.let { setMLKObjectAnalyzer(it) }
                        DetectorModel.EFFICIENTDET -> imageAnalysis?.let { setEfficientDetAnalyzer(it) }
                    }
                }
                _state.value = _state.value.copy(poi = null)
            }

        }


    }
    suspend fun uploadSavedPhoto(savedUri: Uri): String {
        val token = ensureToken()
        val app = getApplication<Application>()
        val cr = app.contentResolver

        // Infer MIME & content type
        val mime = cr.getType(savedUri) ?: "image/jpeg"
        val contentType = when {
            mime.contains("png", true) -> ContentType.Image.PNG
            mime.contains("webp", true) -> ContentType.parse("image/webp")
            mime.contains("heic", true) || mime.contains("heif", true) -> ContentType.parse("image/heic")
            else -> ContentType.Image.JPEG
        }
        val ext = when {
            contentType == ContentType.Image.PNG -> "png"
            contentType.toString().contains("webp", true) -> "webp"
            contentType.toString().contains("heic", true) -> "heic"
            else -> "jpg"
        }
        val remoteName = "photo_${System.currentTimeMillis()}.$ext"

        // Read the image bytes
        val bytes = cr.openInputStream(savedUri)!!.use { it.readBytes() }

        // Upload via your ApiClient
        val reply = api.uploadImageBytes(
            token = token,
            bytes = bytes,
            remoteName = remoteName,
            contentType = contentType
        )
        val uploaded = reply.trim().trim('"').ifBlank {remoteName}
        return uploaded
    }
    // Take a photo with ImageCapture and save it to ARPreview by MediaStoreSaver
    private fun takePhoto(){
        val imageCapture = imageCapture ?: return //no image capture configured
        viewModelScope.launch {
            try {
                val savedUri: Uri? = MediaStoreSaver.savePhoto(getApplication(),imageCapture)
                if (savedUri == null){
                    _state.value = _state.value.copy(message = "Save failed: URI")
                    return@launch
                }
//                val msg = "Photo saved: $savedUri"
//                _state.value = _state.value.copy(message = msg)

//                val t = ensureToken()
//                val remoteName = "photo_${System.currentTimeMillis()}.png"
//                val result = api.uploadImageUri(
//                    token = t,
//                    context = getApplication(),
//                    uri = savedUri,
//                    remoteName = remoteName
//                )
                val uploadedName = uploadSavedPhoto(savedUri)
                val url = api.photoUrl(uploadedName)
                _state.value = _state.value.copy(
//                    message = "Uploaded $remoteName: $result"
                    message = "Uploaded $uploadedName\n$url"
                )
            } catch (e: Exception) {
                _state.value = _state.value.copy(message = "Save/Upload failed: ${e.message}")
            }
        }
    }



    private fun setMLKObjectAnalyzer(analysis: ImageAnalysis) {
        analysis.setAnalyzer(analysisExecutor) { imageProxy ->
            try {

                val bmp = imageProxy.toBitmap()

                // Feed ML Kit the upright bitmap
                val input = com.google.mlkit.vision.common.InputImage.fromBitmap(bmp, 0)

                mlkitObjectDetector.process(input)
                    .addOnSuccessListener { objects ->
                        val raw = buildList {
                            for (obj in objects) {
                                val bb = obj.boundingBox  // coords in analyzer-buffer space
                                val best = obj.labels.maxByOrNull { it.confidence }
                                val label = best?.text?.takeIf { it.isNotBlank() } ?: "object"
                                val score = best?.confidence ?: 0f

                                add(
                                    RawDetection(
                                        rectPx = androidx.compose.ui.geometry.Rect(
                                            bb.left.toFloat(), bb.top.toFloat(),
                                            bb.right.toFloat(), bb.bottom.toFloat()
                                        ),
                                        label = label,
                                        score = score
                                    )
                                )
                            }
                        }
                        _rawDetections.value = raw


                        val bufferToSensor = android.graphics.Matrix().apply {
                            imageProxy.imageInfo.sensorToBufferTransformMatrix.invert(this)
                        }
                        _analyzerToSensor.value = bufferToSensor
                    }
                    .addOnFailureListener { e ->
                        _state.value = _state.value.copy(message = "ML Kit error: ${e.message}")
                    }
                    .addOnCompleteListener { imageProxy.close() }
            } catch (t: Throwable) {
                _state.value = _state.value.copy(message = "Analyzer crash: ${t.message}")
                imageProxy.close()
            }
        }
    }



    private fun setEfficientDetAnalyzer(analysis: ImageAnalysis) {
        analysis.setAnalyzer(analysisExecutor) { imageProxy ->
            try {
                // Run EfficientDet on the current frame
                val bmp = imageProxy.toBitmap()
                val tensorImage = org.tensorflow.lite.support.image.TensorImage.fromBitmap(bmp)
                val detections = efficientDet.detect(tensorImage) // sync call

                val raw = buildList {
                    for (det in detections) {
                        val best = det.categories.maxByOrNull { it.score } ?: continue
                        val score = best.score
                        if (score < 0.25f) continue


                        val idx = best.index
                        val label = when {
                            idx in COCO80.indices -> COCO80[idx]
                            !best.displayName.isNullOrBlank() -> best.displayName
                            !best.label.isNullOrBlank() -> best.label
                            else -> "object"
                        }

                        val r = det.boundingBox
                        add(
                            RawDetection(
                                rectPx = androidx.compose.ui.geometry.Rect(
                                    r.left.toFloat(), r.top.toFloat(),
                                    r.right.toFloat(), r.bottom.toFloat()
                                ),
                                label = label,
                                score = score
                            )
                        )
                    }
                }
                _rawDetections.value = raw

                // Keep your overlay in sync: analyzerBuffer -> sensor
                val bufferToSensor = android.graphics.Matrix().apply {
                    imageProxy.imageInfo.sensorToBufferTransformMatrix.invert(this)
                }
                _analyzerToSensor.value = bufferToSensor
            } catch (t: Throwable) {
                _state.value = _state.value.copy(message = "EfficientDet error: ${t.message}")
            } finally {
                imageProxy.close()
            }
        }
    }

    /* Build a cameraSelector for matching our current choice in state */
    private fun cameraSelector(): CameraSelector =
        CameraSelector.Builder()
            .requireLensFacing(_state.value.lensFacing)
            .build()



    private fun displayRotation(): Int {
        val app = getApplication<Application>()
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            val dm = app.getSystemService(DisplayManager::class.java)
            val d = dm?.displays?.firstOrNull()
            d?.rotation ?: Surface.ROTATION_0
        } else {
            @Suppress("DEPRECATION")
            (app.getSystemService(Context.WINDOW_SERVICE) as WindowManager)
                .defaultDisplay.rotation
        }
    }

}






