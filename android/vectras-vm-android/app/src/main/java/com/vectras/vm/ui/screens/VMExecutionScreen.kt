/**
 * VMExecutionScreen - Real-time VM execution and monitoring
 * ISO 9001/8000 Compliant UI/UX
 * 
 * Features:
 * - Real-time status (CPU%, RAM, I/O, FPS/VNC)
 * - Pause/Resume/Restart controls
 * - Log capture
 */
package com.vectras.vm.ui.screens

import com.vectras.vm.core.VMService
import androidx.compose.animation.animateColorAsState
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.semantics.contentDescription
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.unit.dp
import kotlinx.coroutines.delay
import java.text.SimpleDateFormat
import java.util.*

/**
 * VM runtime metrics.
 */
data class VMMetrics(
    val cpuUsagePercent: Float = 0f,
    val memoryUsedMB: Long = 0L,
    val memoryTotalMB: Long = 0L,
    val diskReadMBps: Float = 0f,
    val diskWriteMBps: Float = 0f,
    val networkRxKBps: Float = 0f,
    val networkTxKBps: Float = 0f,
    val fps: Int = 0,
    val vncConnected: Boolean = false
)

/**
 * VM execution state.
 */
enum class ExecutionState {
    STARTING,
    RUNNING,
    PAUSED,
    STOPPING,
    STOPPED,
    ERROR
}

/**
 * Log entry for VM.
 */
data class VMLogEntry(
    val timestamp: Long,
    val level: LogLevel,
    val message: String
)

enum class LogLevel {
    DEBUG, INFO, WARNING, ERROR
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun VMExecutionScreen(
    vmId: String,
    onNavigateBack: () -> Unit,
    modifier: Modifier = Modifier
) {
    var executionState by remember { mutableStateOf(ExecutionState.STARTING) }
    var metrics by remember { mutableStateOf(VMMetrics()) }
    var logs by remember { mutableStateOf(listOf<VMLogEntry>()) }
    
    // Pull metrics from JNI bridge
    LaunchedEffect(executionState) {
        if (executionState == ExecutionState.STARTING) {
            if (VMService.startVm(vmId)) {
                executionState = ExecutionState.RUNNING
                logs = logs + VMLogEntry(
                    System.currentTimeMillis(),
                    LogLevel.INFO,
                    "VM started successfully"
                )
            } else {
                executionState = ExecutionState.ERROR
                logs = logs + VMLogEntry(
                    System.currentTimeMillis(),
                    LogLevel.ERROR,
                    "Failed to start VM bridge"
                )
            }
        }
        
        while (executionState == ExecutionState.RUNNING) {
            val bridge = VMService.collectMetrics()
            if (bridge != null) {
                metrics = metrics.copy(
                    cpuUsagePercent = bridge.cpuUsagePercent,
                    memoryUsedMB = bridge.memoryUsedMB,
                    memoryTotalMB = bridge.memoryTotalMB,
                    diskReadMBps = bridge.diskReadMBps,
                    diskWriteMBps = bridge.diskWriteMBps,
                    networkRxKBps = bridge.networkRxKBps,
                    networkTxKBps = bridge.networkTxKBps,
                    fps = bridge.fps,
                    vncConnected = bridge.vncConnected
                )
            }
            delay(1000)
        }
    }

    Scaffold(
        topBar = {
            TopAppBar(
                title = {
                    Column {
                        Text("VM Execution")
                        Text(
                            text = "ID: ${vmId.take(8)}...",
                            style = MaterialTheme.typography.bodySmall,
                            color = MaterialTheme.colorScheme.onSurfaceVariant
                        )
                    }
                },
                navigationIcon = {
                    IconButton(
                        onClick = onNavigateBack,
                        modifier = Modifier.semantics { 
                            contentDescription = "Navigate back" 
                        }
                    ) {
                        Icon(
                            imageVector = Icons.AutoMirrored.Filled.ArrowBack,
                            contentDescription = null
                        )
                    }
                },
                actions = {
                    // Status badge
                    StatusBadge(state = executionState)
                }
            )
        },
        modifier = modifier
    ) { innerPadding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
        ) {
            // Control buttons
            ControlBar(
                state = executionState,
                onStart = { executionState = ExecutionState.STARTING },
                onPause = { executionState = ExecutionState.PAUSED },
                onResume = { executionState = ExecutionState.RUNNING },
                onStop = { 
                    executionState = ExecutionState.STOPPING
                    VMService.stopVm()
                    logs = logs + VMLogEntry(
                        System.currentTimeMillis(),
                        LogLevel.INFO,
                        "Stopping VM..."
                    )
                    executionState = ExecutionState.STOPPED
                },
                onRestart = {
                    VMService.stopVm()
                    executionState = ExecutionState.STARTING
                    logs = logs + VMLogEntry(
                        System.currentTimeMillis(),
                        LogLevel.INFO,
                        "Restarting VM..."
                    )
                }
            )
            
            HorizontalDivider()
            
            // Metrics display
            MetricsPanel(
                metrics = metrics,
                modifier = Modifier.padding(16.dp)
            )
            
            HorizontalDivider()
            
            // Logs section
            Text(
                text = "Recent Logs",
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.padding(16.dp, 16.dp, 16.dp, 8.dp)
            )
            
            LazyColumn(
                modifier = Modifier
                    .fillMaxWidth()
                    .weight(1f)
                    .padding(horizontal = 16.dp),
                verticalArrangement = Arrangement.spacedBy(4.dp)
            ) {
                items(logs.takeLast(50).reversed()) { log ->
                    LogEntryItem(log)
                }
            }
        }
    }
}

