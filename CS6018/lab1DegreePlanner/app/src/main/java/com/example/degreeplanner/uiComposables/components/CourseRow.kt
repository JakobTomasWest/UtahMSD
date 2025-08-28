package com.example.degreeplanner.uiComposables.components

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ElevatedCard
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.degreeplanner.domain.Course
import com.example.degreeplanner.ui.theme.DegreePlannerTheme

@Composable
fun CourseRow(
    course: Course,
    onEdit: (Course) -> Unit,
    onRemove: (Course) -> Unit,
    modifier: Modifier = Modifier.Companion
) {
    ElevatedCard(modifier.fillMaxWidth()) {
        Row(
            Modifier.Companion.padding(8.dp).fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.Companion.CenterVertically
        ) {
            Text(course.code)
            Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                TextButton(onClick = { onEdit(course) }) { Text("Edit") }
                TextButton(onClick = { onRemove(course) }) { Text("Remove") }
            }
        }
    }
}

@Preview(showBackground = true)
@Composable
fun CourseRowPreview() {
    DegreePlannerTheme {
        CourseRow(course = Course.Companion("CS", 101), onEdit = {}, onRemove = {})
    }
}