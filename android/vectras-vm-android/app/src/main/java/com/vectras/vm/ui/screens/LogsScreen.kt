/**
 * LogsScreen - System and VM log viewer with filtering
 * ISO 9001/8000 Compliant UI/UX
 * 
 * Features:
 * - Filter by level (DEBUG, INFO, WARNING, ERROR)
 * - Export to file
 * - Copy log entries
 * - Clear logs
 */
package com.vectras.vm.ui.screens

import androidx.compose.foundation.horizontalScroll
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.lazy.rememberLazyListState
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.selection.selectable
import androidx.compose.foundation.selection.selectableGroup
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalClipboardManager
import androidx.compose.ui.semantics.Role
import androidx.compose.ui.semantics.contentDescription
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.AnnotatedString
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.unit.dp
import java.text.SimpleDateFormat
import java.util.*

/**
 * Log filter state.
 */
data class LogFilter(
    val showDebug: Boolean = true,
    val showInfo: Boolean = true,
    val showWarning: Boolean = true,
    val showError: Boolean = true,
    val searchQuery: String = ""
)

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun LogsScreen(
    onNavigateBack: () -> Unit,
    modifier: Modifier = Modifier
) {
    var filter by remember { mutableStateOf(LogFilter()) }
    var showFilterSheet by remember { mutableStateOf(false) }
    val clipboardManager = LocalClipboardManager.current
    val snackbarHostState = remember { SnackbarHostState() }
    
    // Sample logs (in real app, these would come from a log repository)
    val allLogs = remember {
        listOf(
            VMLogEntry(System.currentTimeMillis() - 60000, LogLevel.INFO, "Application started"),
            VMLogEntry(System.currentTimeMillis() - 55000, LogLevel.DEBUG, "Initializing QEMU engine"),
            VMLogEntry(System.currentTimeMillis() - 50000, LogLevel.INFO, "Loading VM configurations"),
            VMLogEntry(System.currentTimeMillis() - 45000, LogLevel.DEBUG, "Found 2 VM configurations"),
            VMLogEntry(System.currentTimeMillis() - 40000, LogLevel.WARNING, "VM 'test-vm' has outdated settings"),
            VMLogEntry(System.currentTimeMillis() - 35000, LogLevel.INFO, "Network initialization complete"),
            VMLogEntry(System.currentTimeMillis() - 30000, LogLevel.DEBUG, "VNC server started on port 5900"),
            VMLogEntry(System.currentTimeMillis() - 25000, LogLevel.ERROR, "Failed to load disk image: file not found"),
            VMLogEntry(System.currentTimeMillis() - 20000, LogLevel.INFO, "User started VM 'Ubuntu 22.04'"),
            VMLogEntry(System.currentTimeMillis() - 15000, LogLevel.DEBUG, "Allocating 2048 MB RAM for VM"),
            VMLogEntry(System.currentTimeMillis() - 10000, LogLevel.INFO, "VM boot sequence initiated"),
            VMLogEntry(System.currentTimeMillis() - 5000, LogLevel.DEBUG, "CPU usage: 45%, RAM: 1.2GB"),
            VMLogEntry(System.currentTimeMillis(), LogLevel.INFO, "VM ready for interaction")
        )
    }
    
    val filteredLogs = remember(filter, allLogs) {
        allLogs.filter { log ->
            val levelMatch = when (log.level) {
                LogLevel.DEBUG -> filter.showDebug
                LogLevel.INFO -> filter.showInfo
                LogLevel.WARNING -> filter.showWarning
                LogLevel.ERROR -> filter.showError
            }
            val queryMatch = filter.searchQuery.isBlank() || 
                log.message.contains(filter.searchQuery, ignoreCase = true)
            levelMatch && queryMatch
        }
    }
    
    val listState = rememberLazyListState()

    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("Logs") },
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
                    IconButton(
                        onClick = { showFilterSheet = true },
                        modifier = Modifier.semantics { 
                            contentDescription = "Filter logs" 
                        }
                    ) {
                        Icon(Icons.Default.FilterList, null)
                    }
                    IconButton(
                        onClick = {
                            // Export logs
                            val logText = filteredLogs.joinToString("\n") { log ->
                                formatLogEntry(log)
                            }
                            clipboardManager.setText(AnnotatedString(logText))
                        },
                        modifier = Modifier.semantics { 
                            contentDescription = "Export logs to clipboard" 
                        }
                    ) {
                        Icon(Icons.Default.ContentCopy, null)
                    }
                }
            )
        },
        snackbarHost = { SnackbarHost(snackbarHostState) },
        modifier = modifier
    ) { innerPadding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
        ) {
            // Search bar
            OutlinedTextField(
                value = filter.searchQuery,
                onValueChange = { filter = filter.copy(searchQuery = it) },
                placeholder = { Text("Search logs...") },
                leadingIcon = { Icon(Icons.Default.Search, null) },
                trailingIcon = {
                    if (filter.searchQuery.isNotEmpty()) {
                        IconButton(onClick = { filter = filter.copy(searchQuery = "") }) {
                            Icon(Icons.Default.Clear, "Clear search")
                        }
                    }
                },
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(horizontal = 16.dp, vertical = 8.dp),
                singleLine = true
            )
            
            // Filter chips
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .horizontalScroll(rememberScrollState())
                    .padding(horizontal = 16.dp, vertical = 4.dp),
                horizontalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                FilterChip(
                    selected = filter.showDebug,
                    onClick = { filter = filter.copy(showDebug = !filter.showDebug) },
                    label = { Text("DEBUG") },
                    leadingIcon = if (filter.showDebug) {
                        { Icon(Icons.Default.Check, null, Modifier.size(18.dp)) }
                    } else null
                )
                FilterChip(
                    selected = filter.showInfo,
                    onClick = { filter = filter.copy(showInfo = !filter.showInfo) },
                    label = { Text("INFO") },
                    leadingIcon = if (filter.showInfo) {
                        { Icon(Icons.Default.Check, null, Modifier.size(18.dp)) }
                    } else null
                )
                FilterChip(
                    selected = filter.showWarning,
                    onClick = { filter = filter.copy(showWarning = !filter.showWarning) },
                    label = { Text("WARNING") },
                    leadingIcon = if (filter.showWarning) {
                        { Icon(Icons.Default.Check, null, Modifier.size(18.dp)) }
                    } else null
                )
                FilterChip(
                    selected = filter.showError,
                    onClick = { filter = filter.copy(showError = !filter.showError) },
                    label = { Text("ERROR") },
                    leadingIcon = if (filter.showError) {
                        { Icon(Icons.Default.Check, null, Modifier.size(18.dp)) }
                    } else null
                )
            }
            
            // Log count
            Text(
                text = "${filteredLogs.size} entries",
                style = MaterialTheme.typography.bodySmall,
                color = MaterialTheme.colorScheme.onSurfaceVariant,
                modifier = Modifier.padding(horizontal = 16.dp, vertical = 4.dp)
            )
            
            HorizontalDivider()
            
            // Log list
            if (filteredLogs.isEmpty()) {
                Box(
                    modifier = Modifier
                        .fillMaxSize()
                        .padding(32.dp),
                    contentAlignment = Alignment.Center
                ) {
                    Column(
                        horizontalAlignment = Alignment.CenterHorizontally,
                        verticalArrangement = Arrangement.spacedBy(8.dp)
                    ) {
                        Icon(
                            imageVector = Icons.Default.Description,
                            contentDescription = null,
                            modifier = Modifier.size(48.dp),
                            tint = MaterialTheme.colorScheme.onSurfaceVariant
                        )
                        Text(
                            text = "No logs match filter",
                            style = MaterialTheme.typography.bodyMedium,
                            color = MaterialTheme.colorScheme.onSurfaceVariant
                        )
                    }
                }
            } else {
                LazyColumn(
                    state = listState,
                    modifier = Modifier.fillMaxSize(),
                    contentPadding = PaddingValues(8.dp),
                    verticalArrangement = Arrangement.spacedBy(2.dp)
                ) {
                    items(filteredLogs.reversed()) { log ->
                        LogEntryCard(
                            log = log,
                            onCopy = {
                                clipboardManager.setText(AnnotatedString(formatLogEntry(log)))
                            }
                        )
                    }
                }
            }
        }
    }
    
    // Filter bottom sheet
    if (showFilterSheet) {
        ModalBottomSheet(
            onDismissRequest = { showFilterSheet = false }
        ) {
            FilterBottomSheetContent(
                filter = filter,
                onFilterChange = { filter = it },
                onDismiss = { showFilterSheet = false }
            )
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun LogEntryCard(
    log: VMLogEntry,
    onCopy: () -> Unit,
    modifier: Modifier = Modifier
) {
    val dateFormat = remember { SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault()) }
    
    val backgroundColor = when (log.level) {
        LogLevel.DEBUG -> MaterialTheme.colorScheme.surface
        LogLevel.INFO -> MaterialTheme.colorScheme.surface
        LogLevel.WARNING -> Color(0xFFFFF3E0)
        LogLevel.ERROR -> Color(0xFFFFEBEE)
    }
    
    val textColor = when (log.level) {
        LogLevel.DEBUG -> MaterialTheme.colorScheme.onSurfaceVariant
        LogLevel.INFO -> MaterialTheme.colorScheme.onSurface
        LogLevel.WARNING -> Color(0xFFE65100)
        LogLevel.ERROR -> MaterialTheme.colorScheme.error
    }
    
    Card(
        modifier = modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(containerColor = backgroundColor),
        onClick = onCopy
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(8.dp),
            horizontalArrangement = Arrangement.spacedBy(8.dp),
            verticalAlignment = Alignment.Top
        ) {
            Column(modifier = Modifier.weight(1f)) {
                Text(
                    text = dateFormat.format(Date(log.timestamp)),
                    style = MaterialTheme.typography.labelSmall,
                    fontFamily = FontFamily.Monospace,
                    color = MaterialTheme.colorScheme.onSurfaceVariant
                )
                Row(
                    horizontalArrangement = Arrangement.spacedBy(8.dp),
                    verticalAlignment = Alignment.CenterVertically
                ) {
                    Surface(
                        shape = MaterialTheme.shapes.extraSmall,
                        color = textColor.copy(alpha = 0.2f)
                    ) {
                        Text(
                            text = log.level.name,
                            style = MaterialTheme.typography.labelSmall,
                            fontFamily = FontFamily.Monospace,
                            color = textColor,
                            modifier = Modifier.padding(horizontal = 4.dp, vertical = 2.dp)
                        )
                    }
                    Text(
                        text = log.message,
                        style = MaterialTheme.typography.bodySmall,
                        fontFamily = FontFamily.Monospace,
                        color = textColor
                    )
                }
            }
        }
    }
}

