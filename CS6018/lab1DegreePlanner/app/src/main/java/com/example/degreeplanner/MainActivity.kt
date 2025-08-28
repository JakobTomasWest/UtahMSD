package com.example.degreeplanner

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import com.example.degreeplanner.domain.Courses
import com.example.degreeplanner.domain.Degrees
import com.example.degreeplanner.ui.theme.DegreePlannerTheme
import com.example.degreeplanner.uiComposables.screen.PlannerScreen

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
                        degree = Degrees.csDegree,
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }
}

