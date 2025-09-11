package com.example.maracas.model

import kotlinx.coroutines.flow.Flow

// Abstraction so the ViewModel doesn't know about Room

interface ShakeStorage {
    val shakes: Flow<List<Shake>>
    suspend fun addShake(tsMillis: Long, intensity: Float)
    suspend fun deleteOlderThan(cutoff: Long)
    suspend fun clearAll()
}
//save new shake objects when vm onAccelerationNewSample gets shake data from SensorListener onShakeDetected
class ShakeRepository(private val dao: ShakeDao) : ShakeStorage {
    override val shakes: Flow<List<Shake>> = dao.observeAll()

    override suspend fun addShake(tsMillis: Long, intensity: Float) {
        dao.insert(Shake(timestamp = tsMillis, intensity = intensity))
    }

    override suspend fun deleteOlderThan(cutoff: Long) {
        dao.deleteOlderThan(cutoff)
    }

    override  suspend fun clearAll() = dao.clear()

}