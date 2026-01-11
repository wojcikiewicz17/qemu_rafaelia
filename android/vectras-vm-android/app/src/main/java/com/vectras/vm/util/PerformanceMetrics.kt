/**
 * PerformanceMetrics - Performance measurement utilities
 * ISO 9001/8000 Compliant - Performance/Stability module
 * 
 * Provides utilities for measuring:
 * - Boot time
 * - Disk throughput (sequential read/write)
 * - Input latency
 * - Memory usage
 */
package com.vectras.vm.util

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.io.File
import java.io.RandomAccessFile
import kotlin.system.measureNanoTime
import kotlin.system.measureTimeMillis

/**
 * Performance measurement result containing timing and throughput data.
 */
data class PerformanceResult(
    val operationType: String,
    val durationMs: Long,
    val throughputBytesPerSecond: Long? = null,
    val sampleCount: Int = 1,
    val metadata: Map<String, String> = emptyMap()
) {
    val throughputMBps: Double?
        get() = throughputBytesPerSecond?.let { it / (1024.0 * 1024.0) }
    
    override fun toString(): String {
        val throughputStr = throughputMBps?.let { 
            String.format("%.2f MB/s", it) 
        } ?: "N/A"
        return "$operationType: ${durationMs}ms, Throughput: $throughputStr"
    }
}

/**
 * Boot time measurement result.
 */
data class BootTimeResult(
    val totalMs: Long,
    val initMs: Long,
    val qemuStartMs: Long,
    val displayConnectMs: Long,
    val bootCompleteMs: Long
) {
    override fun toString(): String = buildString {
        appendLine("Boot Time Breakdown:")
        appendLine("  Initialization: ${initMs}ms")
        appendLine("  QEMU Start: ${qemuStartMs}ms")
        appendLine("  Display Connect: ${displayConnectMs}ms")
        appendLine("  Boot Complete: ${bootCompleteMs}ms")
        appendLine("  Total: ${totalMs}ms")
    }
}

/**
 * Performance metrics collector and analyzer.
 */
object PerformanceMetrics {

    private const val DEFAULT_BLOCK_SIZE = 4 * 1024 // 4KB
    private const val DEFAULT_TEST_SIZE = 16 * 1024 * 1024 // 16MB

    /**
     * Measure sequential disk write throughput.
     * 
     * @param file File to write to
     * @param totalBytes Total bytes to write
     * @param blockSize Block size for each write operation
     * @return Performance result with throughput
     */
    suspend fun measureSequentialWrite(
        file: File,
        totalBytes: Long = DEFAULT_TEST_SIZE.toLong(),
        blockSize: Int = DEFAULT_BLOCK_SIZE
    ): PerformanceResult = withContext(Dispatchers.IO) {
        val buffer = ByteArray(blockSize) { (it % 256).toByte() }
        var bytesWritten = 0L
        
        val durationMs = measureTimeMillis {
            RandomAccessFile(file, "rw").use { raf ->
                while (bytesWritten < totalBytes) {
                    val toWrite = minOf(blockSize.toLong(), totalBytes - bytesWritten).toInt()
                    raf.write(buffer, 0, toWrite)
                    bytesWritten += toWrite
                }
                raf.fd.sync() // Ensure data is flushed to disk
            }
        }
        
        val throughput = if (durationMs > 0) {
            (bytesWritten * 1000) / durationMs
        } else 0L
        
        PerformanceResult(
            operationType = "Sequential Write",
            durationMs = durationMs,
            throughputBytesPerSecond = throughput,
            metadata = mapOf(
                "totalBytes" to bytesWritten.toString(),
                "blockSize" to blockSize.toString()
            )
        )
    }

