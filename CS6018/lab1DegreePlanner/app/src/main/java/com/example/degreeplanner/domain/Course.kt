package com.example.degreeplanner.domain


//makes both the constructor private as well as the copy that was defaulted as public
@ConsistentCopyVisibility
// Create a Course class with department and number properties
data class Course private constructor(val dept: String, val number: Int) {
    // note: use val instead of var because Course fields shouldn't change after creation
    val code: String get() = "$dept $number" // Concatenated property calculated on get

    // AI:A companion object provides a factory method to create Course instances
    companion object {
        // Always capitalize on creation
        operator fun invoke(dept: String, number: Int): Course =
            Course(dept.trim().uppercase(), number)
    }
}
