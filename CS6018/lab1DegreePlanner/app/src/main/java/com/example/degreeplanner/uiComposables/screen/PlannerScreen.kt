package com.example.degreeplanner.uiComposables.screen

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.LocalContentColor
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import com.example.degreeplanner.domain.Course
import com.example.degreeplanner.domain.Courses
import com.example.degreeplanner.domain.DegreeRequirements
import com.example.degreeplanner.uiComposables.components.AddCourseRow
import com.example.degreeplanner.uiComposables.components.PlannedCoursesList
import com.example.degreeplanner.uiComposables.components.RequirementCard
import com.example.degreeplanner.uiComposables.dialogs.EditCourseDialog


// Note: Recomposition = a composable re-runs because it read some state that changed.
@Composable
fun PlannerScreen(
    user: String,
    courses: Courses,
    degree: DegreeRequirements,
    modifier: Modifier = Modifier
) {
    // RememberSavable was supposed to make rotations keep data
    var dept by rememberSaveable { mutableStateOf("") }
    var num  by rememberSaveable { mutableStateOf("") }
    val planned = courses.items

    var editing: Course? by rememberSaveable { mutableStateOf(null) }

    val satisfied = degree.satisfiedCount(planned)
    val allDone = degree.allSatisfied(planned)
// lazy column refractoring
    LazyColumn(
        modifier = modifier.fillMaxSize().padding(12.dp),
        verticalArrangement = Arrangement.spacedBy(8.dp)
    ) {
        item {
            Text("Degree Planner    Welcome, $user!", style = MaterialTheme.typography.titleLarge)
        }
        // Call Composable with current state dept/num inputs and lambdas for updating state and adding course to list
        item {
            AddCourseRow(
                dept = dept,
                num = num,
                onDeptChange = { dept = it },
                onNumChange = { num = it },
                onAdd = {
                    if (courses.add(dept, num)) {
                        dept = ""; num = ""
                    } //return true if successful and reset fields to empty
                }
            )
        }
        // Not anymore( but its buggy so might have to revert changes)-Call Composable with current list of planned courses and lambdas for editing/removing a course

        PlannedCoursesList(
            planned = planned,
            onEdit = { editing = it },
            onRemove = { courses.remove(it) }
        )

        item {
            val readyToGraduate =
                if (allDone) "All requirements met!"
                else "$satisfied / ${degree.items.size} requirements met"
            Text(
                text = readyToGraduate,
                style = MaterialTheme.typography.titleMedium,
                color = if (allDone) Color(0xFF2E7D32) else LocalContentColor.current
            )
        }

        items(degree.items) { req ->
            RequirementCard(req = req, planned = planned, degree = degree)
        }
    }
    // If edit is selected call composable w/ current course
    EditCourseDialog(
        course = editing,
        onDismiss = { editing = null },
        // ** Only execute if editing block is not null
        onSave = { dep, num -> //when save is clicked, make dep and num local states  the new values for the edited course and delete the old one
            editing?.let { old -> courses.remove(old) }
            courses.add(dep, num.toString())
            editing = null
        }
    )
}

