package com.example.maracas.userInterface.screen

import androidx.compose.foundation.Canvas
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.*
import androidx.compose.ui.geometry.*
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import com.example.maracas.model.Shake
import java.text.SimpleDateFormat
import java.util.*

data class MaracasUIState(
    val capturing: Boolean,
    val shakes: List<Shake>
)

@Composable
fun MaracasScreen(
    state: MaracasUIState,
    onCapturingChange: (Boolean) -> Unit,
    onDeleteOlderThan: (durationMs: Long) -> Unit,
    modifier: Modifier = Modifier
) {
    Scaffold(

        topBar = {
            Surface(shadowElevation = 4.dp) {
                Row(
                    modifier = Modifier
                        .statusBarsPadding()
                        .fillMaxWidth()
                        .height(56.dp)
                        .padding(horizontal = 16.dp),
                    verticalAlignment = Alignment.CenterVertically,
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Text("Maracas", style = MaterialTheme.typography.titleLarge)
                    Row(verticalAlignment = Alignment.CenterVertically) {
                        Text(if (state.capturing) "On" else "Off")
                        Spacer(Modifier.width(8.dp))
                        Switch(checked = state.capturing, onCheckedChange = onCapturingChange)
                    }
                }
            }
        }
    ) { pad ->
        Column(
            modifier = modifier
                .padding(pad)
                .fillMaxSize()
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(16.dp)
        ) {

            Canvas(Modifier.fillMaxWidth().height(180.dp)) {
                val sorted = state.shakes.sortedBy { it.timestamp }
                val maxI = (sorted.maxOfOrNull { it.intensity } ?: 1f)
                val barW = if (sorted.isEmpty()) 0f else size.width / sorted.size
                val barColor = Color.Black
                sorted.forEachIndexed { i, s ->
                    val h = (s.intensity / maxI) * size.height
                    drawRect(
                        color = barColor,
                        topLeft = Offset(i * barW, size.height - h),
                        size = Size(barW * 0.8f, h)
                    )
                }
            }

            Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                Button(onClick = { onDeleteOlderThan(10 * 60 * 1000L) }) { Text("Delete >10 min") }
                Button(onClick = { onDeleteOlderThan(60 * 60 * 1000L) }) { Text("Delete >1 hr") }
                Button(onClick = { onDeleteOlderThan(24 * 60 * 60 * 1000L) }) { Text("Delete >1 day") }
            }

            // List of past shakes (time + intensity).
            LazyColumn(Modifier.weight(1f)) {
                items(state.shakes) { s -> ShakeRow(s) }
            }
        }
    }
}


@Composable
private fun ShakeRow(s: Shake) {
    val fmt = remember { SimpleDateFormat("HH:mm:ss.SSS", Locale.getDefault()) }
    Row(Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceBetween) {
        Text(fmt.format(Date(s.timestamp)))
        Text(String.format(Locale.getDefault(), "%.2f m/s²", s.intensity))
    }
}