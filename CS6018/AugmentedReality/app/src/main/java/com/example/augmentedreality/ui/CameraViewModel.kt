package com.example.augmentedreality.ui

import android.app.Application
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
import androidx.camera.lifecycle.awaitInstance
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.launch
import androidx.compose.ui.geometry.Offset
import com.example.augmentedreality.data.MediaStoreSaver
import kotlin.math.max
import androidx.camera.core.resolutionselector.AspectRatioStrategy
import androidx.camera.core.resolutionselector.ResolutionSelector


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
                    setSurfaceProvider { srequest ->
                        // Use the PREVIEW’s resolution for overlay math (this is what the viewfinder draws)
                        val res = srequest.resolution
                        _state.value = _state.value.copy(
                            surfaceRequest = srequest,
                            frameWidth = res.width,
                            frameHeight = res.height
                        )
                    }
                }
            buildAnalysis()
            // Fourth, Build the image capture
            imageCapture = ImageCapture.Builder()
                .setCaptureMode(ImageCapture.CAPTURE_MODE_MAXIMIZE_QUALITY)
                .build()

            // Fifth, link each case to the lifecycle owner using the current camera toggle
            val analysis = imageAnalysis ?: return@launch
            cameraProvider.bindToLifecycle(
                owner,
                cameraSelector(),
                preview,
                imageCapture,
                analysis
            )
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


    // Read the center pixels RGB and update brightness
    private fun setCenterBrightnessAnalyzer(analysis: ImageAnalysis){
        analysis.setAnalyzer(analysisExecutor){ proxy -> //runs analysis on background thread
            try {
                val plane = proxy.planes[0]
                val buffer = plane.buffer
                val rowStride = plane.rowStride
                val pixelStride = plane.pixelStride
                val width = proxy.width
                val height = proxy.height

                if(width > 0 && height > 0){
                    val centerX = width / 2
                    val centerY = height / 2
                    val offset = centerY * rowStride + centerX * pixelStride
                    if(offset + 2 < buffer.limit()){
                        val r = buffer.get(offset).toInt() and 0xFF
                        val g = buffer.get(offset + 1).toInt() and 0xFF
                        val b = buffer.get(offset + 2).toInt() and 0xFF
                        val average = (r + g + b) / (3 * 255f)

                        val xNorm = if (width > 1) centerX.toFloat() / (width - 1) else 0.5f
                        val yNorm = if (height > 1) centerY.toFloat() / (height - 1) else 0.5f

                        // mirror X if front camera is active so that the overlay matches preview
                        val xForUi = if (_state.value.lensFacing == CameraSelector.LENS_FACING_FRONT) 1f - xNorm
                        else xNorm
                        // update state with brightness value
                        _state.value = _state.value.copy(
                            brightness = average,
                            poi = Offset(xForUi, yNorm)
                        )
                    }
                }

            } finally {
                proxy.close() // closes frame
            }
        }
    }

    private fun toUprightNormalized(
        x: Int, y: Int, w: Int, h: Int, rotationDegrees: Int
    ): Pair<Float, Float> {
        val w1 = max(1, w - 1)
        val h1 = max(1, h - 1)
        return when (rotationDegrees % 360) {
            0 -> x / w1.toFloat() to y / h1.toFloat()
            90 -> y / h1.toFloat() to (w1 - x) / w1.toFloat()           // (x',y') = (y, w-1-x)
            180 -> (w1 - x) / w1.toFloat() to (h1 - y) / h1.toFloat()    // (w-1-x, h-1-y)
            270 -> (h1 - y) / h1.toFloat() to x / w1.toFloat()           // (h-1-y, x)
            else -> x / w1.toFloat() to y / h1.toFloat()
        }
    }

    // Scan a grid, and pick the brightest pixel, upgrade brightness and poi
    private fun setBrightnessPointAnalyzer(analysis: ImageAnalysis, grid: Int =96){
        analysis.setAnalyzer(analysisExecutor){ proxy -> //runs analysis on background thread
            try {
                val plane = proxy.planes[0]
                val buffer = plane.buffer
                val rowStride = plane.rowStride
                val pixelStride = plane.pixelStride
                val width = proxy.width
                val height = proxy.height
                val rotation = proxy.imageInfo.rotationDegrees
                var bestSum = -1
                var bestX =0
                var bestY = 0
                var total = 0f
                var totalCount = 0

                val stepX = max(1, width / grid)
                val stepY = max(1, height / grid)

                for(y in 0 until height step stepY){
                    val base = y * rowStride
                    for (x in 0 until width step stepX){
                        val offset = base + x * pixelStride
                        if(offset + 2 < buffer.limit()){
                            val r = buffer.get(offset).toInt() and 0xFF
                            val g = buffer.get(offset + 1).toInt() and 0xFF
                            val b = buffer.get(offset + 2).toInt() and 0xFF
                            val sum = r + g + b
                            total += sum
                            totalCount += 3
                            if(sum > bestSum){
                                bestSum = sum
                                bestX = x
                                bestY = y
                            }
                        }
                    }
                }

                var (xn, yn) = toUprightNormalized(bestX, bestY, width, height, rotation)
                if (_state.value.lensFacing == CameraSelector.LENS_FACING_FRONT) {
                    xn = 1f - xn
                }
                val average = if(totalCount == 0) 0f else total / totalCount
                // update state
                _state.value = _state.value.copy(
                    brightness = average ,
                    poi = Offset(xn, yn)
                )
            } finally {
            proxy.close() // closes frame}
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






