package com.example.maracas.model

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.Query
import kotlinx.coroutines.flow.Flow

@Dao
interface ShakeDao {
    // Reactive query to get all shakes ordered by timestamp descending
    @Query("SELECT * FROM shakes ORDER BY timestamp DESC")
    fun observeAll(): Flow<List<Shake>>

    @Insert
    suspend fun insert(shake: Shake)

    // Delete all shakes older than the specified timestamp
    @Query("DELETE FROM shakes WHERE timestamp < :cutoff")
    suspend fun deleteOlderThan(cutoff: Long)

    @Query("DELETE FROM shakes") suspend fun clear()
}