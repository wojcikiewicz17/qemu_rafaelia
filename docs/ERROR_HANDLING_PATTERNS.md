# QEMU Error Handling Patterns for Android Integration
## Professional Error Codes and User-Facing Messages

**Version**: 1.0.0  
**Date**: 2026-01-10  
**Standard Compliance**: ISO 9001:2015 (Quality Management), ISO 8000 (Data Quality)

---

## Overview

This document defines standardized error handling patterns for Android applications integrating with QEMU Rafaelia. All error codes, messages, and recovery actions follow ISO quality management principles.

---

## Error Code Structure

### Format
```
QEMU_<CATEGORY>_<CODE>
```

### Categories

| Category | Description | Range |
|----------|-------------|-------|
| SYS | System-level errors | 001-099 |
| MEM | Memory-related errors | 100-199 |
| DISK | Disk/storage errors | 200-299 |
| NET | Network-related errors | 300-399 |
| PERM | Permission errors | 400-499 |
| CFG | Configuration errors | 500-599 |
| PROC | Process/execution errors | 600-699 |
| IO | I/O operations errors | 700-799 |

---

## System Errors (SYS)

### QEMU_SYS_001: QEMU Binary Not Found

**Technical Cause**: The QEMU executable is not available or accessible.

**User Message**:
> The virtualization engine is not properly installed. Please reinstall the application or contact support.

**Recovery Actions**:
1. Reinstall application
2. Clear app data and restart
3. Contact support

**Code Pattern**:
```kotlin
class QemuBinaryNotFoundError : QemuError(
    code = "QEMU_SYS_001",
    severity = Severity.CRITICAL,
    userMessage = R.string.error_qemu_binary_not_found,
    technicalMessage = "QEMU binary not found at expected path",
    recoveryActions = listOf(
        RecoveryAction.REINSTALL_APP,
        RecoveryAction.CLEAR_APP_DATA,
        RecoveryAction.CONTACT_SUPPORT
    )
)
```

### QEMU_SYS_002: Unsupported Architecture

**Technical Cause**: Device CPU architecture not supported.

**User Message**:
> Your device's processor architecture is not supported for virtualization. This app requires ARM64 (aarch64) devices.

**Recovery Actions**:
1. Check device specifications
2. Use alternative device

### QEMU_SYS_003: Insufficient System Resources

**Technical Cause**: System cannot allocate required resources.

**User Message**:
> Your device does not have enough available resources. Please close other applications and try again.

**Recovery Actions**:
1. Close other applications
2. Restart device
3. Reduce VM resource allocation

---

## Memory Errors (MEM)

### QEMU_MEM_100: Out of Memory

**Technical Cause**: Cannot allocate requested RAM for VM.

**User Message**:
> Not enough memory available to start this virtual machine.
>
> Requested: {requested_mb} MB  
> Available: {available_mb} MB

**Recovery Actions**:
1. Reduce VM memory allocation
2. Close other applications
3. Restart device

**Implementation**:
```kotlin
data class OutOfMemoryError(
    val requestedMb: Long,
    val availableMb: Long
) : QemuError(
    code = "QEMU_MEM_100",
    severity = Severity.HIGH,
    userMessage = R.string.error_out_of_memory,
    userMessageArgs = arrayOf(requestedMb, availableMb),
    technicalMessage = "malloc() failed: Cannot allocate $requestedMb MB, only $availableMb MB available",
    recoveryActions = listOf(
        RecoveryAction.REDUCE_VM_MEMORY,
        RecoveryAction.CLOSE_OTHER_APPS,
        RecoveryAction.RESTART_DEVICE
    )
)
```

### QEMU_MEM_101: Memory Allocation Failed

**Technical Cause**: Specific memory allocation operation failed.

**User Message**:
> A memory allocation error occurred. Please reduce the virtual machine's memory settings or restart your device.

### QEMU_MEM_102: Memory Limit Exceeded

**Technical Cause**: VM memory exceeds device capabilities.

**User Message**:
> The requested memory ({requested_mb} MB) exceeds your device's safe limit ({max_mb} MB). Please reduce the memory allocation.

---

## Disk/Storage Errors (DISK)

### QEMU_DISK_200: Disk Image Not Found

**Technical Cause**: Virtual disk file does not exist at specified path.

