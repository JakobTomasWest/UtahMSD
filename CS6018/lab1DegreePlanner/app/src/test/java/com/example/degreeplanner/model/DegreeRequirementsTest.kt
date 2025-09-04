package com.example.degreeplanner.model

import org.junit.Assert.*
import org.junit.Test

class DegreeRequirementsTest {

    @Test
    fun specificCourse_isMet_whenPlannedContainsIt() {
        val req = DegreeRequirement("Intro to CS", setOf(Course("CS", 101)))
        val degree = DegreeRequirements(listOf(req))
        val planned = listOf(Course("CS", 101))

        assertTrue(degree.isMet(req, planned))
        assertEquals(1, degree.satisfiedCount(planned))
        assertTrue(degree.allSatisfied(planned))
    }

    @Test
    fun oneOf_met_whenAnyOneIsPlanned() {
        val req = DegreeRequirement(
            "Writing (take 1)",
            setOf(Course("WRTG", 1010), Course("ADVWRTG", 2010)),
            minCount = 1
        )
        val degree = DegreeRequirements(listOf(req))

        assertFalse(degree.isMet(req, emptyList()))
        assertTrue(degree.isMet(req, listOf(Course("WRTG", 1010))))
    }

    @Test
    fun pickTwoOfThree_countsCorrectly() {
        val req = DegreeRequirement(
            "Programming core (take 2)",
            setOf(Course("CS", 6011), Course("CS", 6012), Course("CS", 6013)),
            minCount = 2
        )
        val degree = DegreeRequirements(listOf(req))

        assertFalse(degree.isMet(req, listOf(Course("CS", 6011))))
        assertTrue(degree.isMet(req, listOf(Course("CS", 6011), Course("CS", 6012))))
    }

    @Test
    fun allSatisfied_onlyWhenAllReqsAreMet() {
        val reqs = listOf(
            DegreeRequirement("Intro", setOf(Course("CS", 101))),
            DegreeRequirement("Humanities", setOf(Course("PHIL", 101), Course("SOC", 101)), 1)
        )
        val degree = DegreeRequirements(reqs)
        val planned = listOf(Course("CS", 101), Course("PHIL", 101))

        assertEquals(2, degree.satisfiedCount(planned))
        assertTrue(degree.allSatisfied(planned))
    }

    @Test
    fun counts_satisfied_requirements_correctly() {
        val cs101 = Course("CS", 101)
        val cs200 = Course("CS", 200)

        val reqs = DegreeRequirements(
            items = listOf(
                DegreeRequirement(title = "Take CS 101", choices = setOf(cs101), minCount = 1),
                DegreeRequirement(title = "Pick one of CS 200", choices = setOf(cs200), minCount = 1),
            )
        )

        // none
        assertEquals(0, reqs.satisfiedCount(emptyList()))
        // one
        assertEquals(1, reqs.satisfiedCount(listOf(cs101)))
        // both
        assertEquals(2, reqs.satisfiedCount(listOf(cs101, cs200)))
        // allSatisfied
        assertTrue(reqs.allSatisfied(listOf(cs101, cs200)))
    }
}
