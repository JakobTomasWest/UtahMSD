package com.example.degreeplanner.uiComposables.dialogs

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.unit.dp
import com.example.degreeplanner.domain.Course

@Composable
fun EditCourseDialog(
    course: Course?,
    onDismiss: () -> Unit,
    onSave: (dept: String, number: Int) -> Unit
) {
    if (course == null)
        return
    // take editing state (course fields )from PS for key
    var dept by remember(course) { mutableStateOf(course.dept) }
    var num  by remember(course) { mutableStateOf(course.number.toString()) }
    var error by remember { mutableStateOf<String?>(null) }

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text("Edit course") },
        text = {
            Column(verticalArrangement = Arrangement.spacedBy(8.dp)) {
                OutlinedTextField(
                    dept, onValueChange = { dept = it },
                    label = { Text("Dept") })
                OutlinedTextField(
                    num, onValueChange = { num = it.filter(Char::isDigit) },
                    label = { Text("Number") })
                error?.let { Text(it, color = MaterialTheme.colorScheme.error) }
            }
        },
        confirmButton = {
            TextButton(onClick = {
                val n = num.toIntOrNull()
                if (n == null) error = "Number must be numeric"
                else onSave(dept, n)
            }) { Text("Save") }
        },
        dismissButton = { TextButton(onClick = onDismiss) { Text("Cancel") } }
    )
}