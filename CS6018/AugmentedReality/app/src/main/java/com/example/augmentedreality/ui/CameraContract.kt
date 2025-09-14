package com.example.augmentedreality.ui

import androidx.camera.core.SurfaceRequest


data class CameraUiState(
    val permissionsGranted: Boolean = false,
    val lensFacing: Int = androidx.camera.core.CameraSelector.LENS_FACING_BACK,
    val surfaceRequest: SurfaceRequest? = null,
    val brightness: Float = 0f,
//    val sampleSpot:
    val message: String? = null
    )

sealed interface CameraIntent {
    data object ToggleLens : CameraIntent
    data object CapturePhoto : CameraIntent
    data class OnPermissionsResult(val granted: Boolean) : CameraIntent

}