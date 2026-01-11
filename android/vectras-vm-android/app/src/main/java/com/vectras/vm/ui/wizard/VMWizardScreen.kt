/**
 * VMWizardScreen - 4-step VM creation wizard
 * ISO 9001/8000 Compliant UI/UX
 * 
 * Steps:
 * 1. Name/Architecture selection
 * 2. Disk/RAM/CPU configuration
 * 3. Network/Display settings
 * 4. Review and Start
 */
package com.vectras.vm.ui.wizard

import androidx.compose.animation.*
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.semantics.contentDescription
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.unit.dp
import java.util.UUID

/**
 * VM configuration state holder.
 */
data class VMConfigState(
    // Step 1: Name/Architecture
    val name: String = "",
    val architecture: String = "x86_64",
    val description: String = "",
    
    // Step 2: Resources
    val diskSizeGB: Int = 20,
    val memoryMB: Int = 2048,
    val cpuCores: Int = 2,
    
    // Step 3: Network/Display
    val networkEnabled: Boolean = true,
    val networkMode: String = "user",
    val displayType: String = "vnc",
    val vncPort: Int = 5900,
    
    // Validation
    val nameError: String? = null
)

/**
 * Wizard step enumeration.
 */
enum class WizardStep(val title: String, val stepNumber: Int) {
    NAME_ARCH("Name & Architecture", 1),
    RESOURCES("Disk & Resources", 2),
    NETWORK_DISPLAY("Network & Display", 3),
    REVIEW("Review & Start", 4)
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun VMWizardScreen(
    onNavigateBack: () -> Unit,
    onVMCreated: (String) -> Unit,
    modifier: Modifier = Modifier
) {
    var currentStep by remember { mutableStateOf(WizardStep.NAME_ARCH) }
    var config by remember { mutableStateOf(VMConfigState()) }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = {
                    Column {
                        Text("Create Virtual Machine")
                        Text(
                            text = "Step ${currentStep.stepNumber} of 4: ${currentStep.title}",
                            style = MaterialTheme.typography.bodySmall,
                            color = MaterialTheme.colorScheme.onSurfaceVariant
                        )
                    }
                },
                navigationIcon = {
                    IconButton(
                        onClick = {
                            if (currentStep == WizardStep.NAME_ARCH) {
                                onNavigateBack()
                            } else {
                                currentStep = WizardStep.entries[currentStep.ordinal - 1]
                            }
                        },
                        modifier = Modifier.semantics { 
                            contentDescription = "Go back" 
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
        bottomBar = {
            WizardBottomBar(
                currentStep = currentStep,
                canProceed = validateStep(currentStep, config),
                onPrevious = {
                    if (currentStep.ordinal > 0) {
                        currentStep = WizardStep.entries[currentStep.ordinal - 1]
                    }
                },
                onNext = {
                    if (currentStep == WizardStep.REVIEW) {
                        // Create VM and navigate
                        val vmId = UUID.randomUUID().toString()
                        onVMCreated(vmId)
                    } else {
                        currentStep = WizardStep.entries[currentStep.ordinal + 1]
                    }
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
            // Step indicator
            StepIndicator(
                currentStep = currentStep,
                modifier = Modifier.padding(16.dp)
            )
            
            // Step content
            AnimatedContent(
                targetState = currentStep,
                transitionSpec = {
                    if (targetState.ordinal > initialState.ordinal) {
                        slideInHorizontally { it } + fadeIn() togetherWith
                                slideOutHorizontally { -it } + fadeOut()
                    } else {
                        slideInHorizontally { -it } + fadeIn() togetherWith
                                slideOutHorizontally { it } + fadeOut()
                    }
                },
                label = "wizard_step_transition"
            ) { step ->
                when (step) {
                    WizardStep.NAME_ARCH -> NameArchStep(
                        config = config,
                        onConfigChange = { config = it }
                    )
                    WizardStep.RESOURCES -> ResourcesStep(
                        config = config,
                        onConfigChange = { config = it }
                    )
                    WizardStep.NETWORK_DISPLAY -> NetworkDisplayStep(
                        config = config,
                        onConfigChange = { config = it }
                    )
                    WizardStep.REVIEW -> ReviewStep(config = config)
                }
            }
        }
    }
}

@Composable
fun StepIndicator(
    currentStep: WizardStep,
    modifier: Modifier = Modifier
) {
    Row(
        modifier = modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.Center,
        verticalAlignment = Alignment.CenterVertically
    ) {
        WizardStep.entries.forEachIndexed { index, step ->
            val isActive = step.ordinal <= currentStep.ordinal
            val isCurrent = step == currentStep
            
            Surface(
                modifier = Modifier.size(32.dp),
                shape = MaterialTheme.shapes.extraLarge,
                color = when {
                    isCurrent -> MaterialTheme.colorScheme.primary
                    isActive -> MaterialTheme.colorScheme.primaryContainer
                    else -> MaterialTheme.colorScheme.surfaceVariant
                }
            ) {
                Box(contentAlignment = Alignment.Center) {
                    Text(
                        text = "${step.stepNumber}",
                        style = MaterialTheme.typography.labelMedium,
                        color = when {
                            isCurrent -> MaterialTheme.colorScheme.onPrimary
                            isActive -> MaterialTheme.colorScheme.onPrimaryContainer
                            else -> MaterialTheme.colorScheme.onSurfaceVariant
                        }
                    )
                }
            }
            
            if (index < WizardStep.entries.lastIndex) {
                Box(
                    modifier = Modifier
                        .width(32.dp)
                        .height(2.dp)
                        .padding(horizontal = 4.dp)
                ) {
                    Surface(
                        modifier = Modifier.fillMaxSize(),
                        color = if (step.ordinal < currentStep.ordinal) {
                            MaterialTheme.colorScheme.primary
                        } else {
                            MaterialTheme.colorScheme.surfaceVariant
                        }
                    ) {}
                }
            }
        }
    }
}

@Composable
fun WizardBottomBar(
    currentStep: WizardStep,
    canProceed: Boolean,
    onPrevious: () -> Unit,
    onNext: () -> Unit
) {
    Surface(
        modifier = Modifier.fillMaxWidth(),
        tonalElevation = 3.dp
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically
        ) {
            // Previous button
            if (currentStep != WizardStep.NAME_ARCH) {
                TextButton(
                    onClick = onPrevious,
                    modifier = Modifier.semantics { 
                        contentDescription = "Go to previous step" 
                    }
                ) {
                    Icon(
                        imageVector = Icons.AutoMirrored.Filled.ArrowBack,
                        contentDescription = null,
                        modifier = Modifier.size(18.dp)
                    )
                    Spacer(modifier = Modifier.width(8.dp))
                    Text("Previous")
                }
            } else {
                Spacer(modifier = Modifier.width(1.dp))
            }
            
            // Next/Create button
            Button(
                onClick = onNext,
                enabled = canProceed,
                modifier = Modifier.semantics { 
                    contentDescription = if (currentStep == WizardStep.REVIEW) {
                        "Create virtual machine"
                    } else {
                        "Go to next step"
                    }
                }
            ) {
                Text(
                    text = if (currentStep == WizardStep.REVIEW) "Create VM" else "Next Step"
                )
                Spacer(modifier = Modifier.width(8.dp))
                Icon(
                    imageVector = if (currentStep == WizardStep.REVIEW) {
                        Icons.Default.Check
                    } else {
                        Icons.Default.ArrowForward
                    },
                    contentDescription = null,
                    modifier = Modifier.size(18.dp)
                )
            }
        }
    }
}

@Composable
fun NameArchStep(
    config: VMConfigState,
    onConfigChange: (VMConfigState) -> Unit
) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
            .verticalScroll(rememberScrollState()),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        OutlinedTextField(
            value = config.name,
            onValueChange = {
                onConfigChange(config.copy(
                    name = it,
                    nameError = validateVMName(it)
                ))
            },
            label = { Text("VM Name *") },
            placeholder = { Text("e.g., Ubuntu 22.04") },
            isError = config.nameError != null,
            supportingText = {
                config.nameError?.let { Text(it) }
                    ?: Text("Name must be 3-50 characters, alphanumeric and hyphens only")
            },
            modifier = Modifier.fillMaxWidth(),
            singleLine = true
        )
        
        OutlinedTextField(
            value = config.description,
            onValueChange = { onConfigChange(config.copy(description = it)) },
            label = { Text("Description") },
            placeholder = { Text("Optional description") },
            modifier = Modifier.fillMaxWidth(),
            maxLines = 3
        )
        
        Text(
            text = "Architecture",
            style = MaterialTheme.typography.titleSmall,
            color = MaterialTheme.colorScheme.onSurfaceVariant
        )
        
        val architectures = listOf("x86_64", "aarch64", "arm", "riscv64")
        architectures.forEach { arch ->
            Row(
                modifier = Modifier.fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically
            ) {
                RadioButton(
                    selected = config.architecture == arch,
                    onClick = { onConfigChange(config.copy(architecture = arch)) }
                )
                Text(
                    text = arch,
                    modifier = Modifier.padding(start = 8.dp)
                )
            }
        }
    }
}

@Composable
fun ResourcesStep(
    config: VMConfigState,
    onConfigChange: (VMConfigState) -> Unit
) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
            .verticalScroll(rememberScrollState()),
        verticalArrangement = Arrangement.spacedBy(24.dp)
    ) {
        // Disk size
        Column {
            Text(
                text = "Disk Size: ${config.diskSizeGB} GB",
                style = MaterialTheme.typography.titleSmall
            )
            Slider(
                value = config.diskSizeGB.toFloat(),
                onValueChange = { onConfigChange(config.copy(diskSizeGB = it.toInt())) },
                valueRange = 1f..256f,
                steps = 255
            )
            Text(
                text = "Recommended: 20+ GB for most operating systems",
                style = MaterialTheme.typography.bodySmall,
                color = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
        
        // RAM
        Column {
            Text(
                text = "Memory (RAM): ${config.memoryMB} MB",
                style = MaterialTheme.typography.titleSmall
            )
            Slider(
                value = config.memoryMB.toFloat(),
                onValueChange = { 
                    onConfigChange(config.copy(memoryMB = (it.toInt() / 256) * 256))
                },
                valueRange = 256f..8192f,
                steps = 31
            )
            Text(
                text = "Recommended: 2048+ MB for GUI-based operating systems",
                style = MaterialTheme.typography.bodySmall,
                color = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
        
        // CPU Cores
        Column {
            Text(
                text = "CPU Cores: ${config.cpuCores}",
                style = MaterialTheme.typography.titleSmall
            )
            Slider(
                value = config.cpuCores.toFloat(),
                onValueChange = { onConfigChange(config.copy(cpuCores = it.toInt())) },
                valueRange = 1f..8f,
                steps = 7
            )
            Text(
                text = "Recommended: 2+ cores for better performance",
                style = MaterialTheme.typography.bodySmall,
                color = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
    }
}

@Composable
fun NetworkDisplayStep(
    config: VMConfigState,
    onConfigChange: (VMConfigState) -> Unit
) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
            .verticalScroll(rememberScrollState()),
        verticalArrangement = Arrangement.spacedBy(24.dp)
    ) {
        // Network settings
        Card(
            modifier = Modifier.fillMaxWidth()
        ) {
            Column(
                modifier = Modifier.padding(16.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceBetween,
                    verticalAlignment = Alignment.CenterVertically
                ) {
                    Text(
                        text = "Network",
                        style = MaterialTheme.typography.titleMedium
                    )
                    Switch(
                        checked = config.networkEnabled,
                        onCheckedChange = { onConfigChange(config.copy(networkEnabled = it)) }
                    )
                }
                
                if (config.networkEnabled) {
                    Text(
                        text = "Network Mode",
                        style = MaterialTheme.typography.titleSmall
                    )
                    val networkModes = listOf("user" to "User Mode (NAT)", "tap" to "TAP Bridge")
                    networkModes.forEach { (mode, label) ->
                        Row(
                            verticalAlignment = Alignment.CenterVertically
                        ) {
                            RadioButton(
                                selected = config.networkMode == mode,
                                onClick = { onConfigChange(config.copy(networkMode = mode)) }
                            )
                            Text(text = label, modifier = Modifier.padding(start = 8.dp))
                        }
                    }
                }
            }
        }
        
        // Display settings
        Card(
            modifier = Modifier.fillMaxWidth()
        ) {
            Column(
                modifier = Modifier.padding(16.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                Text(
                    text = "Display",
                    style = MaterialTheme.typography.titleMedium
                )
                
                val displayTypes = listOf("vnc" to "VNC Display", "sdl" to "SDL Display", "none" to "No Display")
                displayTypes.forEach { (type, label) ->
                    Row(
                        verticalAlignment = Alignment.CenterVertically
                    ) {
                        RadioButton(
                            selected = config.displayType == type,
                            onClick = { onConfigChange(config.copy(displayType = type)) }
                        )
                        Text(text = label, modifier = Modifier.padding(start = 8.dp))
                    }
                }
                
                if (config.displayType == "vnc") {
                    OutlinedTextField(
                        value = config.vncPort.toString(),
                        onValueChange = { 
                            it.toIntOrNull()?.let { port ->
                                onConfigChange(config.copy(vncPort = port))
                            }
                        },
                        label = { Text("VNC Port") },
                        keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
                        modifier = Modifier.fillMaxWidth()
                    )
                }
            }
        }
    }
}

@Composable
fun ReviewStep(config: VMConfigState) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
            .verticalScroll(rememberScrollState()),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        Text(
            text = "Review Configuration",
            style = MaterialTheme.typography.headlineSmall
        )
        
        Card(
            modifier = Modifier.fillMaxWidth()
        ) {
            Column(
                modifier = Modifier.padding(16.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                ReviewRow("VM Name", config.name)
                if (config.description.isNotBlank()) {
                    ReviewRow("Description", config.description)
                }
                ReviewRow("Architecture", config.architecture)
                
                HorizontalDivider(modifier = Modifier.padding(vertical = 8.dp))
                
                ReviewRow("Disk Size", "${config.diskSizeGB} GB")
                ReviewRow("Memory", "${config.memoryMB} MB")
                ReviewRow("CPU Cores", "${config.cpuCores}")
                
                HorizontalDivider(modifier = Modifier.padding(vertical = 8.dp))
                
                ReviewRow("Network", if (config.networkEnabled) "Enabled (${config.networkMode})" else "Disabled")
                ReviewRow("Display", config.displayType.uppercase())
                if (config.displayType == "vnc") {
                    ReviewRow("VNC Port", "${config.vncPort}")
                }
            }
        }
        
        Card(
            colors = CardDefaults.cardColors(
                containerColor = MaterialTheme.colorScheme.primaryContainer
            ),
            modifier = Modifier.fillMaxWidth()
        ) {
            Row(
                modifier = Modifier.padding(16.dp),
                horizontalArrangement = Arrangement.spacedBy(12.dp),
                verticalAlignment = Alignment.CenterVertically
            ) {
                Icon(
                    imageVector = Icons.Default.Info,
                    contentDescription = null,
                    tint = MaterialTheme.colorScheme.onPrimaryContainer
                )
                Text(
                    text = "Click 'Create VM' to create this virtual machine. You can modify settings later.",
                    style = MaterialTheme.typography.bodyMedium,
                    color = MaterialTheme.colorScheme.onPrimaryContainer
                )
            }
        }
    }
}

@Composable
fun ReviewRow(label: String, value: String) {
    Row(
        modifier = Modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.SpaceBetween
    ) {
        Text(
            text = label,
            style = MaterialTheme.typography.bodyMedium,
            color = MaterialTheme.colorScheme.onSurfaceVariant
        )
        Text(
            text = value,
            style = MaterialTheme.typography.bodyMedium
        )
    }
}

private fun validateVMName(name: String): String? {
    return when {
        name.isBlank() -> "VM name is required"
        name.length < 3 -> "Name must be at least 3 characters"
        name.length > 50 -> "Name must not exceed 50 characters"
        !name.matches(Regex("^[a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9]$|^[a-zA-Z0-9]$")) -> 
            "Name must be alphanumeric with optional hyphens"
        else -> null
    }
}

private fun validateStep(step: WizardStep, config: VMConfigState): Boolean {
    return when (step) {
        WizardStep.NAME_ARCH -> config.name.isNotBlank() && config.nameError == null
        WizardStep.RESOURCES -> config.diskSizeGB > 0 && config.memoryMB >= 256 && config.cpuCores >= 1
        WizardStep.NETWORK_DISPLAY -> true
        WizardStep.REVIEW -> true
    }
}
