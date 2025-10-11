package com.example.augmentedreality.net


import android.content.Context
import android.net.Uri
import io.ktor.client.*
import io.ktor.client.call.body
import io.ktor.client.engine.android.*
import io.ktor.client.plugins.contentnegotiation.*
import io.ktor.client.plugins.logging.*
import io.ktor.client.request.*
import io.ktor.client.statement.*
import io.ktor.serialization.kotlinx.json.*
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json
import io.ktor.client.request.forms.*
import io.ktor.client.plugins.*
import io.ktor.http.ContentType
import io.ktor.http.Headers
import io.ktor.http.HttpHeaders
import io.ktor.http.contentType
import io.ktor.http.isSuccess
import io.ktor.util.*

private const val BASE_URL = "http://10.0.2.2:8080"

@Serializable data class LoginRequest(val username: String, val password: String)
@Serializable data class TokenResponse(val token: String)
@Serializable data class Note(val id: String? = null, val message: String, val public: Boolean)
class ApiClient {

    private val client = HttpClient(Android) {
        install(ContentNegotiation) { json(Json { ignoreUnknownKeys = true }) }
        install(Logging) { level = LogLevel.BODY }
        install(HttpTimeout) { requestTimeoutMillis = 30_000 }
        defaultRequest { url(BASE_URL) }
    }

    suspend fun signUp(username: String, password: String) {
        client.post("/api/user") {
            contentType(ContentType.Application.Json)
            setBody(LoginRequest(username, password))
        }.bodyAsText()
    }

    suspend fun login(username: String, password: String): String =
        client.post("/api/auth") {
            contentType(ContentType.Application.Json)
            setBody(LoginRequest(username, password))
        }.body<TokenResponse>().token


    suspend fun getNotes(token:String): List<Note> =
        client.get("/api/notes") {
            header(HttpHeaders.Authorization, "Bearer $token")
        }.body()

    suspend fun addNote(token: String, message: String, public: Boolean) {
        client.post("/api/notes") {
            header(HttpHeaders.Authorization, "Bearer $token")
            contentType(ContentType.Application.Json)
            setBody(Note(message = message, public = public))
        }.bodyAsText()
    }

    suspend fun uploadImageBytes(
        token: String,
        bytes: ByteArray,
        remoteName: String = "image.png",
        contentType: ContentType = ContentType.Image.PNG
    ): String {
        val safeName = Uri.encode(remoteName)
        val resp: HttpResponse = client.post("/api/upload/$safeName") {
            header(HttpHeaders.Authorization, "Bearer $token")
            contentType(ContentType.MultiPart.FormData)
            setBody(
                MultiPartFormDataContent(
                    formData {
                        append(
                            key = "image",
                            value = bytes,
                            headers = Headers.build {
                                append(HttpHeaders.ContentType, contentType.toString())
                                append(
                                    HttpHeaders.ContentDisposition,
                                    """form-data; name="image"; filename="$remoteName""""
                                )
                            }
                        )
                    }
                )
            )
        }
        if (!resp.status.isSuccess()) {
            throw IllegalStateException("Upload failed: ${resp.status} ${resp.bodyAsText()}")
        }
        return resp.bodyAsText()
    }
    suspend fun uploadImageUri(
        token: String,
        context: Context,
        uri: Uri,
        remoteName: String
    ): String {
        val ct = context.contentResolver.getType(uri) ?: "image/png"
        val bytes = context.contentResolver.openInputStream(uri)!!.use { it.readBytes() }
        return client.submitFormWithBinaryData(
            url = "/api/upload/$remoteName",
            formData = formData {
                append(
                    "image",
                    bytes,
                    Headers.build {
                        append(HttpHeaders.ContentType, ct)
                        append(HttpHeaders.ContentDisposition, "filename=\"$remoteName\"")
                    }
                )
            }
        ) {
            header(HttpHeaders.Authorization, "Bearer $token")
        }.bodyAsText()


    }

    suspend fun listPhotos(token: String): List<String> =
        client.get("/api/upload") {
            header(HttpHeaders.Authorization, "Bearer $token")
            accept(ContentType.Application.Json)
        }.body()

    fun photoUrl(name: String): String = "$BASE_URL/api/upload/$name"
}