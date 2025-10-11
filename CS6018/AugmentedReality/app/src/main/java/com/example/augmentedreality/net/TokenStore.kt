package com.example.augmentedreality.net

import android.content.Context
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.stringPreferencesKey
import androidx.datastore.preferences.preferencesDataStore
import kotlinx.coroutines.flow.map

private val Context.dataStore by preferencesDataStore("auth")
private val KEY_TOKEN = stringPreferencesKey("jwt")

class TokenStore(private val context: Context) {

    val tokenFlow = context.dataStore.data.map { it[KEY_TOKEN]}
    suspend fun save(token: String) = context.dataStore.edit {
        it[KEY_TOKEN]= token
}
    suspend fun clear() = context.dataStore.edit {
        it.remove(KEY_TOKEN)
    }
}