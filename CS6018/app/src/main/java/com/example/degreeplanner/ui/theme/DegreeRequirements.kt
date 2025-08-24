package com.example.degreeplanner.ui.theme

import com.example.degreeplanner.domain.Course


data class DegreeRequirement(
    val title: String,
    val choices: Set<Course>,
    val minCount: Int = 1
)