package com.example.degreeplanner.domain

data class Course(val dept: String, val number: Int) {
    val code get() = "${dept.uppercase()} $number"
}