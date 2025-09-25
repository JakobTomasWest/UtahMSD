package com.example.augmentedreality.ui

import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.lifecycle.compose.LocalLifecycleOwner
import android.Manifest
import android.os.Build
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.remember

import androidx.camera.compose.CameraXViewfinder
import androidx.camera.core.SurfaceRequest
import androidx.camera.viewfinder.compose.MutableCoordinateTransformer
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Text

import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.geometry.Size
import androidx.compose.ui.unit.dp
import androidx.compose.ui.geometry.Offset

import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.FilledTonalButton
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.aspectRatio

import androidx.compose.material3.SnackbarHost
import androidx.compose.material3.SnackbarHostState
import androidx.compose.ui.graphics.drawscope.drawIntoCanvas
import androidx.compose.ui.graphics.nativeCanvas
import androidx.compose.ui.graphics.setFrom
import androidx.compose.ui.platform.LocalContext
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.MaterialTheme



//The screen composable will connect to the VM and grab the Lifecycle Owner
@Composable
fun CameraRoute ( vm: CameraViewModel = viewModel() ) {

    // Turn the VM's stateflow into Compose state so recomp happens on updates
    val state by vm.state.collectAsState()
    // Lifecycle for this composable used later to bind CameraX
    val owner = LocalLifecycleOwner.current // pass to startPreview in vm when starting camera
    //
    val snackbarHostState = remember { SnackbarHostState() }

    // VM publishes each frrane; maps analyzer-buffer coords to sensor cords
    val analyzerToSensor by vm.analyzerToSensor.collectAsState()
    // List from data class with rect + label + score
    val rawDetections by vm.rawDetections.collectAsState()
    // Build the list of permissions once
    val requiredPerms = remember {
        buildList {
            add(Manifest.permission.CAMERA)
            if (Build.VERSION.SDK_INT >= 33){
                add(Manifest.permission.READ_MEDIA_IMAGES)
            } else {
                add(Manifest.permission.READ_EXTERNAL_STORAGE)
            }
        }.toTypedArray()
    }
    // Permission launcher - updates VM state when user responds
    val permissionLauncher = rememberLauncherForActivityResult(
        ActivityResultContracts.RequestMultiplePermissions()
    ) { resultMap ->
        // True only if all requested permissions were granted
        val granted = requiredPerms.all { perm -> resultMap[perm] == true}
        vm.handle(CameraIntent.OnPermissionsResult(granted))
    }

    // Send the permission request once the composable first appears
    LaunchedEffect(Unit) {
        permissionLauncher.launch(requiredPerms)
    }

    // When permissions are granted/becoem true or the lensFacing changes, re/bind the camera
    LaunchedEffect(state.permissionsGranted, state.lensFacing, state.detectorModel) {
        if (state.permissionsGranted) {
            vm.startPreview(owner) // tell the VM to bind preview, imagecapture, analysis
        } else {
            vm.stopPreview()
        }
    }
    LaunchedEffect(state.message) {
        val msg = state.message ?: return@LaunchedEffect
        snackbarHostState.showSnackbar(msg)
        vm.handle(CameraIntent.ClearMessage)
    }

    // UI -- show the camera preview when we have a SurfaceRequest from the VM
    Box(
        Modifier
            .fillMaxSize()
            .background(Color.Black)
    ) {

            val sr = state.surfaceRequest
            val ar = 4f / 3f

            // Center a box that matches the camera’s upright aspect ratio
            Box(
                Modifier
                    .fillMaxWidth()
                    .aspectRatio(ar)
                    .align(Alignment.Center)
            ) {
                // creates coordinate transformer for camerax viewfinder to update
                val transformer = remember { MutableCoordinateTransformer() }
                // If we have a SR from VM render the camera previeew
                if (sr != null) {
                    CameraXViewfinder( //How camerax get surface to draw into
                        modifier = Modifier.matchParentSize(),
                        surfaceRequest = sr,
                        coordinateTransformer = transformer // keep transfromer synced
                    )
                }

                val transformInfo = androidx.compose.runtime.produceState<SurfaceRequest.TransformationInfo?>(null, sr) {
                    if (sr != null) {
                        // CameraX calls this with latest crop rectangle, rot and sensor to bufffer matrix
                        sr.setTransformationInfoListener(Runnable::run) { value = it }
                    }
                    kotlinx.coroutines.awaitCancellation()
                }

                // Compose matrix that maps buffer -> UI (invert of viewfinder’s UI->buffer)
                val bufferToUi = androidx.compose.ui.graphics.Matrix().apply {
                    setFrom(transformer.transformMatrix) // UI -> buffer mapping
                    invert()                             // buffer -> UI
                }

                Canvas(Modifier.matchParentSize()) {
                    val info = transformInfo.value ?: return@Canvas

                    // Android matrices to multiply with
                    val sensorToPreviewBufferAndroid: android.graphics.Matrix = info.sensorToBufferTransform
                    val analyzerToSensorAndroid: android.graphics.Matrix = analyzerToSensor   // from VM publish each frame

                    // analysis -> previewBuffer = (sensor->previewBuffer) x (analysis->sensor)
                    val analysisToPreviewAndroid = android.graphics.Matrix().apply {
                        set(analyzerToSensorAndroid)
                        postConcat(sensorToPreviewBufferAndroid) // apply previous transform and then this one
                    }

                    // Convert to Compose matrix once
                    val analysisToPreview = androidx.compose.ui.graphics.Matrix().apply {
                        setFrom(analysisToPreviewAndroid)
                    }

                    rawDetections.forEach { det ->
                        // map analyzer rect -> preview buffer
                        val bufRect = analysisToPreview.map(det.rectPx)
                        // map preview buffer -> UI
                        val uiRect  = bufferToUi.map(bufRect)

                        // box
                        drawRect(
                            color = Color.Red,
                            topLeft = Offset(uiRect.left, uiRect.top),
                            size    = Size(uiRect.width, uiRect.height),
                            style   = Stroke(width = 4.dp.toPx())
                        )

                        drawIntoCanvas { canvas ->
                            val paint = android.graphics.Paint().apply {
                                color = android.graphics.Color.WHITE
                                textSize = 14.dp.toPx()
                                isAntiAlias = true
                            }
                            canvas.nativeCanvas.drawText(
                                "${det.label} ${(det.score * 100).toInt()}%",
                                uiRect.left,
                                (uiRect.top - 6.dp.toPx()).coerceAtLeast(12.dp.toPx()),  // keep visible
                                paint
                            )
                        }
                    }
                }
            }
        if (sr == null) {
        Text(
            "Waiting for camera",
            modifier = Modifier.align(Alignment.Center),
            color = Color.White
        )
        }

        SnackbarHost(
            hostState = snackbarHostState,
            modifier = Modifier.align(Alignment.TopCenter).padding(top = 48.dp)
        )
        val using = if (state.detectorModel == DetectorModel.MLKIT) "Using: ML Kit" else "Using: EfficientDet"
        Text(
            using,
            modifier = Modifier
                .align(Alignment.TopStart)
                .padding(8.dp)
                .background(Color(0x66000000), RoundedCornerShape(8.dp))
                .padding(horizontal = 8.dp, vertical = 4.dp),
            color = Color.White,
            style = MaterialTheme.typography.labelMedium
        )

        Row(
            Modifier
                .fillMaxWidth()
                .align(Alignment.BottomCenter)
                .padding(16.dp),
            horizontalArrangement = Arrangement.SpaceBetween
        )
            {
            FilledTonalButton(onClick = { vm.handle(CameraIntent.ToggleLens) }) {
                Text("Flip")
            }

            val isMlkit = state.detectorModel == DetectorModel.MLKIT
            FilledTonalButton(
                onClick = {
                    vm.handle(
                        CameraIntent.SetModel(
                            if (isMlkit) DetectorModel.EFFICIENTDET else DetectorModel.MLKIT
                        )
                    )
                }
            ) {
                Text(if (isMlkit) "Switch to EfficientDet" else "Switch to ML Kit")
            }

                Button(
                onClick = { vm.handle(CameraIntent.CapturePhoto) },
                enabled = state.permissionsGranted
            ) {
                Text("Save Photo")
            }
        }
    }

}

//LaunchedEffect(keys...) runs once when the composable first appears, and again only when any key value changes.
//
//That means: user taps Flip → VM updates lensFacing → Compose sees a new state.lensFacing → this LaunchedEffect runs → calls startPreview(owner) → CameraX re-binds to the other lens.


//android:configChanges="orientation|screenSize|smallestScreenSize|screenLayout|keyboardHidden|uiMode"
//    tools:replace="android:screenOrientation,android:configChanges"