package com.example.augmentedreality.ui

import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.compose.ui.platform.LocalLifecycleOwner
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
import androidx.compose.material3.Text
import androidx.compose.ui.unit.dp
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.Canvas



//For now: the screen composable will connect to the VM and grab the Lifecycle Owner
@Composable
fun CameraRoute ( vm: CameraViewModel = viewModel() ) {

    // Turn the VM's stateflow into Compose state so recomp happens on updates
    val state by vm.state.collectAsState()
    // Lifecycle for this composable used later to bind CameraX
    val owner = androidx.lifecycle.compose.LocalLifecycleOwner.current // pass to startPreview in vm

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

    // UI -- show the camera preview when we have a SurfaceRequest from the VM
    Box(Modifier.fillMaxSize()) {

        //If preview produced a SR, render it with CameraXViewfinder
        state.surfaceRequest?.let { sr ->
            val transformer = remember { MutableCoordinateTransformer() }
            CameraXViewfinder(
                modifier = Modifier.fillMaxSize(),
                surfaceRequest = sr,
                coordinateTransformer = transformer
            )
        } ?: run {
            Text("Waiting for camera", modifier = Modifier.align(Alignment.Center))
        }
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