**User Message**:
> The virtual disk file could not be found. It may have been moved or deleted.
>
> Expected location: {path}

**Recovery Actions**:
1. Select new disk location
2. Restore from backup
3. Create new disk

### QEMU_DISK_201: Disk Image Corrupted

**Technical Cause**: QCOW2 header or data is corrupted.

**User Message**:
> The virtual disk file is corrupted and cannot be opened. 
>
> ⚠️ Data recovery may not be possible.

**Recovery Actions**:
1. Attempt repair (if possible)
2. Restore from backup
3. Create new disk (data loss)

**Implementation**:
```kotlin
class DiskCorruptedError(
    val diskPath: String,
    val corruptionType: CorruptionType
) : QemuError(
    code = "QEMU_DISK_201",
    severity = Severity.CRITICAL,
    userMessage = R.string.error_disk_corrupted,
    technicalMessage = "QCOW2 validation failed: ${corruptionType.description} at $diskPath",
    recoveryActions = when (corruptionType) {
        CorruptionType.HEADER -> listOf(RecoveryAction.ATTEMPT_REPAIR, RecoveryAction.RESTORE_BACKUP)
        CorruptionType.DATA -> listOf(RecoveryAction.RESTORE_BACKUP, RecoveryAction.CREATE_NEW_DISK)
        CorruptionType.UNKNOWN -> listOf(RecoveryAction.CONTACT_SUPPORT)
    }
)

enum class CorruptionType(val description: String) {
    HEADER("Invalid QCOW2 header"),
    DATA("Data block checksum mismatch"),
    UNKNOWN("Unknown corruption type")
}
```

### QEMU_DISK_202: Insufficient Disk Space

**Technical Cause**: Not enough storage to create/expand disk image.

**User Message**:
> Not enough storage space available.
>
> Required: {required_gb} GB  
> Available: {available_gb} GB

**Recovery Actions**:
1. Free up storage space
2. Use external storage
3. Reduce disk size

### QEMU_DISK_203: Disk Image Locked

**Technical Cause**: Disk file is locked by another process.

**User Message**:
> This virtual disk is currently in use by another process. Please ensure no other instance is running.

### QEMU_DISK_204: Unsupported Disk Format

**Technical Cause**: Disk format not recognized or unsupported.

**User Message**:
> The disk format is not supported. Supported formats: QCOW2, RAW

---

## Permission Errors (PERM)

### QEMU_PERM_400: Storage Permission Denied

**Technical Cause**: App lacks storage access permission.

**User Message**:
> Storage access permission is required to manage virtual machine files.

**Recovery Actions**:
1. Grant permission in settings
2. Select different storage location

**Implementation**:
```kotlin
class StoragePermissionDeniedError(
    val permissionType: StoragePermissionType
) : QemuError(
    code = "QEMU_PERM_400",
    severity = Severity.HIGH,
    userMessage = R.string.error_storage_permission_denied,
    technicalMessage = "Storage permission denied: ${permissionType.name}",
    recoveryActions = listOf(
        RecoveryAction.OPEN_SETTINGS,
        RecoveryAction.SELECT_DIFFERENT_LOCATION
    ),
    settingsIntent = permissionType.settingsIntent
)

// Required imports:
// import android.content.Intent
// import android.provider.Settings

enum class StoragePermissionType(val settingsIntent: Intent) {
    READ_EXTERNAL(Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS)),
    WRITE_EXTERNAL(Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS)),
    MANAGE_EXTERNAL(Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION))
}
```

### QEMU_PERM_401: File Access Denied

**Technical Cause**: Cannot read/write specific file.

**User Message**:
> Cannot access the selected file. The file may be protected or in a restricted location.

### QEMU_PERM_402: SAF URI Expired

**Technical Cause**: Storage Access Framework URI permission has expired.

**User Message**:
> Access to the selected file has expired. Please select the file again.

**Recovery Actions**:
1. Reselect file
2. Grant persistent permission

---

## Configuration Errors (CFG)

### QEMU_CFG_500: Invalid Configuration

**Technical Cause**: VM configuration contains invalid values.

**User Message**:
> The virtual machine configuration is invalid.
>
> Issue: {specific_issue}

