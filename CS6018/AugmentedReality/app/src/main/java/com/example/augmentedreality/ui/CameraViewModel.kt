package com.example.augmentedreality.ui

import android.app.Application
import android.content.Context
import android.graphics.Bitmap
import android.util.Rational
import android.view.Display
import android.view.Surface
import android.hardware.display.DisplayManager
import android.graphics.Color as AColor
import androidx.camera.core.AspectRatio
import androidx.lifecycle.AndroidViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import androidx.camera.core.ImageCapture
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.Preview
import java.util.concurrent.Executors
import androidx.camera.core.CameraSelector
import androidx.camera.core.SurfaceRequest
import androidx.camera.core.UseCaseGroup
import androidx.camera.core.ViewPort
import androidx.camera.lifecycle.awaitInstance
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.launch
import androidx.compose.ui.geometry.Offset
import com.example.augmentedreality.data.MediaStoreSaver
import kotlin.math.max
import androidx.camera.core.resolutionselector.AspectRatioStrategy
import androidx.camera.core.resolutionselector.ResolutionSelector
import androidx.core.content.ContextCompat
import androidx.core.content.getSystemService
import kotlin.math.min


// ViewModel holds UI state and business logic for the camera screen.
class CameraViewModel(app: Application) : AndroidViewModel(app) {

    // Backing, mutable state inside the VM
    private val _state = MutableStateFlow(CameraUiState())

    // RO view for Ui to collectAsState without being able to modify
    val state = _state.asStateFlow()

    // Keep the use cases as fields in VM to survive recompositions and UI
    // photo capture pipeline
    private var imageCapture: ImageCapture? = null
    private var imageAnalysis: ImageAnalysis? = null
    // per frane analysis for brightest spot
    private var preview: Preview? = null // live camera preview to feed to viewfinder

    // Single background thread for image analysis
    private val analysisExecutor = Executors.newSingleThreadExecutor()


    private val resolutionSelector: ResolutionSelector =
        ResolutionSelector.Builder()
            .setAspectRatioStrategy(
                AspectRatioStrategy(
                    AspectRatio.RATIO_16_9,
                    AspectRatioStrategy.FALLBACK_RULE_AUTO
                )
            )
            .build()

    /* Build a cameraSelector for matching our current choice in state */
    private fun cameraSelector(): CameraSelector =
        CameraSelector.Builder()
            .requireLensFacing(_state.value.lensFacing)
            .build()

    private fun displayRotation(): Int {
        val dm = getApplication<Application>().getSystemService(Context.DISPLAY_SERVICE) as DisplayManager
        val display = dm.getDisplay(Display.DEFAULT_DISPLAY)
        return display?.rotation ?: Surface.ROTATION_0
    }

    // State the camera preview and expose the SurfaceRequest for CameraXViewfinder.
    fun startPreview(owner: androidx.lifecycle.LifecycleOwner) {
        // Launch work on the VM's coroutine scope
        viewModelScope.launch {
            // First, get the cameraX provider to bind use cases to lifecycle
            val cameraProvider = ProcessCameraProvider.awaitInstance(getApplication())

            // Second, Unbind any use cases before rebinding - for camera flips
            cameraProvider.unbindAll()
            // Third, setup the viewfinder use case to display camera preview
            // Capture each SurfaceRequest  and put it into our state. compose hands to CameraXViewfinder
            preview = Preview.Builder()
                .setResolutionSelector(resolutionSelector)
                .build()
                .apply {
                    setSurfaceProvider { srequest: SurfaceRequest ->

                        // Listen for how the preview is transformed on screen
                        srequest.setTransformationInfoListener(
                            ContextCompat.getMainExecutor(getApplication())
                        ) { info: SurfaceRequest.TransformationInfo ->
                            val res = srequest.resolution
                            val rot = info.rotationDegrees      // {0,  90, 180, 270}

                            // Use the *upright* preview size (swap when 90/270)
                            val (wUpright, hUpright) =
                                if (rot % 180 == 90) res.height to res.width else res.width to res.height

                            _state.value = _state.value.copy(
                                surfaceRequest = srequest,
                                frameWidth = wUpright,
                                frameHeight = hUpright
                            )
                        }
                    }
                }
            buildAnalysis()
            // Fourth, Build the image capture
            imageCapture = ImageCapture.Builder()
//                .setCaptureMode(ImageCapture.CAPTURE_MODE_MAXIMIZE_QUALITY)
                .build()

            val analysis = createImageAnalysis().also{
                setBrightnessPointAnalyzer(it)
            }

            val vp = ViewPort.Builder(
                Rational(16, 9),
                displayRotation()
            ).setScaleType(ViewPort.FIT).build()

            val group = UseCaseGroup.Builder()
                .setViewPort(vp)
                .addUseCase(preview!!)
                .addUseCase(imageCapture!!)
                .addUseCase(analysis)
                .build()
            // Fifth, link each case to the lifecycle owner using the current camera toggle
//            val analysis = imageAnalysis ?: return@launch
            cameraProvider.bindToLifecycle(
                owner,
                cameraSelector(),
                group)
        }
    }

