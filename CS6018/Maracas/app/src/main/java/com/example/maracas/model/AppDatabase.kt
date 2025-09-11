package com.example.maracas.model

import androidx.room.Database
import androidx.room.RoomDatabase

@Database(entities = [Shake::class], version = 1)
abstract class AppDatabase : RoomDatabase() {
    abstract fun shakeDao(): ShakeDao
}