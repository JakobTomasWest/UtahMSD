package com.example.degreeplanner.uiComposables.components

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.AssistChip
import androidx.compose.material3.ElevatedCard
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.degreeplanner.domain.Course
import com.example.degreeplanner.domain.DegreeRequirement
import com.example.degreeplanner.domain.DegreeRequirements
import com.example.degreeplanner.ui.theme.DegreePlannerTheme

@Composable
fun RequirementCard(
    req: DegreeRequirement,
    planned: List<Course>,
    degree: DegreeRequirements,
    modifier: Modifier = Modifier.Companion
) {
    val taken = degree.countTaken(req, planned)
    val needed = req.minCount
    val met = taken >= needed
    val progressVal = (taken.toFloat() / needed).coerceIn(0f, 1f)

    ElevatedCard(modifier.fillMaxWidth()) {
        Column(Modifier.Companion.padding(8.dp), verticalArrangement = Arrangement.spacedBy(6.dp)) {
            Row(
                Modifier.Companion.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Text(req.title, style = MaterialTheme.typography.titleMedium)
                AssistChip(onClick = {}, label = { Text(if (met) "Done!" else "$taken / $needed") })
            }

            if (!met) {
                val plannedSet = planned.toSet()
                // Create a list of missing courses by filtering out the planned courses from the requirement choices
                val missing = req.choices.filterNot { it in plannedSet }.sortedBy { it.code }

                LinearProgressIndicator(
                    progress = { progressVal },
                    modifier = Modifier.Companion.fillMaxWidth()
                )
                Column(verticalArrangement = Arrangement.spacedBy(2.dp)) {
                    missing.forEach { m -> Text("• ${m.code}") }
                }
            }
        }
    }
}

@Preview(showBackground = true)
@Composable
fun RequirementCardPreview() {
    val req = DegreeRequirement(
        "Programming core (take 2)",
        setOf(
            Course.Companion("CS", 6011),
            Course.Companion("CS", 6012),
            Course.Companion("CS", 6013)
        ),
        2
    )
    val deg = DegreeRequirements(listOf(req))
    DegreePlannerTheme {
        RequirementCard(req, planned = listOf(Course.Companion("CS", 601)), degree = deg)
    }
}