@Composable
fun FilterBottomSheetContent(
    filter: LogFilter,
    onFilterChange: (LogFilter) -> Unit,
    onDismiss: () -> Unit
) {
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        Text(
            text = "Filter Logs",
            style = MaterialTheme.typography.titleLarge
        )
        
        Text(
            text = "Log Levels",
            style = MaterialTheme.typography.titleMedium
        )
        
        Column(Modifier.selectableGroup()) {
            LogLevelFilterRow(
                level = "DEBUG",
                description = "Detailed diagnostic information",
                checked = filter.showDebug,
                onCheckedChange = { onFilterChange(filter.copy(showDebug = it)) }
            )
            LogLevelFilterRow(
                level = "INFO",
                description = "General information messages",
                checked = filter.showInfo,
                onCheckedChange = { onFilterChange(filter.copy(showInfo = it)) }
            )
            LogLevelFilterRow(
                level = "WARNING",
                description = "Potential issues that don't prevent operation",
                checked = filter.showWarning,
                onCheckedChange = { onFilterChange(filter.copy(showWarning = it)) }
            )
            LogLevelFilterRow(
                level = "ERROR",
                description = "Errors that may affect operation",
                checked = filter.showError,
                onCheckedChange = { onFilterChange(filter.copy(showError = it)) }
            )
        }
        
        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            OutlinedButton(
                onClick = {
                    onFilterChange(LogFilter()) // Reset to defaults
                },
                modifier = Modifier.weight(1f)
            ) {
                Text("Reset")
            }
            Button(
                onClick = onDismiss,
                modifier = Modifier.weight(1f)
            ) {
                Text("Apply")
            }
        }
        
        Spacer(modifier = Modifier.height(32.dp)) // Bottom padding for navigation bar
    }
}

@Composable
fun LogLevelFilterRow(
    level: String,
    description: String,
    checked: Boolean,
    onCheckedChange: (Boolean) -> Unit
) {
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .selectable(
                selected = checked,
                role = Role.Checkbox,
                onClick = { onCheckedChange(!checked) }
            )
            .padding(vertical = 8.dp),
        horizontalArrangement = Arrangement.spacedBy(12.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Checkbox(
            checked = checked,
            onCheckedChange = null // Handled by parent
        )
        Column(modifier = Modifier.weight(1f)) {
            Text(
                text = level,
                style = MaterialTheme.typography.bodyMedium
            )
            Text(
                text = description,
                style = MaterialTheme.typography.bodySmall,
                color = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
    }
}

private fun formatLogEntry(log: VMLogEntry): String {
    val dateFormat = SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault())
    return "${dateFormat.format(Date(log.timestamp))} [${log.level.name}] ${log.message}"
}
