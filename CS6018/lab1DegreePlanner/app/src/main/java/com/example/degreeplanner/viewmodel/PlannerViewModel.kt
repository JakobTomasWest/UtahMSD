@file:OptIn(kotlinx.serialization.ExperimentalSerializationApi::class)

package com.example.degreeplanner.viewmodel


import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import com.example.degreeplanner.model.*
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
/*ViewModel is the lifecycle-aware holder of UI data. viewModelScope is a CoroutineScope tied
 to the VM for launching jobs that auto-cancel when the VM is cleared.
*/
import io.ktor.client.HttpClient
import io.ktor.client.call.body
import io.ktor.client.engine.cio.CIO
import io.ktor.client.plugins.contentnegotiation.ContentNegotiation
import io.ktor.client.request.get
import io.ktor.serialization.kotlinx.json.json
import kotlinx.coroutines.launch
import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.JsonNames

// Extend VM to give lifecycle awareness and access to viewModelScope
// This VM holds the state for the planner screen: the list of courses and the degree requirements
// Vm will out livescreen to keep state
class PlannerViewModel : ViewModel() {

    // State Internal list of planned courses; Compose will recompose when it changes
    private val _planned = mutableStateListOf<Course>() //wraps mutable list in a stateful list
    val planned: List<Course> get() = _planned //RO public access to list

    var degree: DegreeRequirements by mutableStateOf(Degrees.csDegree)
        private set

    // null or diaglog
    var editing: Course? by mutableStateOf(null); private set

    var plans by mutableStateOf<List<PlanRefDTO>>(emptyList())
        private set
    var selectedPlan : PlanRefDTO? by mutableStateOf(null)
        private set

    // Events to modify the state - called by the UI
    // After ui calls vm function, vm updates state, recomposition happens
    fun addCourse(dept: String, numberText: String) {
        val n = numberText.toIntOrNull() ?: return
        val cls = Course(dept.trim().uppercase(), n)
        if (cls !in _planned) _planned.add(cls)
    }

    fun removeCourse(cls: Course) {
        _planned.remove(cls)
    }

    fun startEditing(cls: Course) {
        editing = cls
    }

    fun cancelEditing() {
        editing = null
    }

    fun saveEdit(dept: String, number: Int) {
        val old = editing ?: return
        val next = Course(dept.trim().uppercase(), number)
        _planned.remove(old)
        if (next !in _planned) _planned.add(next)
        editing = null
    }

    // Show a spinner while we fetch
    var loading by mutableStateOf(false)
        private set
    var error by mutableStateOf<String?>(null); private set

    private val client = HttpClient(CIO) { //Uses CIO engine
        install(ContentNegotiation) {json(
            kotlinx.serialization.json.Json {

                ignoreUnknownKeys = true  // ignore extra fields from server
                isLenient = true
            }
        )
        }  //enables kotlinx.serialization JSON
    }

    private companion object {
        const val INDEX = "https://msd2025.github.io/degreePlans/degreePlans.json"
        const val BASE  = "https://msd2025.github.io/degreePlans/"
    }


    fun fetchPlans() = viewModelScope.launch {
        loading = true
        error = null
        runCatching {
            val index: PlansIndexDTO = client.get(INDEX).body()
            plans =index.plans
        }.onFailure { e ->
            error = e.message ?: e.toString()
            plans = emptyList()
        }.also { loading = false }
    }

    fun loadPlan(ref: PlanRefDTO) = viewModelScope.launch {
        loading = true;
        error = null
        runCatching {
            val dto: DegreePlanDTO = client.get(BASE + ref.path).body()
            degree = dto.toDomain()
            selectedPlan = ref
        }.onFailure { e ->
            error = e.message ?: e.toString()
        }.also { loading = false}
    }


    @Serializable
    data class PlansIndexDTO(val plans: List<PlanRefDTO>)

    @Serializable
    data class PlanRefDTO(
        @SerialName("name") val title: String,
        val path: String
    )
    @Serializable
    data class DegreePlanDTO(
        val name: String,
        val requirements: List<RequirementDTO>
    )

    @Serializable
    data class RequirementDTO(
        val type: String,                          // "requiredCourse" or "oneOf"
        val course: CourseDTO? = null,             // present when type == "requiredCourse"
        val courses: List<CourseDTO>? = null       // present when type == "oneOf"
    )

    @Serializable
    data class CourseDTO(
        val department: String,                    // e.g., "CS"
        val number: String                         // NOTE: string in JSON -> we'll parse to Int
    )

    private fun DegreePlanDTO.toDomain(): DegreeRequirements {
        val items = requirements.mapIndexed { idx, r ->
            val choices: Set<Course> = when {
                r.course != null  -> setOf(r.course.toCourse())
                r.courses != null -> r.courses.map { it.toCourse() }.toSet()
                else              -> emptySet()
            }

            val title = when {
                r.type == "requiredCourse" && r.course != null ->
                    "Take ${r.course.department} ${r.course.number}"
                r.type == "oneOf" && r.courses != null ->
                    "Pick one of: " + r.courses.joinToString { "${it.department} ${it.number}" }
                else -> "Requirement ${idx + 1}"
            }


            val minCount = 1

            DegreeRequirement(
                title = title,
                choices = choices,
                minCount = minCount
            )
        }

        return DegreeRequirements(items)
    }

    // helper to convert the wire course into your domain Course (Int number)
    private fun CourseDTO.toCourse(): Course =
        Course(
            dept = department.trim().uppercase(),
            number = number.toIntOrNull() ?: 0
        )



}

// vm (state) -> Mainactivity(wires) -> PlannerScreen(pass state, event handlers) -> render various composables (read state, call event handlers)
// child composables ( user edits/clicks) -> callbacks -> call vm functions to update state(event methods mutate state)-> PlannerViewModel mutates/updates state -> recomposition happens --> ui updates
