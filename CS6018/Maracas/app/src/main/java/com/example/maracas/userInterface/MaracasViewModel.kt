package com.example.maracas.userInterface


import android.os.SystemClock
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.maracas.model.Shake
import com.example.maracas.model.ShakeStorage
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch



// ViewModel owns the UI-facing state and business logic.
// Right now it's in-memory;we'll swap to Room via a repository. --> done
class MaracasViewModel(
    private val repo: ShakeStorage, //provided by MainActivity using factory
    private val shakeThreshold: Float = 12.0f,
    private val shakeCooldownMs: Long = 400L
    ) : ViewModel(){

        //Public state will be exposed here as StateFlow for Compose to collect
    // private val _shakes = MutableStateFlow<List<Shake>>(emptyList())
    // val shakes: StateFlow<List<Shake>> = _shakes
        // Expose the list of shakes to the UI as a StateFlow
    val shakes: StateFlow<List<Shake>> = repo.shakes.stateIn(viewModelScope,
            SharingStarted.Eagerly, // Start collecting immediately and share
            emptyList()
    )
    private val _capturing = MutableStateFlow(true)
    val capturing: StateFlow<Boolean> = _capturing // Expose as public read-only StateFlow

    fun setCapturing(enabled: Boolean) {
        _capturing.value = enabled
    }
    // Shake detection parameters
//    private val shakeThreshold: Float = 12.0f // m/s²
//    private val shakeCooldownMs: Long = 400L // debounce time
    private var lastShakeTimestamp: Long = 0L

    // Call this from the sensor listener when new accelerometer data is available
    fun onAccelerationNewSample(x: Float, y: Float, z: Float, elapsedRealtime: Long = SystemClock.elapsedRealtime()) {
        if (!_capturing.value) return // Ignore if not capturing
        // Calculate the magnitude of the acceleration vector
        val mag = Math.sqrt((x * x + y * y + z * z).toDouble()).toFloat()
        // time since last shake for cooldown or debounce
        val timeSinceLastShake = elapsedRealtime - lastShakeTimestamp

        if (mag >= shakeThreshold && timeSinceLastShake >= shakeCooldownMs) {
            lastShakeTimestamp = elapsedRealtime
            // Launch a coroutine in viewModelScope to call the suspend function in repo
            viewModelScope.launch {
                //trigger add shake to repo - trigger db insert - this will update the flow and ui automatically by recomposition
                repo.addShake(System.currentTimeMillis(), mag)
            }
        }
    }

//    // Adds a Shake to state list
//    private fun recordShake(intensity: Float) = viewModelScope.launch {
//        _shakes.value = _shakes.value + Shake(System.currentTimeMillis(), intensity = intensity)
//    }

    // Deletes shakes older than the specified duration in milliseconds
    fun deleteOlderThan(durationMs: Long) {
        val cutoff = System.currentTimeMillis() - durationMs
        viewModelScope.launch { repo.deleteOlderThan(cutoff) }
    }


    fun clearAll() {
        viewModelScope.launch { repo.clearAll() }
    }


    //    fun deleteOlderThan(durationMs: Long) {
//        val cutoff = System.currentTimeMillis() - durationMs
//        _shakes.value = _shakes.value.filter { it.timestamp >= cutoff }
//    }

}