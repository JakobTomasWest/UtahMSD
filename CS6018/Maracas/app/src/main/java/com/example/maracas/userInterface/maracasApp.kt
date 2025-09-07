package com.example.maracas.userInterface


import androidx.compose.runtime.*
import com.example.maracas.model.Shake
import com.example.maracas.userInterface.screen.MaracasScreen
import com.example.maracas.userInterface.screen.MaracasUIState


@Composable
fun MaracasApp() {
    var capturing by remember { mutableStateOf(true) }
    var shakes by remember {
        mutableStateOf(
            listOf(
                Shake(System.currentTimeMillis() - 30_000, 6.5f),
                Shake(System.currentTimeMillis() - 20_000, 12.3f),
                Shake(System.currentTimeMillis() - 10_000, 9.1f),
            )
        )
    }

    MaracasScreen(
        state = MaracasUIState(capturing, shakes),
        onCapturingChange = { capturing = it },
        onDeleteOlderThan = { durationMs ->
            val cutoff = System.currentTimeMillis() - durationMs
            shakes = shakes.filter { it.timestamp >= cutoff }
        }
    )
}