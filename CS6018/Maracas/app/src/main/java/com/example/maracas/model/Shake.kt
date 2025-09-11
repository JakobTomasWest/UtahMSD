package com.example.maracas.model

import androidx.room.Entity
import androidx.room.PrimaryKey


@Entity(tableName = "shakes")
data class Shake(
    @PrimaryKey(autoGenerate = true) val id: Long = 0, //PK
    val timestamp: Long, //val for readonly - dont change it after saving it
    val intensity: Float
)
