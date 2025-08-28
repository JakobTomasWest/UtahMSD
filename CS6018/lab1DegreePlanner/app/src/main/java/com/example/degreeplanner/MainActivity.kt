package com.example.degreeplanner

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.degreeplanner.domain.Course
import com.example.degreeplanner.domain.Courses
import com.example.degreeplanner.domain.DegreeRequirement
import com.example.degreeplanner.domain.DegreeRequirements
import com.example.degreeplanner.ui.theme.DegreePlannerTheme

private val csDegree = DegreeRequirements(
    listOf(
        DegreeRequirement("Intro to CS", setOf(Course("CS", 101))),
        DegreeRequirement(
            "Programming (must take 2)",
            setOf(Course("CS", 6010), Course("CS", 6011), Course("CS", 6012)),
            minCount = 2
        ),
        DegreeRequirement(
            "Writing (must take 1)",
            setOf(Course("WRTG", 1010), Course("WRTG", 2010))
        ),
        DegreeRequirement(
            "Humanities (must take 1)",
            setOf(Course("PHIL", 101), Course("SOC", 101))
        )
    )
)

class MainActivity : ComponentActivity() {
    private val user = "u067502"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            DegreePlannerTheme {
                val courses = remember { Courses() } //courses holds an instance of Courses class and is remembered across add/remove/edit--
                Scaffold { innerPadding ->
                    PlannerScreen(
                        user = user,
                        courses = courses,
                        degree = csDegree,
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }
}
// Note: Recomposition = a composable re-runs because it read some state that changed.
@Composable
fun PlannerScreen(
    user: String,
    courses: Courses,
    degree: DegreeRequirements,
    modifier: Modifier = Modifier
) {
    var dept by remember { mutableStateOf("") }
    var num  by remember { mutableStateOf("") }
    val planned = courses.items

    var editing: Course? by remember { mutableStateOf(null) }

    val satisfied = degree.satisfiedCount(planned)
    val allDone = degree.allSatisfied(planned)
// lazy column refractoring
    Column(
        modifier = modifier.fillMaxSize().padding(12.dp),
        verticalArrangement = Arrangement.spacedBy(8.dp)
    ) {
        Text("Degree Planner    Welcome, $user!", style = MaterialTheme.typography.titleLarge)

        // Call Composable with current state dept/num inputs and lambdas for updating state and adding course to list
        AddCourseRow(
            dept = dept,
            num = num,
            onDeptChange = { dept = it },
            onNumChange = { num = it },
            onAdd = {
                if (courses.add(dept, num)) { dept = ""; num = "" } //return true if successful and reset fields to empty
            }
        )
        // Call Composable with current list of planned courses and lambdas for editing/removing a course
        PlannedCoursesList(
            planned = planned,
            onEdit = { editing = it },
            onRemove = { courses.remove(it) }
        )

        val readyToGraduate =
            if (allDone) "All requirements met!"
            else "$satisfied / ${degree.items.size} requirements met"
        Text(
            text = readyToGraduate,
            style = MaterialTheme.typography.titleMedium,
            color = if (allDone) Color(0xFF2E7D32) else LocalContentColor.current //
        )
        // For each requirement item in our degree, show a card with its status based on planned courses for now
        degree.items.forEach { req ->
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
    //
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

@Composable
fun PlannedCoursesList(
    planned: List<Course>,
    onEdit: (Course) -> Unit,
    onRemove: (Course) -> Unit,
    modifier: Modifier = Modifier
) {
    if (planned.isEmpty()) {
        Text("No courses yet.")
    } else {
        Column(modifier, verticalArrangement = Arrangement.spacedBy(4.dp)) {
            Text("Courses Planned:", style = MaterialTheme.typography.titleMedium)
            planned.forEach { c ->
                CourseRow(
                    course = c,
                    onEdit = onEdit,
                    onRemove = onRemove
                )
            }
        }
    }
}

@Composable
fun CourseRow(
    course: Course,
    onEdit: (Course) -> Unit,
    onRemove: (Course) -> Unit,
    modifier: Modifier = Modifier
) {
    ElevatedCard(modifier.fillMaxWidth()) {
        Row(
            Modifier.padding(8.dp).fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically
        ) {
            Text(course.code)
            Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                TextButton(onClick = { onEdit(course) }) { Text("Edit") }
                TextButton(onClick = { onRemove(course) }) { Text("Remove") }
            }
        }
    }
}

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
                OutlinedTextField(dept, onValueChange = { dept = it },
                    label = { Text("Dept") })
                OutlinedTextField(num, onValueChange = { num = it.filter(Char::isDigit) },
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

@Composable
fun RequirementCard(
    req: DegreeRequirement,
    planned: List<Course>,
    degree: DegreeRequirements,
    modifier: Modifier = Modifier
) {
    val taken = degree.countTaken(req, planned)
    val needed = req.minCount
    val met = taken >= needed
    val progressVal = (taken.toFloat() / needed).coerceIn(0f, 1f)

    ElevatedCard(modifier.fillMaxWidth()) {
        Column(Modifier.padding(8.dp), verticalArrangement = Arrangement.spacedBy(6.dp)) {
            Row(Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceBetween) {
                Text(req.title, style = MaterialTheme.typography.titleMedium)
                AssistChip(onClick = {}, label = { Text(if (met) "Done!" else "$taken / $needed") })
            }

            if (!met) {
                val plannedSet = planned.toSet()
                // Create a list of missing courses by filtering out the planned courses from the requirement choices
                val missing = req.choices.filterNot { it in plannedSet }.sortedBy { it.code }

                LinearProgressIndicator(progress = { progressVal }, modifier = Modifier.fillMaxWidth())
                Column(verticalArrangement = Arrangement.spacedBy(2.dp)) {
                    missing.forEach { m -> Text("• ${m.code}") }
                }
            }
        }
    }
}

@Preview(showBackground = true)
@Composable
fun CourseRowPreview() {
    DegreePlannerTheme {
        CourseRow(course = Course("CS", 101), onEdit = {}, onRemove = {})
    }
}

@Preview(showBackground = true)
@Composable
fun RequirementCardPreview() {
    val req = DegreeRequirement(
        "Programming core (take 2)",
        setOf(Course("CS", 6011), Course("CS", 6012), Course("CS", 6013)),
        2
    )
    val deg = DegreeRequirements(listOf(req))
    DegreePlannerTheme {
        RequirementCard(req, planned = listOf(Course("CS", 601)), degree = deg)
    }
}
