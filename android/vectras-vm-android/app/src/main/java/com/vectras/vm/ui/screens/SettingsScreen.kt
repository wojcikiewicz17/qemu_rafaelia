/**
 * SettingsScreen - Application settings and preferences
 * ISO 9001/8000 Compliant UI/UX
 */
package com.vectras.vm.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
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
import androidx.compose.ui.unit.dp

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun SettingsScreen(
    onNavigateBack: () -> Unit,
    modifier: Modifier = Modifier
) {
    var useDarkTheme by remember { mutableStateOf(false) }
    var useHighContrast by remember { mutableStateOf(false) }
    var enableLargeTouchTargets by remember { mutableStateOf(false) }
    var defaultMemoryMB by remember { mutableStateOf(2048) }
    var autoStartLastVM by remember { mutableStateOf(false) }
    var enableCrashReporting by remember { mutableStateOf(true) }

    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("Settings") },
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
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
                .verticalScroll(rememberScrollState())
        ) {
            // Appearance section
            SettingsSection(title = "Appearance") {
                SettingsSwitchItem(
                    title = "Dark Theme",
                    subtitle = "Use dark color scheme",
                    icon = Icons.Default.DarkMode,
                    checked = useDarkTheme,
                    onCheckedChange = { useDarkTheme = it }
                )
                SettingsSwitchItem(
                    title = "High Contrast",
                    subtitle = "Increase text and UI contrast for better visibility",
                    icon = Icons.Default.Contrast,
                    checked = useHighContrast,
                    onCheckedChange = { useHighContrast = it }
                )
            }
            
            HorizontalDivider(modifier = Modifier.padding(vertical = 8.dp))
            
            // Accessibility section
            SettingsSection(title = "Accessibility") {
                SettingsSwitchItem(
                    title = "Large Touch Targets",
                    subtitle = "Increase button and control sizes (WCAG 2.1 compliant)",
                    icon = Icons.Default.TouchApp,
                    checked = enableLargeTouchTargets,
                    onCheckedChange = { enableLargeTouchTargets = it }
                )
                SettingsInfoItem(
                    title = "Scalable Fonts",
                    subtitle = "Fonts automatically scale with system settings",
                    icon = Icons.Default.TextFields
                )
            }
            
            HorizontalDivider(modifier = Modifier.padding(vertical = 8.dp))
            
            // VM Defaults section
            SettingsSection(title = "VM Defaults") {
                SettingsSliderItem(
                    title = "Default Memory",
                    subtitle = "$defaultMemoryMB MB",
                    icon = Icons.Default.Memory,
                    value = defaultMemoryMB.toFloat(),
                    valueRange = 512f..8192f,
                    steps = 30,
                    onValueChange = { defaultMemoryMB = (it.toInt() / 256) * 256 }
                )
                SettingsSwitchItem(
                    title = "Auto-start Last VM",
                    subtitle = "Automatically start the last used VM on app launch",
                    icon = Icons.Default.PlayArrow,
                    checked = autoStartLastVM,
                    onCheckedChange = { autoStartLastVM = it }
                )
            }
            
            HorizontalDivider(modifier = Modifier.padding(vertical = 8.dp))
            
            // Privacy section
            SettingsSection(title = "Privacy") {
                SettingsSwitchItem(
                    title = "Crash Reporting",
                    subtitle = "Send anonymous crash reports to help improve the app",
                    icon = Icons.Default.BugReport,
                    checked = enableCrashReporting,
                    onCheckedChange = { enableCrashReporting = it }
                )
            }
            
            HorizontalDivider(modifier = Modifier.padding(vertical = 8.dp))
            
            // About section
            SettingsSection(title = "About") {
                SettingsInfoItem(
                    title = "Version",
                    subtitle = "1.0.0 (ISO 9001/8000 Compliant)",
                    icon = Icons.Default.Info
                )
                SettingsInfoItem(
                    title = "QEMU Version",
                    subtitle = "QEMU Rafaelia 9.0.0",
                    icon = Icons.Default.Computer
                )
            }
            
            Spacer(modifier = Modifier.height(16.dp))
        }
    }
}

@Composable
fun SettingsSection(
    title: String,
    content: @Composable ColumnScope.() -> Unit
) {
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .padding(horizontal = 16.dp, vertical = 8.dp)
    ) {
        Text(
            text = title,
            style = MaterialTheme.typography.titleSmall,
            color = MaterialTheme.colorScheme.primary,
            modifier = Modifier.padding(bottom = 8.dp)
        )
        content()
    }
}

@Composable
fun SettingsSwitchItem(
    title: String,
    subtitle: String,
    icon: androidx.compose.ui.graphics.vector.ImageVector,
    checked: Boolean,
    onCheckedChange: (Boolean) -> Unit
) {
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 12.dp),
        horizontalArrangement = Arrangement.spacedBy(16.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Icon(
            imageVector = icon,
            contentDescription = null,
            tint = MaterialTheme.colorScheme.onSurfaceVariant
        )
        Column(modifier = Modifier.weight(1f)) {
            Text(
                text = title,
                style = MaterialTheme.typography.bodyLarge
            )
            Text(
                text = subtitle,
                style = MaterialTheme.typography.bodyMedium,
                color = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
        Switch(
            checked = checked,
            onCheckedChange = onCheckedChange
        )
    }
}

@Composable
fun SettingsSliderItem(
    title: String,
    subtitle: String,
    icon: androidx.compose.ui.graphics.vector.ImageVector,
    value: Float,
    valueRange: ClosedFloatingPointRange<Float>,
    steps: Int,
    onValueChange: (Float) -> Unit
) {
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 12.dp)
    ) {
        Row(
            horizontalArrangement = Arrangement.spacedBy(16.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Icon(
                imageVector = icon,
                contentDescription = null,
                tint = MaterialTheme.colorScheme.onSurfaceVariant
            )
            Column(modifier = Modifier.weight(1f)) {
                Text(
                    text = title,
                    style = MaterialTheme.typography.bodyLarge
                )
                Text(
                    text = subtitle,
                    style = MaterialTheme.typography.bodyMedium,
                    color = MaterialTheme.colorScheme.onSurfaceVariant
                )
            }
        }
        Slider(
            value = value,
            onValueChange = onValueChange,
            valueRange = valueRange,
            steps = steps,
            modifier = Modifier.padding(start = 40.dp)
        )
    }
}

@Composable
fun SettingsInfoItem(
    title: String,
    subtitle: String,
    icon: androidx.compose.ui.graphics.vector.ImageVector
) {
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 12.dp),
        horizontalArrangement = Arrangement.spacedBy(16.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Icon(
            imageVector = icon,
            contentDescription = null,
            tint = MaterialTheme.colorScheme.onSurfaceVariant
        )
        Column(modifier = Modifier.weight(1f)) {
            Text(
                text = title,
                style = MaterialTheme.typography.bodyLarge
            )
            Text(
                text = subtitle,
                style = MaterialTheme.typography.bodyMedium,
                color = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
    }
}
