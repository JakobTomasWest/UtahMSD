package com.example.degreeplanner.view

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier

import androidx.activity.viewModels
import com.example.degreeplanner.ui.theme.DegreePlannerTheme
import com.example.degreeplanner.view.screen.PlannerScreen
import com.example.degreeplanner.viewmodel.PlannerViewModel

class MainActivity : ComponentActivity() {
    private val user = "u067502"
    private val vm : PlannerViewModel by viewModels() // preserves state when activity is destroyed/recreated - stores viewmodel is longer lived part of memory

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            DegreePlannerTheme {
                Scaffold { innerPadding ->
                    PlannerScreen(
                        user = user,
                        planned = vm.planned,
                        degree = vm.degree,
                        editing = vm.editing,
                        // Events forward to the viewmodel - callbacks to screen
                        onAddCourse     = vm::addCourse,
                        onRemoveCourse  = vm::removeCourse,
                        onStartEdit     = vm::startEditing,
                        onCancelEdit    = vm::cancelEditing,
                        onSaveEdit      = vm::saveEdit,
                        // Plan picking wiring to the viewmodel
                        onFetchPlans = vm::fetchPlans,
                        plans = vm.plans,
                        selectedPlanTitle = vm.selectedPlan?.title,
                        onChoosePlan = vm::loadPlan,
                        loading = vm.loading,
                        error = vm.error,
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }
}