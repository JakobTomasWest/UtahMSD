
val ktor_version: String by project
val kotlin_version: String by project
val logback_version: String by project

plugins {
    kotlin("jvm") version "2.0.21"
    id("io.ktor.plugin") version "3.1.2"
    id("org.jetbrains.kotlin.plugin.serialization") version "2.0.21"
}

group = "com.codersee"
version = "0.0.1"

application {
    mainClass.set("io.ktor.server.netty.EngineMain")

    val isDevelopment: Boolean = project.ext.has("development")
    applicationDefaultJvmArgs = listOf("-Dio.ktor.development=$isDevelopment")
}

repositories {
    mavenCentral()
}

tasks.test {
    useJUnitPlatform()
}

dependencies {
    implementation("io.ktor:ktor-server-auth")
    implementation("io.ktor:ktor-server-core")
    implementation("io.ktor:ktor-server-auth-jwt")
    implementation("io.ktor:ktor-server-content-negotiation")
    implementation("io.ktor:ktor-serialization-kotlinx-json")
    implementation("io.ktor:ktor-server-netty")
    implementation("io.ktor:ktor-utils")


    implementation("ch.qos.logback:logback-classic:$logback_version")

    implementation("at.favre.lib:bcrypt:0.10.2")
    implementation("io.ktor:ktor-client-core-jvm:3.1.2")
    implementation("io.ktor:ktor-client-apache:3.1.2")

    // testImplementation("io.ktor:ktor-server-tests")
    testImplementation(kotlin("test"))
    // testImplementation("org.jetbrains.kotlin:kotlin-test-junit")
    testImplementation("io.ktor:ktor-server-test-host")
    testImplementation("org.jetbrains.kotlin:kotlin-test")
    testImplementation("io.ktor:ktor-client-core")
    testImplementation("io.ktor:ktor-client-content-negotiation")
    testImplementation("io.ktor:ktor-client-auth")
    // testImplementation("io.ktor:ktor-client-auth-jwt")
}