    /**
     * Measure sequential disk read throughput.
     * 
     * @param file File to read from
     * @param blockSize Block size for each read operation
     * @return Performance result with throughput
     */
    suspend fun measureSequentialRead(
        file: File,
        blockSize: Int = DEFAULT_BLOCK_SIZE
    ): PerformanceResult = withContext(Dispatchers.IO) {
        val buffer = ByteArray(blockSize)
        var bytesRead = 0L
        
        val durationMs = measureTimeMillis {
            RandomAccessFile(file, "r").use { raf ->
                var read: Int
                while (raf.read(buffer).also { read = it } != -1) {
                    bytesRead += read
                }
            }
        }
        
        val throughput = if (durationMs > 0) {
            (bytesRead * 1000) / durationMs
        } else 0L
        
        PerformanceResult(
            operationType = "Sequential Read",
            durationMs = durationMs,
            throughputBytesPerSecond = throughput,
            metadata = mapOf(
                "totalBytes" to bytesRead.toString(),
                "blockSize" to blockSize.toString()
            )
        )
    }

    /**
     * Measure input latency by timing a simple operation.
     * 
     * @param operation The operation to measure
     * @param iterations Number of iterations for averaging
     * @return Performance result with average latency
     */
    inline fun measureInputLatency(
        iterations: Int = 100,
        operation: () -> Unit
    ): PerformanceResult {
        // Warm up
        repeat(10) { operation() }
        
        var totalNanos = 0L
        repeat(iterations) {
            totalNanos += measureNanoTime { operation() }
        }
        
        val avgNanos = totalNanos / iterations
        val avgMs = avgNanos / 1_000_000
        
        return PerformanceResult(
            operationType = "Input Latency",
            durationMs = avgMs,
            sampleCount = iterations,
            metadata = mapOf(
                "avgNanos" to avgNanos.toString(),
                "iterations" to iterations.toString()
            )
        )
    }

    /**
     * Get current memory usage statistics.
     * 
     * @return Map of memory metrics
     */
    fun getMemoryUsage(): Map<String, Long> {
        val runtime = Runtime.getRuntime()
        return mapOf(
            "totalMemory" to runtime.totalMemory(),
            "freeMemory" to runtime.freeMemory(),
            "usedMemory" to (runtime.totalMemory() - runtime.freeMemory()),
            "maxMemory" to runtime.maxMemory()
        )
    }

    /**
     * Create a stopwatch for measuring boot time phases.
     * 
     * @return BootTimeStopwatch instance
     */
    fun createBootStopwatch(): BootTimeStopwatch = BootTimeStopwatch()

    /**
     * Format bytes as human-readable string.
     */
    fun formatBytes(bytes: Long): String {
        return when {
            bytes >= 1024L * 1024L * 1024L -> 
                String.format("%.2f GB", bytes / (1024.0 * 1024.0 * 1024.0))
            bytes >= 1024L * 1024L -> 
                String.format("%.2f MB", bytes / (1024.0 * 1024.0))
            bytes >= 1024L -> 
                String.format("%.2f KB", bytes / 1024.0)
            else -> "$bytes bytes"
        }
    }
}

/**
 * Stopwatch for measuring boot time phases.
 */
class BootTimeStopwatch {
    private val startTime = System.currentTimeMillis()
    private var initTime: Long = 0
    private var qemuStartTime: Long = 0
    private var displayConnectTime: Long = 0
    private var bootCompleteTime: Long = 0

    fun markInitComplete() {
        initTime = System.currentTimeMillis() - startTime
    }

    fun markQemuStarted() {
        qemuStartTime = System.currentTimeMillis() - startTime - initTime
    }

    fun markDisplayConnected() {
        displayConnectTime = System.currentTimeMillis() - startTime - initTime - qemuStartTime
    }

    fun markBootComplete(): BootTimeResult {
        val now = System.currentTimeMillis()
        bootCompleteTime = now - startTime - initTime - qemuStartTime - displayConnectTime
        
        return BootTimeResult(
            totalMs = now - startTime,
            initMs = initTime,
            qemuStartMs = qemuStartTime,
            displayConnectMs = displayConnectTime,
            bootCompleteMs = bootCompleteTime
        )
    }
}