**Implementation**:
```kotlin
class InvalidConfigurationError(
    val field: String,
    val currentValue: Any?,
    val validRange: String
) : QemuError(
    code = "QEMU_CFG_500",
    severity = Severity.MEDIUM,
    userMessage = R.string.error_invalid_configuration,
    userMessageArgs = arrayOf(field, currentValue, validRange),
    technicalMessage = "Invalid configuration: $field = $currentValue (valid: $validRange)",
    recoveryActions = listOf(RecoveryAction.FIX_CONFIGURATION)
)
```

### QEMU_CFG_501: Missing Required Field

**Technical Cause**: Required configuration field is missing.

**User Message**:
> The virtual machine configuration is incomplete. Missing: {field_name}

### QEMU_CFG_502: Incompatible Options

**Technical Cause**: Conflicting configuration options selected.

**User Message**:
> The selected options are incompatible: {option_a} cannot be used with {option_b}

### QEMU_CFG_503: Configuration File Corrupted

**Technical Cause**: VM configuration file is corrupted.

**User Message**:
> The configuration file is corrupted. Default settings will be applied.

---

## Process Errors (PROC)

### QEMU_PROC_600: Process Start Failed

**Technical Cause**: Failed to start QEMU process.

**User Message**:
> Failed to start the virtual machine. Please check the configuration and try again.

**Implementation**:
```kotlin
class ProcessStartFailedError(
    val exitCode: Int?,
    val stderr: String?
) : QemuError(
    code = "QEMU_PROC_600",
    severity = Severity.HIGH,
    userMessage = R.string.error_process_start_failed,
    technicalMessage = "QEMU process failed to start. Exit code: $exitCode, stderr: $stderr",
    recoveryActions = listOf(
        RecoveryAction.CHECK_CONFIGURATION,
        RecoveryAction.RETRY,
        RecoveryAction.VIEW_LOGS
    )
)
```

### QEMU_PROC_601: Process Crashed

**Technical Cause**: QEMU process terminated unexpectedly.

**User Message**:
> The virtual machine crashed unexpectedly. Your data may be affected.

**Recovery Actions**:
1. View crash logs
2. Check VM configuration
3. Report bug

### QEMU_PROC_602: Process Timeout

**Technical Cause**: Operation timed out waiting for QEMU response.

**User Message**:
> The operation timed out. The virtual machine may be unresponsive.

**Recovery Actions**:
1. Force stop VM
2. Wait longer
3. Restart

### QEMU_PROC_603: Signal Delivery Failed

**Technical Cause**: Failed to send signal to QEMU process.

**User Message**:
> Failed to communicate with the virtual machine. Please try force stopping.

---

## I/O Errors (IO)

### QEMU_IO_700: ISO Read Error

**Technical Cause**: Cannot read from ISO file.

**User Message**:
> Cannot read the installation image. The file may be corrupted or inaccessible.

### QEMU_IO_701: Network Socket Error

**Technical Cause**: Failed to create/bind network socket.

**User Message**:
> Failed to set up network for the virtual machine. Port may be in use.

### QEMU_IO_702: Display Connection Failed

**Technical Cause**: Failed to connect to QEMU display output.

**User Message**:
> Cannot connect to virtual machine display. Please restart the VM.

---

## Error Dialog Implementation

### Standard Error Dialog

