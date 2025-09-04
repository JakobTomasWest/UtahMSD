package com.example.degreeplanner.view.screen

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import com.example.degreeplanner.model.Course
//import com.example.degreeplanner.model.Courses
import com.example.degreeplanner.model.DegreeRequirements
import com.example.degreeplanner.view.components.AddCourseRow
import com.example.degreeplanner.view.components.PlannedCoursesList
import com.example.degreeplanner.view.components.RequirementCard
import com.example.degreeplanner.view.dialogs.EditCourseDialog
import com.example.degreeplanner.viewmodel.PlannerViewModel


// Note: Recomposition = a composable re-runs because it read some state that changed.
// Read only data + events
@Composable
fun PlannerScreen(
    user: String,
    planned: List<Course>,
    degree: DegreeRequirements,
    editing: Course?,
    onAddCourse: (String, String) -> Unit,
    onRemoveCourse: (Course) -> Unit,
    onStartEdit: (Course) -> Unit,
    onCancelEdit: () -> Unit,
    onSaveEdit: (String, Int) -> Unit,
    plans: List<PlannerViewModel.PlanRefDTO> = emptyList(),
    selectedPlanTitle: String? = null,
    onFetchPlans: (() -> Unit)? = null,
    onChoosePlan: (PlannerViewModel.PlanRefDTO) -> Unit = {},
    loading: Boolean = false,
    error: String? = null,
    modifier: Modifier = Modifier
) {

    // Local state for the text fields in the add course ui
    var dept by rememberSaveable { mutableStateOf("") } // rememberSaveable saves state across recompositions and process death (like screen rotation)
    var num  by rememberSaveable { mutableStateOf("") }

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

        if (onFetchPlans != null) {
            item {
                TextButton(onClick = onFetchPlans) {
                    Text(if (loading) "Fetching degree plans…" else "Fetch degree plans")
                }
                if (error != null) {
                    Text("Error: $error", color = MaterialTheme.colorScheme.error)
                }
            }
        }
        // If the plans are fetched, aka the list is not empty, create buttons for each plan
        if (plans.isNotEmpty()) {
            item {
                Text("Available Degree Plans:", style = MaterialTheme.typography.titleMedium)
                Spacer(Modifier.padding(8.dp))
                Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                    plans.forEach { p ->
                        val selected = p.title == selectedPlanTitle
                        OutlinedButton(
                            onClick = { onChoosePlan(p) },
                            enabled = !loading
                        ) {
                            Text(if (selected) " ${p.title}" else p.title)
                        }

                    }
                }
                Spacer(Modifier.height(12.dp))
            }
        }
        // Call Composable with current state dept/num inputs and lambdas for updating state and adding course to list
        item {
            AddCourseRow(
                dept = dept,
                num = num,
                onDeptChange = { dept = it }, // update dept state when user types
                onNumChange = { num = it },
                onAdd = { // when user clicks "Add" button
                    onAddCourse(dept, num) // call PS param to add course to list
                    // reset input fields
                    dept = ""; num = ""
                }
            )
        }

        // Forward callbacks to list
        PlannedCoursesList(
            planned = planned,
            onEdit =  onStartEdit ,
            onRemove = onRemoveCourse
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
    //Forward callbacks to dialog
    // If edit is selected call composable w/ current course
    EditCourseDialog(
        course = editing,
        onDismiss = onCancelEdit,
        // ** Only execute if editing block is not null
        onSave = onSaveEdit
    )
}

