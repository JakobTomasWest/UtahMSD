package com.example.degreeplanner


import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.material3.Button
import androidx.compose.material3.ElevatedCard
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.degreeplanner.domain.Course
import com.example.degreeplanner.domain.Courses
import com.example.degreeplanner.ui.theme.DegreePlannerTheme


//Create a DegreePlanner Page that has a button for adding or editing courses
//Create a dropdown list of my planned courses for the user to select from
//Create a degreeRequirements list
//Give a progress bar that updates with Courses that are satisified by the user
class MainActivity : ComponentActivity() {
    private val user = "u067502"
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            DegreePlannerTheme {
                val courses = remember { Courses() }


                Scaffold { innerPadding ->
                    PlannerScreen(
                        user = user,
                        courses = courses,
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }


    //CourseList(courses)
//var requirements = remember {mutableListOf(
//requiredCourse(Course("CS", 101))
//required course
//pickone}
    @Composable
    fun PlannerScreen(
        user: String,
        courses: Courses,
        modifier: Modifier = Modifier
    ) {
        var dept by remember { mutableStateOf("") }
        var num by remember { mutableStateOf("") }

        Column(
            modifier = modifier.fillMaxSize().padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Text(
                text = "Degree Planner — Welcome, $user",
                style = MaterialTheme.typography.titleLarge
            )

            Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                OutlinedTextField(
                    value = dept,
                    onValueChange = { dept = it },
                    label = { Text("Dept (e.g., CS)") },
                    singleLine = true,
                    modifier = Modifier.weight(1f)
                )
                OutlinedTextField(
                    value = num,
                    onValueChange = { num = it.filter(Char::isDigit) },
                    label = { Text("Number (e.g., 2420)") },
                    singleLine = true,
                    modifier = Modifier.width(140.dp)
                )
                Button(onClick = {
                    if (courses.add(dept, num)) {   // uses your Courses.add()
                        dept = ""
                        num = ""
                    }
                }) { Text("Add") }
            }

            // Planned Courses list
            if (courses.items.isEmpty()) {
                Text("No courses yet.")
            } else {
                Column(verticalArrangement = Arrangement.spacedBy(8.dp)) {
                    courses.items.forEach { c: Course ->
                        ElevatedCard {
                            Row(
                                Modifier.fillMaxWidth().padding(12.dp),
                                horizontalArrangement = Arrangement.SpaceBetween
                            ) {
                                Text(c.code)
                                TextButton(onClick = { courses.remove(c) }) { Text("Remove") }
                            }
                        }
                    }
                }
            }

            // (You can add your requirement cards below later)
        }
    }
//@Composable
//fun PlannerScreen(user: String, modifier: () -> Unit = Modifier) {
//    Column(modifier = Modifier.fillMaxSize().padding(16.dp)) {
//        Text("Welcome, $user")
//
//    }
//}

//private fun (() -> Unit).fillMaxSize() {
//    TODO("Not yet implemented")
//}

    @Composable
    fun Greeting(name: String, modifier: Modifier = Modifier) {
        Text(
            text = "Hello $name!",
            modifier = modifier
        )
    }

    @Preview(showBackground = true)
    @Composable
    fun GreetingPreview() {
        DegreePlannerTheme {
            Greeting("Android")
        }
    }
}