```kotlin
@Composable
fun QemuErrorDialog(
    error: QemuError,
    onDismiss: () -> Unit,
    onAction: (RecoveryAction) -> Unit
) {
    var showTechnicalDetails by remember { mutableStateOf(false) }
    
    AlertDialog(
        onDismissRequest = onDismiss,
        icon = {
            Icon(
                imageVector = error.severity.icon,
                contentDescription = null,
                tint = error.severity.color
            )
        },
        title = {
            Text(stringResource(error.titleRes))
        },
        text = {
            Column {
                // User message
                Text(
                    text = stringResource(error.userMessage, *error.userMessageArgs),
                    style = MaterialTheme.typography.bodyMedium
                )
                
                Spacer(modifier = Modifier.height(16.dp))
                
                // Recovery actions
                if (error.recoveryActions.isNotEmpty()) {
                    Text(
                        text = stringResource(R.string.what_you_can_do),
                        style = MaterialTheme.typography.labelMedium,
                        fontWeight = FontWeight.Bold
                    )
                    Spacer(modifier = Modifier.height(8.dp))
                    error.recoveryActions.forEachIndexed { index, action ->
                        Text("${index + 1}. ${stringResource(action.labelRes)}")
                    }
                }
                
                // Technical details (expandable)
                Spacer(modifier = Modifier.height(16.dp))
                TextButton(
                    onClick = { showTechnicalDetails = !showTechnicalDetails }
                ) {
                    Icon(
                        imageVector = if (showTechnicalDetails) 
                            Icons.Default.ExpandLess else Icons.Default.ExpandMore,
                        contentDescription = null
                    )
                    Text(stringResource(R.string.technical_details))
                }
                
                AnimatedVisibility(visible = showTechnicalDetails) {
                    Column(
                        modifier = Modifier
                            .background(
                                MaterialTheme.colorScheme.surfaceVariant,
                                RoundedCornerShape(4.dp)
                            )
                            .padding(8.dp)
                    ) {
                        SelectionContainer {
                            Text(
                                text = buildString {
                                    appendLine("Error Code: ${error.code}")
                                    appendLine("Message: ${error.technicalMessage}")
                                    appendLine("Time: ${Instant.now()}")
                                },
                                style = MaterialTheme.typography.bodySmall,
                                fontFamily = FontFamily.Monospace
                            )
                        }
                        Row(
                            modifier = Modifier.fillMaxWidth(),
                            horizontalArrangement = Arrangement.End
                        ) {
                            TextButton(
                                onClick = { 
                                    // Copy to clipboard
                                }
                            ) {
                                Text(stringResource(R.string.copy))
                            }
                        }
                    }
                }
            }
        },
        confirmButton = {
            if (error.primaryAction != null) {
                Button(onClick = { onAction(error.primaryAction) }) {
                    Text(stringResource(error.primaryAction.labelRes))
                }
            } else {
                TextButton(onClick = onDismiss) {
                    Text(stringResource(R.string.ok))
                }
            }
        },
        dismissButton = {
            if (error.recoveryActions.size > 1) {
                TextButton(onClick = onDismiss) {
                    Text(stringResource(R.string.dismiss))
                }
            }
        }
    )
}
```

### Severity Levels

```kotlin
enum class Severity(
    val icon: ImageVector,
    val color: Color,
    val titleRes: Int
) {
    LOW(Icons.Outlined.Info, Color(0xFF1976D2), R.string.error_title_info),
    MEDIUM(Icons.Outlined.Warning, Color(0xFFE65100), R.string.error_title_warning),
    HIGH(Icons.Filled.Error, Color(0xFFB00020), R.string.error_title_error),
    CRITICAL(Icons.Filled.Error, Color(0xFF7F0000), R.string.error_title_critical)
}
```

---

## Recovery Action Definitions

```kotlin
enum class RecoveryAction(
    val labelRes: Int,
    val descriptionRes: Int,
    val automaticAction: Boolean = false
) {
    RETRY(
        labelRes = R.string.action_retry,
        descriptionRes = R.string.action_retry_desc,
        automaticAction = true
    ),
    REDUCE_VM_MEMORY(
        labelRes = R.string.action_reduce_memory,
        descriptionRes = R.string.action_reduce_memory_desc
    ),
    CLOSE_OTHER_APPS(
        labelRes = R.string.action_close_apps,
        descriptionRes = R.string.action_close_apps_desc
    ),
    RESTART_DEVICE(
        labelRes = R.string.action_restart_device,
        descriptionRes = R.string.action_restart_device_desc
    ),
    OPEN_SETTINGS(
        labelRes = R.string.action_open_settings,
        descriptionRes = R.string.action_open_settings_desc,
        automaticAction = true
    ),
    SELECT_DIFFERENT_LOCATION(
        labelRes = R.string.action_select_location,
        descriptionRes = R.string.action_select_location_desc,
        automaticAction = true
    ),
    CHECK_CONFIGURATION(
        labelRes = R.string.action_check_config,
        descriptionRes = R.string.action_check_config_desc,
        automaticAction = true
    ),
    FIX_CONFIGURATION(
        labelRes = R.string.action_fix_config,
        descriptionRes = R.string.action_fix_config_desc,
        automaticAction = true
    ),
    ATTEMPT_REPAIR(
        labelRes = R.string.action_attempt_repair,
        descriptionRes = R.string.action_attempt_repair_desc,
        automaticAction = true
    ),
    RESTORE_BACKUP(
        labelRes = R.string.action_restore_backup,
        descriptionRes = R.string.action_restore_backup_desc
    ),
    CREATE_NEW_DISK(
        labelRes = R.string.action_create_new_disk,
        descriptionRes = R.string.action_create_new_disk_desc
    ),
    VIEW_LOGS(
        labelRes = R.string.action_view_logs,
        descriptionRes = R.string.action_view_logs_desc,
        automaticAction = true
    ),
    REINSTALL_APP(
        labelRes = R.string.action_reinstall,
        descriptionRes = R.string.action_reinstall_desc
    ),
    CLEAR_APP_DATA(
        labelRes = R.string.action_clear_data,
        descriptionRes = R.string.action_clear_data_desc
    ),
    CONTACT_SUPPORT(
        labelRes = R.string.action_contact_support,
        descriptionRes = R.string.action_contact_support_desc,
        automaticAction = true
    ),
    FORCE_STOP(
        labelRes = R.string.action_force_stop,
        descriptionRes = R.string.action_force_stop_desc,
        automaticAction = true
    )
}
```

