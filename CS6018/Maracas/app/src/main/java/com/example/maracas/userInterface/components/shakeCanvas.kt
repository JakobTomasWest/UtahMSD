package com.example.maracas.userInterface.components

import androidx.compose.foundation.Canvas
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.geometry.Size
import androidx.compose.ui.graphics.Color
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.remember
import com.example.maracas.model.Shake

@Composable
fun ShakeCanvas(
    shakes: List<Shake>,
    modifier: Modifier = Modifier,
    barColor: Color = MaterialTheme.colorScheme.primary
) {

    val sorted = remember(shakes) { shakes.sortedBy { it.timestamp } }

    Canvas(modifier) {
        if (sorted.isEmpty()) return@Canvas

        val maxI = (sorted.maxOf { it.intensity }).coerceAtLeast(1f)
        val barW = size.width / sorted.size

        sorted.forEachIndexed { i, s ->
            val h = (s.intensity / maxI) * size.height
            drawRect(
                color = barColor, // uses the parameter now
                topLeft = Offset(i * barW, size.height - h),
                size = Size(barW * 0.8f, h)
            )
        }
    }
}
