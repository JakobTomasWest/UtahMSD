package com.example.maracas

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.room.Room
import com.example.maracas.model.AppDatabase
import com.example.maracas.model.ShakeRepository
import com.example.maracas.ui.theme.MaracasTheme
import com.example.maracas.userInterface.MaracasAppWith
import com.example.maracas.userInterface.MaracasViewModel
import com.example.maracas.userInterface.MaracasViewModelFactory

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
//        enableEdgeToEdge()

        // Build Room Database and repository
        val db = Room.databaseBuilder(
            applicationContext,
            AppDatabase::class.java, "shake-database"
        ).build()
        val repo = ShakeRepository(db.shakeDao())

        // Create ViewModel wth repository using factory , control the vms lifecycle
        val vm = ViewModelProvider(this, MaracasViewModelFactory(repo))
            .get(MaracasViewModel::class.java)
        // Entry point for ui through JPC
        setContent {
            val capturing = vm.capturing.collectAsStateWithLifecycle().value //got to use val to use collectAsState and keep flow for auto Ui updates
            val shakes = vm.shakes.collectAsStateWithLifecycle().value

            MaracasAppWith(capturing = capturing,
                shakes = shakes,
                onCapturingChange = vm::setCapturing,
                onDeleteOlderThan = vm::deleteOlderThan,
                onClearAll = vm::clearAll,
                onShakeSample = vm::onAccelerationNewSample)
        }
    }
}

