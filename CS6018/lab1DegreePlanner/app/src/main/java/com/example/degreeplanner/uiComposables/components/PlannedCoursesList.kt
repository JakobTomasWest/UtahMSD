package com.example.degreeplanner.uiComposables.components

import androidx.compose.foundation.lazy.LazyListScope
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import com.example.degreeplanner.domain.Course

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