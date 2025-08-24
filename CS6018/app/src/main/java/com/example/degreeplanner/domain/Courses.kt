package com.example.degreeplanner.domain

import androidx.compose.runtime.mutableStateListOf

class Courses {
    private val _courses = mutableStateListOf<Course>()
    val items: List<Course> get() = _courses

    //Create function to add courses to our DegreePlanner
    fun add(deptInput: String, numberInput: String): Boolean {
        val num = numberInput.toIntOrNull() ?: return false
        if (num <= 0) return false
        val dept = deptInput.trim().uppercase()
        if (dept.isEmpty()) return false

        val cls = Course(dept, num)
        if (_courses.contains(cls)) return false
        _courses.add(cls)
        return true
    }

    fun remove(course: Course) {
        _courses.remove(course)
    }


}