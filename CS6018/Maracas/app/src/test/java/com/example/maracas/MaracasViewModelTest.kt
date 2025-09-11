// MaracasViewModelTest.kt
@file:OptIn(kotlinx.coroutines.ExperimentalCoroutinesApi::class)

package com.example.maracas

import android.util.Log
import com.example.maracas.userInterface.MaracasViewModel
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.test.*

import org.junit.After
import org.junit.Before
import org.junit.Test
import org.junit.Assert.assertEquals

class MaracasViewModelTest {

    private val dispatcher = StandardTestDispatcher()

    @Before fun setup() {
        // Make viewModelScope (Main) use our test dispatcher
        Dispatchers.setMain(dispatcher)
    }

    @After fun tearDown() {
        Dispatchers.resetMain()
    }

    @Test
    fun `records only when over threshold with respect to cooldown`() = runTest {
        val storage = FakeShakeStorage()
        val vm = MaracasViewModel(storage, shakeThreshold = 10f, shakeCooldownMs = 400L)

        vm.setCapturing(true)

        // First sample below threshold (ignored)
        vm.onAccelerationNewSample(3f, 3f, 3f, elapsedRealtime = 0L);     advanceUntilIdle()

        // Because lastShakeTimestamp starts at 0L, the FIRST valid shake must be >= 400ms
        vm.onAccelerationNewSample(10f, 0f, 0f, elapsedRealtime = 450L);  advanceUntilIdle() // record #1

        // Inside cooldown (450 -> 650 = 200ms): ignored
        vm.onAccelerationNewSample(11f, 0f, 0f, elapsedRealtime = 650L);  advanceUntilIdle()

        // Cooldown passed (450 -> 900 = 450ms): record #2
        vm.onAccelerationNewSample(12f, 0f, 0f, elapsedRealtime = 900L);  advanceUntilIdle()

        assertEquals(2, storage.shakes.value.size)
    }

    @Test
    fun `deleteOlderThan keeps only recent items`() = runTest {
        val storage = FakeShakeStorage()
        val vm = MaracasViewModel(storage)

        val now = System.currentTimeMillis()
        storage.addShake(now - 70*60_000, 7f)   // 70 min old (should be deleted)
        storage.addShake(now - 10*60_000, 9f)   // 10 min old (keep)
        storage.addShake(now - 30_000,   8f)    // 30 sec old (keep)

        vm.deleteOlderThan(60 * 60 * 1000L)     // >1 hour
        advanceUntilIdle()

        assertEquals(2, storage.shakes.value.size)
    }

    @Test
    fun `clearAll wipes everything`() = runTest {
        val storage = FakeShakeStorage()
        val vm = MaracasViewModel(storage)

        storage.addShake(System.currentTimeMillis(), 8f)
        storage.addShake(System.currentTimeMillis(), 9f)
        assertEquals(2, storage.shakes.value.size)

        vm.clearAll()
        advanceUntilIdle()

        assertEquals(0, storage.shakes.value.size)
    }
}