---

## Logging Requirements

All errors must be logged with:
1. Error code
2. Timestamp
3. Device information
4. VM configuration (sanitized)
5. Stack trace
6. User action before error

```kotlin
data class ErrorLogEntry(
    val errorCode: String,
    val timestamp: Instant,
    val deviceInfo: DeviceInfo,
    val vmConfig: SanitizedVMConfig?,
    val stackTrace: String,
    val userAction: String?,
    val additionalContext: Map<String, String>
)

object ErrorLogger {
    fun log(error: QemuError, context: ErrorContext) {
        val entry = ErrorLogEntry(
            errorCode = error.code,
            timestamp = Instant.now(),
            deviceInfo = DeviceInfo.current(),
            vmConfig = context.vmConfig?.sanitized(),
            stackTrace = error.stackTrace ?: "",
            userAction = context.lastUserAction,
            additionalContext = context.additionalData
        )
        
        // Log locally
        LocalErrorDatabase.insert(entry)
        
        // Report if user consented
        if (PrivacySettings.crashReportingEnabled) {
            CrashReporter.report(entry)
        }
    }
}
```

---

## String Resources

```xml
<!-- res/values/strings_errors.xml -->
<resources>
    <!-- Error Titles -->
    <string name="error_title_info">Information</string>
    <string name="error_title_warning">Warning</string>
    <string name="error_title_error">Error</string>
    <string name="error_title_critical">Critical Error</string>
    
    <!-- Error Messages -->
    <string name="error_qemu_binary_not_found">The virtualization engine is not properly installed. Please reinstall the application or contact support.</string>
    <string name="error_out_of_memory">Not enough memory available to start this virtual machine.\n\nRequested: %1$d MB\nAvailable: %2$d MB</string>
    <string name="error_storage_permission_denied">Storage access permission is required to manage virtual machine files.</string>
    <string name="error_disk_corrupted">The virtual disk file is corrupted and cannot be opened.\n\n⚠️ Data recovery may not be possible.</string>
    <string name="error_invalid_configuration">The virtual machine configuration is invalid.\n\nIssue: %1$s = %2$s (valid: %3$s)</string>
    <string name="error_process_start_failed">Failed to start the virtual machine. Please check the configuration and try again.</string>
    
    <!-- Recovery Actions -->
    <string name="action_retry">Try Again</string>
    <string name="action_retry_desc">Attempt the operation again</string>
    <string name="action_reduce_memory">Reduce VM Memory</string>
    <string name="action_reduce_memory_desc">Lower the memory allocation for this VM</string>
    <string name="action_close_apps">Close Other Apps</string>
    <string name="action_close_apps_desc">Free up memory by closing background apps</string>
    <string name="action_open_settings">Open Settings</string>
    <string name="action_open_settings_desc">Open app settings to grant permissions</string>
    <string name="action_contact_support">Contact Support</string>
    <string name="action_contact_support_desc">Get help from our support team</string>
    
    <!-- UI Elements -->
    <string name="what_you_can_do">What you can do:</string>
    <string name="technical_details">Technical Details</string>
    <string name="copy">Copy</string>
    <string name="ok">OK</string>
    <string name="dismiss">Dismiss</string>
</resources>
```

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0.0 | 2026-01-10 | Initial release |

---

**FIAT LUX ΣΩΔΦBITRAF** 💎