@Composable
fun StatusBadge(state: ExecutionState) {
    val color by animateColorAsState(
        targetValue = when (state) {
            ExecutionState.RUNNING -> Color(0xFF4CAF50)
            ExecutionState.PAUSED -> Color(0xFFFF9800)
            ExecutionState.STARTING, ExecutionState.STOPPING -> Color(0xFF2196F3)
            ExecutionState.STOPPED -> Color(0xFF757575)
            ExecutionState.ERROR -> Color(0xFFF44336)
        },
        label = "status_color"
    )
    
    Surface(
        shape = MaterialTheme.shapes.small,
        color = color.copy(alpha = 0.2f),
        modifier = Modifier.padding(8.dp)
    ) {
        Row(
            modifier = Modifier.padding(horizontal = 12.dp, vertical = 6.dp),
            horizontalArrangement = Arrangement.spacedBy(6.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Surface(
                modifier = Modifier.size(8.dp),
                shape = MaterialTheme.shapes.extraSmall,
                color = color
            ) {}
            Text(
                text = state.name,
                style = MaterialTheme.typography.labelMedium,
                color = color
            )
        }
    }
}

@Composable
fun ControlBar(
    state: ExecutionState,
    onStart: () -> Unit,
    onPause: () -> Unit,
    onResume: () -> Unit,
    onStop: () -> Unit,
    onRestart: () -> Unit,
    modifier: Modifier = Modifier
) {
    Row(
        modifier = modifier
            .fillMaxWidth()
            .padding(16.dp),
        horizontalArrangement = Arrangement.spacedBy(8.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        when (state) {
            ExecutionState.STOPPED, ExecutionState.ERROR -> {
                Button(
                    onClick = onStart,
                    modifier = Modifier
                        .weight(1f)
                        .semantics { contentDescription = "Start virtual machine" }
                ) {
                    Icon(Icons.Default.PlayArrow, null)
                    Spacer(Modifier.width(8.dp))
                    Text("Start")
                }
            }
            ExecutionState.RUNNING -> {
                OutlinedButton(
                    onClick = onPause,
                    modifier = Modifier
                        .weight(1f)
                        .semantics { contentDescription = "Pause virtual machine" }
                ) {
                    Icon(Icons.Default.Pause, null)
                    Spacer(Modifier.width(8.dp))
                    Text("Pause")
                }
                OutlinedButton(
                    onClick = onRestart,
                    modifier = Modifier
                        .weight(1f)
                        .semantics { contentDescription = "Restart virtual machine" }
                ) {
                    Icon(Icons.Default.Refresh, null)
                    Spacer(Modifier.width(8.dp))
                    Text("Restart")
                }
                Button(
                    onClick = onStop,
                    colors = ButtonDefaults.buttonColors(
                        containerColor = MaterialTheme.colorScheme.error
                    ),
                    modifier = Modifier
                        .weight(1f)
                        .semantics { contentDescription = "Stop virtual machine" }
                ) {
                    Icon(Icons.Default.Stop, null)
                    Spacer(Modifier.width(8.dp))
                    Text("Stop")
                }
            }
            ExecutionState.PAUSED -> {
                Button(
                    onClick = onResume,
                    modifier = Modifier
                        .weight(1f)
                        .semantics { contentDescription = "Resume virtual machine" }
                ) {
                    Icon(Icons.Default.PlayArrow, null)
                    Spacer(Modifier.width(8.dp))
                    Text("Resume")
                }
                Button(
                    onClick = onStop,
                    colors = ButtonDefaults.buttonColors(
                        containerColor = MaterialTheme.colorScheme.error
                    ),
                    modifier = Modifier
                        .weight(1f)
                        .semantics { contentDescription = "Stop virtual machine" }
                ) {
                    Icon(Icons.Default.Stop, null)
                    Spacer(Modifier.width(8.dp))
                    Text("Stop")
                }
            }
            ExecutionState.STARTING, ExecutionState.STOPPING -> {
                Box(
                    modifier = Modifier.fillMaxWidth(),
                    contentAlignment = Alignment.Center
                ) {
                    CircularProgressIndicator(
                        modifier = Modifier.size(24.dp)
                    )
                }
            }
        }
    }
}

@Composable
fun MetricsPanel(
    metrics: VMMetrics,
    modifier: Modifier = Modifier
) {
    Column(
        modifier = modifier.fillMaxWidth(),
        verticalArrangement = Arrangement.spacedBy(12.dp)
    ) {
        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            MetricCard(
                title = "CPU",
                value = "${metrics.cpuUsagePercent.toInt()}%",
                progress = metrics.cpuUsagePercent / 100f,
                modifier = Modifier.weight(1f)
            )
            MetricCard(
                title = "RAM",
                value = "${metrics.memoryUsedMB}/${metrics.memoryTotalMB} MB",
                progress = if (metrics.memoryTotalMB > 0) {
                    metrics.memoryUsedMB.toFloat() / metrics.memoryTotalMB
                } else 0f,
                modifier = Modifier.weight(1f)
            )
        }
        
        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            MetricCard(
                title = "Disk I/O",
                value = "R: ${String.format("%.1f", metrics.diskReadMBps)} / W: ${String.format("%.1f", metrics.diskWriteMBps)} MB/s",
                progress = null,
                modifier = Modifier.weight(1f)
            )
            MetricCard(
                title = "Network",
                value = "↓${metrics.networkRxKBps.toInt()} / ↑${metrics.networkTxKBps.toInt()} KB/s",
                progress = null,
                modifier = Modifier.weight(1f)
            )
        }
        
        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            MetricCard(
                title = "Display",
                value = if (metrics.vncConnected) "${metrics.fps} FPS" else "Disconnected",
                progress = null,
                modifier = Modifier.weight(1f)
            )
            MetricCard(
                title = "VNC",
                value = if (metrics.vncConnected) "Connected" else "Waiting...",
                progress = null,
                modifier = Modifier.weight(1f)
            )
        }
    }
}

