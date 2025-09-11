package com.example.maracas

import com.example.maracas.model.Shake
import com.example.maracas.model.ShakeStorage
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow


class FakeShakeStorage : ShakeStorage {
    private val _shakes = MutableStateFlow<List<Shake>>(emptyList())
    override val shakes = _shakes.asStateFlow()

    override suspend fun addShake(tsMillis: Long, intensity: Float) {
        _shakes.value = _shakes.value + Shake(timestamp = tsMillis, intensity = intensity)
    }

    override suspend fun deleteOlderThan(cutoff: Long) {
        _shakes.value = _shakes.value.filter { it.timestamp >= cutoff }
    }

    override suspend fun clearAll() {
        _shakes.value = emptyList()
    }
}