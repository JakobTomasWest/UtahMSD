package com.example.degreeplanner.view.components

import androidx.compose.foundation.lazy.LazyListScope
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.tooling.preview.Preview
import com.example.degreeplanner.model.Course

fun LazyListScope.PlannedCoursesList(
    planned: List<Course>,
    onEdit: (Course) -> Unit,
    onRemove: (Course) -> Unit
) {
    if (planned.isEmpty()) {
        item { Text("No courses yet.") }
    } else {
        item { Text("Courses Planned:", style = MaterialTheme.typography.titleMedium) }
        items(planned) { c ->
            CourseRow(course = c, onEdit = onEdit, onRemove = onRemove)
        }
    }
}

