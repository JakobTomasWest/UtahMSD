package com.example.degreeplanner.domain

import androidx.compose.runtime.mutableStateListOf

class Courses {
    private val _courses = mutableStateListOf<Course>() //Courses() wraps our mutable list in a stateful list
    val items: List<Course> get() = _courses

    //Create add function to return true if course object created isn't already on teh list and if successfully added it
    fun add(deptInput: String, numberInput: String): Boolean {
        val num = numberInput.toIntOrNull() ?: return false
        if (num <= 0) return false
        val dept = deptInput.trim().uppercase()
        if (dept.isEmpty()) return false

        val cls = Course(dept, num)
        // works because Course is a data class w/ equals()
        if (_courses.contains(cls)) return false
        _courses.add(cls)
        return true
    }

    fun remove(course: Course) {
        _courses.remove(course)
    }


}