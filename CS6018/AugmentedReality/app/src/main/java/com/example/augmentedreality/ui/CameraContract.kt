package com.example.augmentedreality.ui

import androidx.camera.core.SurfaceRequest
import androidx.compose.ui.geometry.Offset


data class CameraUiState(
    val permissionsGranted: Boolean = false,
    val lensFacing: Int = androidx.camera.core.CameraSelector.LENS_FACING_BACK,
    val surfaceRequest: SurfaceRequest? = null,
    val brightness: Float = 0f,
    val poi: Offset? = null,
    val message: String? = null,

    val frameWidth: Int = 0,
    val frameHeight: Int = 0
    )

sealed interface CameraIntent {
    data object ToggleLens : CameraIntent
    data object CapturePhoto : CameraIntent
    data class OnPermissionsResult(val granted: Boolean) : CameraIntent
    data object ClearMessage : CameraIntent

}