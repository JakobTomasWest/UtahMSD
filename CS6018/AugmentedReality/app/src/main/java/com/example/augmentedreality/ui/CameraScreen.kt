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
import androidx.camera.viewfinder.compose.MutableCoordinateTransformer
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Text
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier

import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.FilledTonalButton
import androidx.compose.ui.unit.dp
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.material3.SnackbarHost
import androidx.compose.material3.SnackbarHostState
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.layout.ContentScale


//For now: the screen composable will connect to the VM and grab the Lifecycle Owner
@Composable
fun CameraRoute ( vm: CameraViewModel = viewModel() ) {

    // Turn the VM's stateflow into Compose state so recomp happens on updates
    val state by vm.state.collectAsState()
    // Lifecycle for this composable used later to bind CameraX
    val owner = LocalLifecycleOwner.current // pass to startPreview in vm

    val snackbarHostState = remember { SnackbarHostState() }

    // Build the list of permissions once
    val requiredPerms = remember {
        buildList {
            add(Manifest.permission.CAMERA) // camera + right media read permission by SDK
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
    LaunchedEffect(state.permissionsGranted, state.lensFacing) {
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
        // If we have a SurfaceRequest, draw the preview + overlay, otherwise show a message
        state.surfaceRequest?.let { sr ->
            val fw = (state.frameWidth.takeIf { it > 0 } ?: 16)
            val fh = (state.frameHeight.takeIf { it > 0 } ?: 9)

            // Center a box that matches the camera’s upright aspect ratio
            Box(
                Modifier
                    .fillMaxWidth()
                    .aspectRatio(fw.toFloat() / fh.toFloat())
                    .align(Alignment.Center)
            ) {
                val transformer = remember { MutableCoordinateTransformer() }

                CameraXViewfinder(
                    modifier = Modifier.matchParentSize(),
                    surfaceRequest = sr,
                    coordinateTransformer = transformer
                )

                // Overlay uses the exact same sized box as the viewfinder
                Canvas(Modifier.matchParentSize()) {
                    val poi = state.poi ?: return@Canvas
                    val imgW = fw.toFloat()
                    val imgH = fh.toFloat()

                    val scale = kotlin.math.min(size.width / imgW, size.height / imgH)
                    val cx = poi.x * imgW * scale
                    val cy = poi.y * imgH * scale
                    drawCircle(
                        color = Color.Red,
                        radius = 8.dp.toPx(),
                        center = Offset(cx, cy)
                    )
                }
            }
        } ?: Text(
            "Waiting for camera",
            modifier = Modifier.align(Alignment.Center),
            color = Color.White
        )
        SnackbarHost(
            hostState = snackbarHostState,
            modifier = Modifier.align(Alignment.TopCenter).padding(top = 48.dp)
        )
        Row(
            Modifier
                .fillMaxWidth()
                .align(Alignment.BottomCenter)
                .padding(16.dp),
            horizontalArrangement = Arrangement.SpaceBetween
        ) {
            FilledTonalButton(onClick = { vm.handle(CameraIntent.ToggleLens) }) {
                Text("Flip")
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