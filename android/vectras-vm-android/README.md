# Vectras VM Android - Performance & Integrity Validation Guide

## ISO 9001:2015 / ISO 8000 Compliant

**Version**: 1.0.0  
**Date**: 2026-01-11  
**Standard Compliance**: ISO 9001:2015 (Quality Management), ISO 8000 (Data Quality)

---

## Table of Contents

1. [Overview](#overview)
2. [Performance Measurement](#performance-measurement)
3. [Integrity Validation](#integrity-validation)
4. [Running Tests](#running-tests)
5. [Anti-Bug-Logic Principles](#anti-bug-logic-principles)

---

## Overview

This document describes how to measure performance and validate integrity in the Vectras VM Android application. All implementations follow ISO quality standards and include comprehensive test coverage.

### Key Principles

1. **No OCR-Derived Calculations**: All mathematical operations are implemented directly in code, never derived from text extracted from images.
2. **Golden Test Vectors**: Fixed input/output pairs validate mathematical correctness.
3. **Stable Implementations**: Mathematical functions use proven algorithms with documented formulas.
4. **Comprehensive Testing**: Unit tests cover edge cases and validate against known values.

---

## Performance Measurement

### Using PerformanceMetrics

The `PerformanceMetrics` utility provides measurement functions for:

#### Boot Time Measurement

```kotlin
import com.vectras.vm.util.PerformanceMetrics

// Create a boot stopwatch
val stopwatch = PerformanceMetrics.createBootStopwatch()

// Mark phases as they complete
stopwatch.markInitComplete()
stopwatch.markQemuStarted()
stopwatch.markDisplayConnected()

// Get final results
val bootResult = stopwatch.markBootComplete()
println(bootResult)
// Output:
// Boot Time Breakdown:
//   Initialization: 150ms
//   QEMU Start: 800ms
//   Display Connect: 200ms
//   Boot Complete: 5000ms
//   Total: 6150ms
```

#### Disk Throughput Measurement

```kotlin
import com.vectras.vm.util.PerformanceMetrics
import kotlinx.coroutines.runBlocking
import java.io.File

runBlocking {
    val testFile = File(cacheDir, "perf_test.bin")
    
    // Measure sequential write
    val writeResult = PerformanceMetrics.measureSequentialWrite(
        file = testFile,
        totalBytes = 16 * 1024 * 1024, // 16 MB
        blockSize = 4096 // 4 KB blocks
    )
    println(writeResult) // Sequential Write: 250ms, Throughput: 64.00 MB/s
    
    // Measure sequential read
    val readResult = PerformanceMetrics.measureSequentialRead(
        file = testFile,
        blockSize = 4096
    )
    println(readResult) // Sequential Read: 100ms, Throughput: 160.00 MB/s
    
    testFile.delete()
}
```

#### Input Latency Measurement

```kotlin
val latencyResult = PerformanceMetrics.measureInputLatency(iterations = 100) {
    // Operation to measure
    someInputOperation()
}
println("Average latency: ${latencyResult.durationMs}ms")
```

#### Memory Usage

```kotlin
val memoryStats = PerformanceMetrics.getMemoryUsage()
println("Used memory: ${PerformanceMetrics.formatBytes(memoryStats["usedMemory"]!!)}")
println("Max memory: ${PerformanceMetrics.formatBytes(memoryStats["maxMemory"]!!)}")
```

---

## Integrity Validation

### Mathematical Constants

All mathematical constants are defined directly in code:

```kotlin
import com.vectras.vm.core.MathUtils

// Constants - NOT derived from OCR
val phi = MathUtils.PHI        // 1.6180339887498949 (Golden ratio)
val euler = MathUtils.EULER    // 2.7182818284590452 (e)
val pi = MathUtils.PI          // 3.1415926535897932
val ln2 = MathUtils.LN2        // 0.6931471805599453
val sqrt2 = MathUtils.SQRT2    // 1.4142135623730951
```

### Mathematical Functions

All functions have documented formulas and edge case handling:

```kotlin
// Integer log base 2 - O(1) using bit manipulation
val log = MathUtils.log2Int(1024L)  // Returns 10

// Integer square root - Newton-Raphson method
val sqrt = MathUtils.sqrtInt(100L)  // Returns 10

// CRC-32 - IEEE 802.3 polynomial
val crc = MathUtils.crc32("123456789".toByteArray())  // Returns 0xCBF43926

// Population count - parallel bit counting
val bits = MathUtils.popCount(0xFFL)  // Returns 8

// Power of 2 check - bit manipulation
val isPow2 = MathUtils.isPowerOfTwo(1024L)  // Returns true
```

### Golden Test Vectors

Validate implementations against known correct values:

```kotlin
import com.vectras.vm.core.GoldenTestVectors

// CRC-32 standard check value
val crcVector = GoldenTestVectors.CRC32_VECTORS.find { 
    it.description.contains("123456789") 
}
// input: "123456789", expected: 0xCBF43926

// Log2 vectors
GoldenTestVectors.LOG2_INT_VECTORS.forEach { vector ->
    val result = MathUtils.log2Int(vector.input)
    assert(result == vector.expected) { vector.description }
}
```

---

## Running Tests

### Unit Tests

Run all mathematical validation tests:

```bash
cd android/vectras-vm-android

# Run all unit tests
./gradlew test

# Run specific test class
./gradlew test --tests "com.vectras.vm.core.MathUtilsTest"

# Run with verbose output
./gradlew test --info
```

### Test Categories

1. **Log2 Tests**: Validate integer and floating-point log base 2
2. **Square Root Tests**: Validate integer and floating-point sqrt
3. **CRC-32 Tests**: Validate against IEEE 802.3 standard values
4. **Bit Manipulation Tests**: Rotation, population count, power of 2
5. **Constant Tests**: Validate mathematical constants match expected values

### Expected Output

```
> Task :app:testDebugUnitTest

MathUtilsTest > log2Int returns correct values for powers of 2 PASSED
MathUtilsTest > log2Int validates against all golden vectors PASSED
MathUtilsTest > sqrtInt returns exact values for perfect squares PASSED
MathUtilsTest > sqrtInt validates against all golden vectors PASSED
MathUtilsTest > crc32 returns standard check value for 123456789 PASSED
MathUtilsTest > crc32 validates against all golden vectors PASSED
MathUtilsTest > constants match expected values PASSED
MathUtilsTest > PHI satisfies golden ratio property PASSED
MathUtilsTest > PHI squared equals PHI plus 1 PASSED
...

BUILD SUCCESSFUL
```

---

## Anti-Bug-Logic Principles

### 1. No OCR-Derived Values

❌ **FORBIDDEN**: Extracting numbers from images or screenshots
```kotlin
// NEVER DO THIS
val constant = parseOcrResult(screenshot) // FORBIDDEN
```

✅ **REQUIRED**: Define values directly in code
```kotlin
// ALWAYS DO THIS
const val PHI = 1.6180339887498949  // Golden ratio: (1 + sqrt(5)) / 2
```

### 2. Document Mathematical Basis

Every mathematical function must include:
- Formula reference
- Algorithm description
- Edge case handling
- Test vectors

```kotlin
/**
 * Compute integer log base 2 using bit manipulation.
 * Mathematical basis: log2(n) = floor(log(n) / log(2))
 * 
 * Implementation uses leading zero count for O(1) lookup.
 * 
 * @param value Non-negative integer
 * @return Floor of log2(value), or -1 if value <= 0
 */
fun log2Int(value: Long): Int { ... }
```

### 3. Golden Test Vectors

Maintain fixed input/output pairs for validation:

```kotlin
data class TestVector(
    val input: Long,
    val expected: Int,
    val description: String
)

val LOG2_VECTORS = listOf(
    TestVector(1L, 0, "log2(1) = 0"),
    TestVector(2L, 1, "log2(2) = 1"),
    TestVector(4L, 2, "log2(4) = 2"),
    // ... comprehensive coverage
)
```

### 4. Stable Implementations

Use proven algorithms with known properties:

| Operation | Algorithm | Complexity | Reference |
|-----------|-----------|------------|-----------|
| log2Int | Bit counting | O(1) | Standard |
| sqrtInt | Newton-Raphson | O(log n) | Numerical Methods |
| crc32 | Polynomial division | O(n) | IEEE 802.3 |
| popCount | Parallel counting | O(1) | HAKMEM 169 |

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0.0 | 2026-01-11 | Initial release |

---

**FIAT LUX ΣΩΔΦBITRAF** 💎
