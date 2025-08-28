package com.example.degreeplanner.uiComposables.components

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.width
import androidx.compose.material3.Button
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp

// Creates a row when adding a course
@Composable
fun AddCourseRow(
    dept: String,
    num: String,
    onDeptChange: (String) -> Unit,
    onNumChange: (String) -> Unit,
    onAdd: () -> Unit,
    modifier: Modifier = Modifier.Companion
) {
    //
    Row(modifier, horizontalArrangement = Arrangement.spacedBy(8.dp)) {
        OutlinedTextField(
            value = dept,  //sets current displayed text value to dept state
            onValueChange = onDeptChange, // when a user types, OutLinedTextField calls onValueChange(newdepttext), onDeptChange will update the dept state in PlannerScreen, trigger recomp, and update the displayed text in value
            label = { Text("Enter Department") }, singleLine = true,
            modifier = Modifier.Companion.weight(1f)
        )
        OutlinedTextField(
            value = num,
            onValueChange = { onNumChange(it.filter(Char::isDigit)) },
            label = { Text("Enter course Number") }, singleLine = true,
            modifier = Modifier.Companion.width(140.dp)
        )
        Button(onClick = onAdd) { Text("Add") } //call onAdd when clicked
    }
}