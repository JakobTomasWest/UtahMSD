package com.example.maracas.userInterface


import androidx.compose.runtime.*
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.maracas.model.Shake
import com.example.maracas.userInterface.effects.SensorCollector
import com.example.maracas.userInterface.screen.MaracasScreen
import com.example.maracas.userInterface.screen.MaracasUIState


@Composable
fun MaracasAppWith(
    capturing: Boolean,
    shakes: List<Shake>,
    onCapturingChange: (Boolean) -> Unit,
    onDeleteOlderThan: (Long) -> Unit,
    onClearAll: () -> Unit,
    onShakeSample: (Float, Float, Float) -> Unit) {

//    val vm: MaracasViewModel = viewModel()


    // Start collecting sensor data when capturing is enabled from the switch MaracasScreen ,
    SensorCollector(enabled = capturing,  onShakeDetected = onShakeSample)

    MaracasScreen(
        state = MaracasUIState(capturing, shakes),
        onCapturingChange = onCapturingChange,
        onDeleteOlderThan = onDeleteOlderThan,
        onClearAll = onClearAll
    )
}