    // Stop the camera preview and clear the SurfaceRequest shown by the UI
    fun stopPreview(){
        // Use the VM scope so this cancels automatically if the VM is cleared
        viewModelScope.launch {
            val cameraProvider = ProcessCameraProvider.awaitInstance(getApplication())
            cameraProvider.unbindAll()
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
                // The UI can now decide to startPreview(owner) or show preview request
            }
        }


    }
    // Build an ImageAnalysis use case which processes frames
    private fun createImageAnalysis(): ImageAnalysis =
        ImageAnalysis.Builder()
            .setResolutionSelector(resolutionSelector)
    .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
            .setOutputImageFormat(ImageAnalysis.OUTPUT_IMAGE_FORMAT_RGBA_8888)
            .build()


    private fun toUprightNormalized(
        x: Float, y: Float, w: Int, h: Int, rotationDegrees: Int
    ): Pair<Float, Float> {
        val w1 = w.toFloat()
        val h1 = h.toFloat()
        return when ((rotationDegrees % 360 + 360) % 360) {
            // Use pixel CENTERS: (x+0.5)/w, (y+0.5)/h
            0   -> (x + 0.5f) / w1 to (y + 0.5f) / h1
            90  -> (h1 - (y + 0.5f)) / h1 to (x + 0.5f) / w1      // Clockwise 90°
            180 -> (w1 - (x + 0.5f)) / w1 to (h1 - (y + 0.5f)) / h1
            270 -> (y + 0.5f) / h1 to (w1 - (x + 0.5f)) / w1      // Clockwise 270°
            else-> (x + 0.5f) / w1 to (y + 0.5f) / h1
        }
    }

    // Scan a grid, and pick the brightest pixel, upgrade brightness and poi
    private fun setBrightnessPointAnalyzer(analysis: ImageAnalysis, grid: Int =64){
        analysis.setAnalyzer(analysisExecutor){ proxy -> //runs analysis on background thread
            try {
//
                val bmp: Bitmap = proxy.toBitmap()
                val width = bmp.width
                val height = bmp.height
                val rotation = proxy.imageInfo.rotationDegrees
                if (width <= 0 || height <= 0) return@setAnalyzer
                var bestLuma = -1f
                var bestX =0
                var bestY = 0


                val stepX = max(1, width / grid)
                val stepY = max(1, height / grid)

//
                for (y in 0 until height step stepY){
                    for (x in 0 until width step stepX) {
                        val pixel = bmp.getPixel(x, y)
                        val r = AColor.red(pixel)
                        val g = AColor.green(pixel)
                        val b = AColor.blue(pixel)
                        val luma = 0.299f * r + 0.587f * g + 0.114f * b
                        if (luma > bestLuma) {
                            bestLuma = luma
                            bestX = x
                            bestY = y
                        }
                    }
                }

                val win = 3
                var wsum = 0f
                var xsum = 0f
                var ysum = 0f
                val x0 = max(0, bestX - win)
                val x1 = min(width - 1, bestX + win)
                val y0 = max(0, bestY - win)
                val y1 = min(height - 1, bestY + win)

                for (yy in y0..y1){
                    for (xx in x0..x1){
                        val pixel = bmp.getPixel(xx, yy)
                        val r = AColor.red(pixel)
                        val g = AColor.green(pixel)
                        val b = AColor.blue(pixel)
                        val luma = 0.299f * r + 0.587f * g + 0.114f * b
                        if (luma > 0f){
                            wsum += luma
                            xsum += xx * luma
                            ysum += yy * luma
                        }
                    }
                }
                var bx = bestX.toFloat()
                var by = bestY.toFloat()
                if( wsum > 0f){
                    bx = (xsum / wsum)
                    by = (ysum / wsum)
                }


                var (xn, yn) = toUprightNormalized(bx, by, width, height, rotation)
                if (_state.value.lensFacing == CameraSelector.LENS_FACING_FRONT) {
                    xn = 1f - xn
                }
                xn = xn.coerceIn(0f, 1f)
                yn = yn.coerceIn(0f, 1f)
                val brightness = (bestLuma.coerceAtLeast(0f))/ 255f
                // update state
                _state.value = _state.value.copy(
                    brightness = brightness ,
                    poi = Offset(xn, yn)
                )
            } finally {
            proxy.close()
            }
        }
    }



    // Create and store an ImageAnalysis with our ( current) analyzer attached
    private fun buildAnalysis(){
        imageAnalysis?.clearAnalyzer() // detach old analyzer for rebuild

        // Make a new analysis with RGBA settings
        val analysis = createImageAnalysis()

        setBrightnessPointAnalyzer(analysis)
        // keep a reference so startPreview() can bind it
        imageAnalysis = analysis
    }

    // Take a photo with ImageCapture and save it to ARPreview by MediaStoreSaver
    private fun takePhoto(){
        val imageCapture = imageCapture ?: return //no image capture configured
        viewModelScope.launch {
            try {
                val savedUri = MediaStoreSaver.savePhoto(getApplication(), imageCapture)
                val msg = "Photo saved: $savedUri"
                _state.value = _state.value.copy(message = msg)
            } catch (e: Exception) {
                _state.value = _state.value.copy(message = "Save failed: ${e.message}")
            }
        }
    }
}