@Composable
fun MetricCard(
    title: String,
    value: String,
    progress: Float?,
    modifier: Modifier = Modifier
) {
    Card(
        modifier = modifier,
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surfaceVariant
        )
    ) {
        Column(
            modifier = Modifier.padding(12.dp),
            verticalArrangement = Arrangement.spacedBy(4.dp)
        ) {
            Text(
                text = title,
                style = MaterialTheme.typography.labelSmall,
                color = MaterialTheme.colorScheme.onSurfaceVariant
            )
            Text(
                text = value,
                style = MaterialTheme.typography.bodyMedium
            )
            progress?.let {
                LinearProgressIndicator(
                    progress = { it },
                    modifier = Modifier
                        .fillMaxWidth()
                        .height(4.dp),
                )
            }
        }
    }
}

@Composable
fun LogEntryItem(log: VMLogEntry) {
    val dateFormat = remember { SimpleDateFormat("HH:mm:ss", Locale.getDefault()) }
    val color = when (log.level) {
        LogLevel.DEBUG -> MaterialTheme.colorScheme.onSurfaceVariant
        LogLevel.INFO -> MaterialTheme.colorScheme.onSurface
        LogLevel.WARNING -> Color(0xFFFF9800)
        LogLevel.ERROR -> MaterialTheme.colorScheme.error
    }
    
    Row(
        modifier = Modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.spacedBy(8.dp)
    ) {
        Text(
            text = dateFormat.format(Date(log.timestamp)),
            style = MaterialTheme.typography.bodySmall,
            fontFamily = FontFamily.Monospace,
            color = MaterialTheme.colorScheme.onSurfaceVariant
        )
        Text(
            text = "[${log.level.name.take(1)}]",
            style = MaterialTheme.typography.bodySmall,
            fontFamily = FontFamily.Monospace,
            color = color
        )
        Text(
            text = log.message,
            style = MaterialTheme.typography.bodySmall,
            fontFamily = FontFamily.Monospace,
            color = color,
            modifier = Modifier.weight(1f)
        )
    }
}
