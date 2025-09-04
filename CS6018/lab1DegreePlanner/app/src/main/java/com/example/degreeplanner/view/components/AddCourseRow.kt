package com.example.degreeplanner.view.components

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.width
import androidx.compose.material3.Button
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
//PlannerScreen calls AddCourseRow with current dept and num state values, and lambdas to update those states and add the course
// Creates a row when adding a course
@Composable
fun AddCourseRow(
    dept: String,
    num: String,
    onDeptChange: (String) -> Unit,
    onNumChange: (String) -> Unit,
    onAdd: () -> Unit,
    modifier: Modifier = Modifier
) {
    // child composables the fires call back on user action (onClick of Add button which will call vm.addCourse(dept,num)
    Row(modifier, horizontalArrangement = Arrangement.spacedBy(8.dp)) {
        OutlinedTextField(
            value = dept,  //sets current displayed text value to dept state
            onValueChange = onDeptChange, // when a user types, OutLinedTextField calls onValueChange(newdepttext), onDeptChange will update the dept state in PlannerScreen, trigger recomp, and update the displayed text in value
            label = { Text("Enter Department") }, singleLine = true,
            modifier = Modifier.weight(1f)
        )
        OutlinedTextField(
            value = num,
            onValueChange = { onNumChange(it.filter(Char::isDigit)) },
            label = { Text("Enter course Number") }, singleLine = true,
            modifier = Modifier.width(140.dp)
        )
        Button(onClick = onAdd) { Text("Add") } //call onAdd when clicked
    }
}