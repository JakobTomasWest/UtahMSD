package com.codersee.routing

import io.ktor.http.*
import io.ktor.http.content.*
import io.ktor.server.application.*
import io.ktor.server.request.*
import io.ktor.server.response.*
import io.ktor.server.routing.*
import io.ktor.util.cio.*
import kotlin.io.path.Path
import kotlin.io.path.div
import io.ktor.utils.io.*
import java.nio.file.Files.createDirectories

val uploadDir = Path("uploads")

private fun sanitize(name: String): String =
    name.replace("..","").replace("/","").replace("\\","")

fun Route.uploadRoute(){

    get {
        val username = extractPrincipalUsername(call)!!
        val dir = (uploadDir / username).toFile().apply { mkdirs() }
        val names = dir.listFiles()?.filter { it.isFile }?.map { it.name } ?: emptyList()
        call.respond(names)
    }

    get("{filename}") {
        val username = extractPrincipalUsername(call)!!
        val raw = call.parameters["filename"] ?: return@get call.respond(HttpStatusCode.BadRequest)
        val filename = sanitize(raw)
        val file = (uploadDir / username / filename).toFile()
        if (!file.exists() || file.isDirectory) return@get call.respond(HttpStatusCode.NotFound)
        call.respondFile(file)
    }

    post("{filename}") {
        val username = extractPrincipalUsername(call)!!
        val raw = call.parameters["filename"] ?: return@post call.respond(HttpStatusCode.BadRequest)
        val filename = sanitize(raw)

        // ensure user folder exists
        createDirectories(uploadDir / username)
        val target = (uploadDir / username / filename).toFile()

        var wrote = false
        val multipart = call.receiveMultipart(formFieldLimit = 100 * 1024 * 1024) // 100MB
        multipart.forEachPart { part ->
            when (part) {
                is PartData.FileItem -> {


                    part.provider().copyAndClose(target.writeChannel())
                    wrote = true
                }
                else -> Unit
            }
            part.dispose()
        }

        if (!wrote) return@post call.respond(HttpStatusCode.BadRequest, "No file part (expected 'image').")

        call.respond(HttpStatusCode.Created, target.name)
    }
}