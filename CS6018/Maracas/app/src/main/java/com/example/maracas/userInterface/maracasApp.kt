package com.example.maracas.userInterface


import androidx.compose.runtime.*
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.maracas.model.Shake
import com.example.maracas.userInterface.screen.MaracasScreen
import com.example.maracas.userInterface.screen.MaracasUIState


@Composable
fun MaracasApp() {

    val vm: MaracasViewModel = viewModel()

    // Convert StateFlow to Compose State
    val capturing by vm.capturing.collectAsState()
    val shakes by vm.shakes.collectAsState()


    MaracasScreen(
        state = MaracasUIState(capturing, shakes),
        onCapturingChange = vm::setCapturing,
        onDeleteOlderThan = vm::deleteOlderThan
    )
}