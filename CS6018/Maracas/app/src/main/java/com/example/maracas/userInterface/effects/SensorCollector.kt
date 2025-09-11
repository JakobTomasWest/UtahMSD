package com.example.maracas.userInterface.effects

import android.content.Context
import androidx.compose.ui.platform.LocalContext
import android.hardware.*
import androidx.compose.runtime.*


/**
 * SensorCollector is a composable that uses the device's accelerometer to detect shakes.
 * It calls onShakeDetected with the x, y, z acceleration values (in m/s²) without gravity.
 * If a linear accelerometer is not available, it uses the accelerometer and applies a high-pass filter
 * to remove the gravity component.
 * autocompleted
 * @param enabled If true, the sensor listener is active. If false, it is unregistered.
 * @param onShakeDetected Callback function that receives x, y, z acceleration values when a shake is detected.
 */
@Composable
fun SensorCollector (
    enabled: Boolean,
    onShakeDetected: (x: Float, y: Float, z: Float) -> Unit //passed from MaracasAppWith to vm::onAccelerationNewSample
){
    // Get Anroid Context inside a composable
    val context = LocalContext.current
    // When enables changes, register or unregister the sensor listener -  restart if enabled changes
    DisposableEffect(enabled) {

        //If disabled, do nothing and return
        if (!enabled) return@DisposableEffect onDispose {}
        // Retrieve the Android system's SM service
        val sensorManager = context.getSystemService(Context.SENSOR_SERVICE) as SensorManager
        // Accel w/out force of gravity
        val linear = sensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION)
        val accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)
        // If linear is not null use it otherwise fallback to accelerometer
        val sensor = linear ?: accelerometer
        // Listener calls onShakeDetected with gravity-free acceleration values
        // Build the callback object that will receive sensor events
        val listener = object : SensorEventListener {
            private val gravity = FloatArray(3)
            private val alpha = 0.8f // closer to 1.0 = slower filter ( more smoothing)
            override fun onSensorChanged(event: SensorEvent) {
                when (event.sensor.type) {
                    Sensor.TYPE_LINEAR_ACCELERATION -> {
                        val x = event.values[0]
                        val y = event.values[1]
                        val z = event.values[2]
                        onShakeDetected(x, y, z)
                    }
                    Sensor.TYPE_ACCELEROMETER -> {
                        // Isolate the force of gravity with a low-pass filter.
                        gravity[0] = alpha * gravity[0] + (1f - alpha) * event.values[0]
                        gravity[1] = alpha * gravity[1] + (1f - alpha) * event.values[1]
                        gravity[2] = alpha * gravity[2] + (1f - alpha) * event.values[2]

                        // Remove the gravity contribution with a high-pass filter.
                        val x = event.values[0] - gravity[0]
                        val y = event.values[1] - gravity[1]
                        val z = event.values[2] - gravity[2]
                        onShakeDetected(x, y, z)
                    }
                    else -> Unit
                }
            }
            override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) = Unit
        }
        if (sensor != null) sensorManager.registerListener(listener, sensor, SensorManager.SENSOR_DELAY_GAME)
        // cleanup when disposed or enabled changes or when SensorCollector composable leaves the composition - navigation to new screen
        onDispose { sensorManager.unregisterListener(listener) }
    }

}