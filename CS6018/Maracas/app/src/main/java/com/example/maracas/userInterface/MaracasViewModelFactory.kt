package com.example.maracas.userInterface

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.example.maracas.model.ShakeStorage
// This is all chat, still need to learn how its working
class MaracasViewModelFactory (private val repo: ShakeStorage) : ViewModelProvider.Factory {
    @Suppress("UNCHECKED_CAST")
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(MaracasViewModel::class.java)) {
            return MaracasViewModel(repo) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}