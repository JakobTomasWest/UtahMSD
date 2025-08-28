package com.example.degreeplanner.domain

data class DegreeRequirement(
    val title: String,
    val choices: Set<Course>,
    val minCount: Int = 1
)
class DegreeRequirements(val items: List<DegreeRequirement>) {

    //Calculate how many courses from the planned list are in the choices for the requirement
    fun countTaken(req: DegreeRequirement, planned: Collection<Course>): Int =
        planned.count { it in req.choices }
    //Check if the number of courses taken meets or exceeds the minimum count required
    fun isMet(req: DegreeRequirement, planned: Collection<Course>): Boolean =
        countTaken(req, planned) >= req.minCount
    //Count how many requirements are satisfied based on the planned courses
    fun satisfiedCount(planned: Collection<Course>): Int =
        items.count { isMet(it, planned) }
    //Check if all requirements are satisfied based on the planned courses
    fun allSatisfied(planned: Collection<Course>): Boolean =
        satisfiedCount(planned) == items.size
}