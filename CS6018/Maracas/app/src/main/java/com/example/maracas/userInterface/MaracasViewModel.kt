package com.example.maracas.userInterface


import android.os.SystemClock
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.maracas.model.Shake
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch



// ViewModel owns the UI-facing state and business logic.
// Right now it's in-memory;we'll swap to Room via a repository.
class MaracasViewModel : ViewModel(){

        //Public state will be exposed here as StateFlow for Compose to collect
    private val _shakes = MutableStateFlow<List<Shake>>(emptyList())
    val shakes: StateFlow<List<Shake>> = _shakes
    private val _capturing = MutableStateFlow(true)
    val capturing: StateFlow<Boolean> = _capturing

    fun setCapturing(enabled: Boolean) {
        _capturing.value = enabled
    }
    // Shake detection parameters
    private val shakeThreshold: Float = 12.0f // m/s²
    private val shakeCooldownMs: Long = 400L // debounce time
    private var lastShakeTimestamp: Long = 0L

    // Call this from the sensor listener when new accelerometer data is available??
    fun onAccelerationNewSample(x: Float, y: Float, z: Float, elapsedRealtime: Long = SystemClock.elapsedRealtime()) {
        if (!_capturing.value) return // Ignore if not capturing

        val mag = Math.sqrt((x * x + y * y + z * z).toDouble()).toFloat()
        val currentTime = elapsedRealtime - lastShakeTimestamp

        if (mag > shakeThreshold && currentTime >= shakeCooldownMs) {
            lastShakeTimestamp = elapsedRealtime
            recordShake(mag)
        }
    }

    // Adds a Shake to state list
    private fun recordShake(intensity: Float) = viewModelScope.launch {
        _shakes.value = _shakes.value + Shake(System.currentTimeMillis(), intensity = intensity)
    }

    // Deletes shakes older than the specified duration in milliseconds
    fun deleteOlderThan(durationMs: Long) {
        val cutoff = System.currentTimeMillis() - durationMs
        _shakes.value = _shakes.value.filter { it.timestamp >= cutoff }
    }


}