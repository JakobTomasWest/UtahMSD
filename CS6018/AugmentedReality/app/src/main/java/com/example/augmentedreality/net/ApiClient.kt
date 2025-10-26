package com.example.augmentedreality.net


import android.net.Uri
import org.json.JSONArray
import org.json.JSONObject
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
import io.ktor.http.*
import io.ktor.client.plugins.*
import io.ktor.http.ContentType
import io.ktor.http.Headers
import io.ktor.http.HttpHeaders
import io.ktor.http.contentType
import io.ktor.http.isSuccess

private const val BASE_URL = "http://10.0.2.2:8080"

@Serializable data class LoginRequest(val username: String, val password: String)
@Serializable data class TokenResponse(val token: String)

/** Thrown when the server returns a non-2xx status. */
class HttpStatusException(
    val status: io.ktor.http.HttpStatusCode,
    val payload: String
) : Exception("HTTP ${status.value} ${status.description}: $payload")

class ApiClient(private val debugLogging: Boolean = true) {

    private val client = HttpClient(Android) {
        install(ContentNegotiation) { json(Json { ignoreUnknownKeys = true }) }
        install(Logging) {
            level = if (debugLogging) LogLevel.INFO else LogLevel.NONE
        }
        install(HttpTimeout) { requestTimeoutMillis = 30_000 }
        defaultRequest { url(BASE_URL) }
    }

    suspend fun signUp(username: String, password: String) {
        val resp = client.post("/api/user") {
            contentType(ContentType.Application.Json)
            setBody(LoginRequest(username, password))
            accept(ContentType.Application.Json)
        }
        if (!resp.status.isSuccess()) {
            throw HttpStatusException(resp.status, runCatching { resp.bodyAsText() }.getOrDefault(""))
        }
        // success (201/200) – nothing to return
    }

    suspend fun login(username: String, password: String): String {
        val resp = client.post("/api/auth") {
            contentType(ContentType.Application.Json)
            setBody(LoginRequest(username, password))
            accept(ContentType.Application.Json)
        }
        if (!resp.status.isSuccess()) {
            throw HttpStatusException(resp.status, runCatching { resp.bodyAsText() }.getOrDefault(""))
        }
        return resp.body<TokenResponse>().token
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


    suspend fun listPhotos(token: String): List<String> =
        client.get("/api/upload") {
            header(HttpHeaders.Authorization, "Bearer $token")
            accept(ContentType.Application.Json)
        }.body()

    fun photoUrl(name: String): String = "$BASE_URL/api/upload/${Uri.encode(name)}"



    //  Object Detection (labels + boxes)
    data class ServerDetection(
        val label: String,
        val score: Float,
        val left: Int,
        val top: Int,
        val right: Int,
        val bottom: Int
    )

    private fun parseDetectionsJson(text: String): List<ServerDetection> {
        val arr = JSONArray(text)
        val out = ArrayList<ServerDetection>(arr.length())
        for (i in 0 until arr.length()) {
            val o: JSONObject = arr.getJSONObject(i)
            out.add(
                ServerDetection(
                    label = o.optString("label", "object"),
                    score = o.optDouble("score", 0.0).toFloat(),
                    left = o.optInt("left", 0),
                    top = o.optInt("top", 0),
                    right = o.optInt("right", 0),
                    bottom = o.optInt("bottom", 0)
                )
            )
        }
        return out
    }

    /**
     * Sends raw image bytes to the Ktor detection endpoint and returns a list of detections.
     */
    suspend fun detectObjectsPhotoBytes(
        token: String,
        imageBytes: ByteArray,
        filename: String = "upload.jpg",
        contentType: ContentType = ContentType.Image.JPEG
    ): List<ServerDetection> {
        val response: HttpResponse = client.submitFormWithBinaryData(
            url = "/api/ai/detect-bytes",
            formData = formData {
                append(
                    key = "image",
                    value = imageBytes,
                    headers = Headers.build {
                        append(HttpHeaders.ContentType, contentType.toString())
                        append(HttpHeaders.ContentDisposition, "filename=\"$filename\"")
                    }
                )
            }
        ) {
            header(HttpHeaders.Authorization, "Bearer $token")
            accept(ContentType.Application.Json)
        }

        val status = response.status
        val ctype = response.headers[HttpHeaders.ContentType] ?: ""
        val text = response.bodyAsText()

        if (!status.isSuccess()) {
            throw IllegalStateException("Server $status: ${text.take(200)}")
        }
        if (!ctype.contains("application/json", ignoreCase = true)) {
            throw IllegalStateException("Unexpected Content-Type: $ctype; body preview=\"${text.take(120)}\"")
        }
        if (text.isBlank()) {
            throw IllegalStateException("Empty response body from server")
        }
        return parseDetectionsJson(text)
    }


    suspend fun detectObjectsByName(token: String, name: String): List<ServerDetection> {
        val resp: HttpResponse = client.get {
            url {
                takeFrom(BASE_URL)
                // Safely build /api/ai/detect-name/{filename} and URL-encode the segment
                appendPathSegments("api", "ai", "detect-name", name)
            }
            header(HttpHeaders.Authorization, "Bearer $token")
            accept(ContentType.Application.Json)
        }

        val status = resp.status
        val ctype = resp.headers[HttpHeaders.ContentType] ?: ""
        val text = resp.bodyAsText()

        if (!status.isSuccess()) {
            // Surface server error/plain text instead of trying to deserialize as JSON
            throw IllegalStateException("detect-name failed: $status ${text.take(200)}")
        }
        if (!ctype.contains("application/json", ignoreCase = true)) {
            throw IllegalStateException("Unexpected Content-Type: $ctype; body preview=\"${text.take(120)}\"")
        }
        if (text.isBlank()) {
            throw IllegalStateException("Empty response body from server")
        }
        return parseDetectionsJson(text)
    }
}