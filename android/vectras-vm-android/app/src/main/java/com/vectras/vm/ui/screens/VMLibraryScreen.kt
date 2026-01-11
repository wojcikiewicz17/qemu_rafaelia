/**
 * VMLibraryScreen - Virtual machine library view
 * ISO 9001/8000 Compliant UI/UX
 */
package com.vectras.vm.ui.screens

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
import androidx.compose.ui.semantics.contentDescription
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.unit.dp

/**
 * VM status enumeration.
 */
enum class VMStatus {
    STOPPED,
    RUNNING,
    PAUSED,
    ERROR
}

/**
 * VM data class for display.
 */
data class VMInfo(
    val id: String,
    val name: String,
    val architecture: String,
    val memoryMB: Int,
    val cpuCores: Int,
    val status: VMStatus,
    val lastRun: String?
)

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun VMLibraryScreen(
    onNavigateBack: () -> Unit,
    onNavigateToVM: (String) -> Unit,
    modifier: Modifier = Modifier
) {
    // TODO: Replace with actual VM data from repository
    val vms = remember {
        listOf(
            VMInfo(
                id = "vm-1",
                name = "Ubuntu 22.04",
                architecture = "x86_64",
                memoryMB = 2048,
                cpuCores = 2,
                status = VMStatus.STOPPED,
                lastRun = "2026-01-10"
            ),
            VMInfo(
                id = "vm-2",
                name = "Windows 11",
                architecture = "x86_64",
                memoryMB = 4096,
                cpuCores = 4,
                status = VMStatus.RUNNING,
                lastRun = "2026-01-11"
            )
        )
    }

    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("VM Library") },
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
                }
            )
        },
        modifier = modifier
    ) { innerPadding ->
        if (vms.isEmpty()) {
            EmptyLibraryState(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(innerPadding)
            )
        } else {
            LazyColumn(
                contentPadding = PaddingValues(16.dp),
                verticalArrangement = Arrangement.spacedBy(8.dp),
                modifier = Modifier
                    .fillMaxSize()
                    .padding(innerPadding)
            ) {
                items(vms) { vm ->
                    VMListItem(
                        vm = vm,
                        onClick = { onNavigateToVM(vm.id) }
                    )
                }
            }
        }
    }
}

@Composable
fun VMListItem(
    vm: VMInfo,
    onClick: () -> Unit,
    modifier: Modifier = Modifier
) {
    Card(
        onClick = onClick,
        modifier = modifier
            .fillMaxWidth()
            .semantics { contentDescription = "Virtual machine ${vm.name}, status ${vm.status.name}" }
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalAlignment = Alignment.CenterVertically,
            horizontalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            // Status indicator
            StatusIndicator(status = vm.status)
            
            Column(modifier = Modifier.weight(1f)) {
                Text(
                    text = vm.name,
                    style = MaterialTheme.typography.titleMedium
                )
                Text(
                    text = "${vm.architecture} • ${vm.memoryMB}MB RAM • ${vm.cpuCores} CPU",
                    style = MaterialTheme.typography.bodySmall,
                    color = MaterialTheme.colorScheme.onSurfaceVariant
                )
                vm.lastRun?.let {
                    Text(
                        text = "Last run: $it",
                        style = MaterialTheme.typography.bodySmall,
                        color = MaterialTheme.colorScheme.onSurfaceVariant
                    )
                }
            }
            
            Icon(
                imageVector = Icons.Default.ChevronRight,
                contentDescription = null,
                tint = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
    }
}

@Composable
fun StatusIndicator(
    status: VMStatus,
    modifier: Modifier = Modifier
) {
    val color = when (status) {
        VMStatus.STOPPED -> MaterialTheme.colorScheme.outline
        VMStatus.RUNNING -> MaterialTheme.colorScheme.primary
        VMStatus.PAUSED -> MaterialTheme.colorScheme.secondary
        VMStatus.ERROR -> MaterialTheme.colorScheme.error
    }
    
    Surface(
        modifier = modifier.size(12.dp),
        shape = MaterialTheme.shapes.extraSmall,
        color = color
    ) {}
}

@Composable
fun EmptyLibraryState(modifier: Modifier = Modifier) {
    Column(
        modifier = modifier,
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        Icon(
            imageVector = Icons.Default.Storage,
            contentDescription = null,
            modifier = Modifier.size(64.dp),
            tint = MaterialTheme.colorScheme.onSurfaceVariant
        )
        Spacer(modifier = Modifier.height(16.dp))
        Text(
            text = "No Virtual Machines",
            style = MaterialTheme.typography.titleMedium,
            color = MaterialTheme.colorScheme.onSurface
        )
        Text(
            text = "Create a new VM to get started",
            style = MaterialTheme.typography.bodyMedium,
            color = MaterialTheme.colorScheme.onSurfaceVariant
        )
    }
}
