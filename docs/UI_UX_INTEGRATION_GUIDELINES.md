# UI/UX Integration Guidelines for Android Frontend
## ISO 9001:2015 & ISO 8000 Compliant Design Standards

**Version**: 1.0.0  
**Date**: 2026-01-10  
**Status**: Active  
**Standard Compliance**: ISO 9001:2015 (Quality Management), ISO 8000 (Data Quality)

---

## Table of Contents

1. [Overview](#overview)
2. [Quality Management Principles (ISO 9001)](#quality-management-principles-iso-9001)
3. [Data Quality Principles (ISO 8000)](#data-quality-principles-iso-8000)
4. [Navigation Clarity](#navigation-clarity)
5. [Permission & Storage UX (Android 13-15)](#permission--storage-ux-android-13-15)
6. [Long Operation Feedback](#long-operation-feedback)
7. [Accessibility Standards](#accessibility-standards)
8. [Error Handling UX](#error-handling-ux)
9. [Implementation Patterns](#implementation-patterns)
10. [Quality Assurance Checklist](#quality-assurance-checklist)

---

## Overview

This document provides comprehensive UI/UX guidelines for Android applications integrating with QEMU Rafaelia. These guidelines ensure professional, stable, and user-trustworthy virtualization experiences while maintaining compliance with international quality standards.

### Design Philosophy

The application should feel:
- **Predictable**: Users always know what to expect
- **Stable**: Operations complete reliably without crashes
- **Honest**: Clear communication about limitations and errors
- **Technically Respectful**: Designed for power users who understand virtualization

---

## Quality Management Principles (ISO 9001)

> **Note**: These principles are based on the ISO 9001:2015 quality management principles described in Clause 0.2 of the standard's introduction.

### Principle 1: Customer Focus
- Understand user needs for VM creation, configuration, and management
- Provide clear feedback on all operations
- Document all limitations upfront

### Principle 2: Leadership
- Establish clear UI hierarchies and navigation flows
- Define consistent design patterns across all screens

### Principle 3: Engagement of People
- Design for both novice and expert users
- Provide contextual help and documentation links

### Principle 4: Process Approach
- Define clear workflows for:
  - VM Creation (Step 1 → Step N)
  - VM Configuration
  - VM Execution
  - Error Recovery

### Principle 5: Improvement
- Collect user feedback on UX issues
- Track error occurrences and resolution rates
- Implement continuous improvement cycles

### Principle 6: Evidence-Based Decision Making
- Log all operations for debugging
- Track user journey analytics (with consent)
- Monitor crash rates and ANRs

### Principle 7: Relationship Management
- Clear communication with users about updates
- Transparent changelog for improvements

---

## Data Quality Principles (ISO 8000)

### Data Accuracy
- Validate all user inputs before processing
- Show clear validation errors with correction guidance
- Prevent data corruption through confirmation dialogs

### Data Completeness
- Required fields must be clearly marked
- Show progress indicators for multi-step forms
- Warn users about incomplete configurations

### Data Consistency
- Use standardized units (GB, MB, cores)
- Consistent date/time formats
- Uniform status indicators across all screens

### Data Timeliness
- Show real-time status updates for running VMs
- Timestamp all operations
- Display last updated time for cached data

### Data Validity
- Validate VM paths exist before operations
- Check file permissions before access
- Verify ISO checksums when possible

---

## Navigation Clarity

### Screen Flow Requirements

#### 1. VM Creation Wizard

```
┌─────────────────────────────────────────┐
│  Create Virtual Machine                 │
│  ─────────────────────────────────────  │
│  Step 1 of 4: Basic Configuration       │
│                                         │
│  [●] [○] [○] [○]   Step Indicators      │
│                                         │
│  VM Name: ________________________      │
│  Description: ____________________      │
│                                         │
│  ⓘ Name must be 3-50 characters,       │
│    alphanumeric and hyphens only        │
│                                         │
│  [Cancel]              [Next Step →]    │
└─────────────────────────────────────────┘
```

**Requirements**:
- Clear step indicators showing current position
- Contextual helper text for each input
- Descriptive button labels ("Next Step" not just "Next")
- Visible cancel option at all steps
- Progress persistence (back navigation retains data)

#### 2. Dangerous Action Identification

Actions must be classified and visually distinguished:

**Safe Actions** (Green/Default):
- View VM details
- Start VM (when stopped)
- View logs

**Moderate Actions** (Amber/Warning):
- Stop VM (may lose unsaved data)
- Modify VM configuration
- Import ISO file

**Dangerous Actions** (Red/Destructive):
- Delete VM
- Format virtual disk
- Reset VM state

**Implementation Pattern**:
```xml
<!-- Android XML - Destructive Action Button -->
<com.google.android.material.button.MaterialButton
    android:id="@+id/btn_delete_vm"
    style="@style/Widget.App.Button.Destructive"
    android:text="@string/delete_vm"
    android:contentDescription="@string/delete_vm_description"
    app:icon="@drawable/ic_delete_forever"
    app:iconGravity="textStart" />
```

#### 3. Screen Titles and Context

Every screen must have:
- Clear, descriptive title
- Breadcrumb or back navigation context
- Current operation state (if applicable)

```
┌─────────────────────────────────────────┐
│  ← VM Configuration                     │
│     my-ubuntu-vm                        │
│  ─────────────────────────────────────  │
│                                         │
│  Last modified: 2026-01-10 23:30        │
│  Status: Stopped                        │
│                                         │
└─────────────────────────────────────────┘
```

---

## Permission & Storage UX (Android 13-15)

### Storage Permission Denied Flow

```
┌─────────────────────────────────────────┐
│  ⚠️ Storage Access Required             │
│  ─────────────────────────────────────  │
│                                         │
│  This app needs access to storage to:   │
│  • Save virtual machine disk images     │
│  • Load ISO installation files          │
│  • Store VM configurations              │
│                                         │
│  Without storage access, you cannot     │
│  create or run virtual machines.        │
│                                         │
│  [Open Settings]    [Continue Limited]  │
│                                         │
│  ⓘ Tap "Open Settings" → Permissions   │
│    → Storage → Allow                    │
└─────────────────────────────────────────┘
```

### SAF Picker Guidance

When using Storage Access Framework (SAF):

```
┌─────────────────────────────────────────┐
│  Select ISO File                        │
│  ─────────────────────────────────────  │
│                                         │
│  ⓘ Please select the installation ISO  │
│    file for your virtual machine.       │
│                                         │
│  Supported formats: .iso, .img          │
│  Maximum size: Depends on storage       │
│                                         │
│  Common locations:                      │
│  • Downloads folder                     │
│  • Internal storage root                │
│  • External SD card                     │
│                                         │
│  [Browse Files]                         │
│                                         │
│  ──────────────────────────────────     │
│  Need help finding files?               │
│  [View Guide] ↗                         │
└─────────────────────────────────────────┘
```

### Invalid Path Handling

```
┌─────────────────────────────────────────┐
│  ❌ File Access Error                   │
│  ─────────────────────────────────────  │
│                                         │
│  Cannot access the selected file:       │
│  /storage/emulated/0/ubuntu.iso         │
│                                         │
│  Possible causes:                       │
│  • File was moved or deleted            │
│  • Permission was revoked               │
│  • File is on disconnected storage      │
│                                         │
│  What you can do:                       │
│  1. Verify the file exists              │
│  2. Check app storage permissions       │
│  3. Select a different file             │
│                                         │
│  [Select Different File]  [Check Perms] │
│                                         │
│  [▼ Technical Details]                  │
│  Error: EACCES - Permission denied      │
│  Path: /storage/emulated/0/ubuntu.iso   │
│  Time: 2026-01-10T23:30:00Z             │
└─────────────────────────────────────────┘
```

### Android Version-Specific Handling

| Android Version | Storage Model | Recommendation |
|----------------|---------------|----------------|
| Android 10 (Q) | Scoped Storage | Use SAF for external files |
| Android 11 (R) | Scoped Storage | Request MANAGE_EXTERNAL_STORAGE only if essential |
| Android 12 (S) | Scoped Storage | Prefer MediaStore and SAF |
| Android 13+ | Photo Picker | Use SAF for documents, new photo picker for media |
| Android 14-15 | Stricter SAF | Always use persistent URI permissions |

---

## Long Operation Feedback

### Operation Types and Feedback Requirements

#### 1. Creating qcow2 Image

```
┌─────────────────────────────────────────┐
│  Creating Virtual Disk                  │
│  ─────────────────────────────────────  │
│                                         │
│  ████████████░░░░░░░░  60%              │
│                                         │
│  Creating: my-vm-disk.qcow2             │
│  Size: 20 GB                            │
│  Time elapsed: 0:45                     │
│  Estimated remaining: 0:30              │
│                                         │
│  ⓘ Do not close the app during this    │
│    operation to avoid data corruption.  │
│                                         │
│  [Cancel Operation]                     │
└─────────────────────────────────────────┘
```

#### 2. Starting VM

```
┌─────────────────────────────────────────┐
│  Starting Virtual Machine               │
│  ─────────────────────────────────────  │
│                                         │
│  ◉ ○ ○ ○   Initializing...              │
│                                         │
│  Current step: Loading disk image       │
│  VM: my-ubuntu-vm                       │
│                                         │
│  Steps:                                 │
│  ✓ Validating configuration             │
│  ✓ Allocating memory                    │
│  → Loading disk image                   │
│  ○ Starting QEMU process                │
│  ○ Connecting display                   │
│                                         │
│  [Cancel]                               │
└─────────────────────────────────────────┘
```

#### 3. Stopping VM

```
┌─────────────────────────────────────────┐
│  Stopping Virtual Machine               │
│  ─────────────────────────────────────  │
│                                         │
│  ⚠️ Unsaved work in the VM may be lost  │
│                                         │
│  Sending shutdown signal...             │
│  ████████████████░░░░  80%              │
│                                         │
│  If the VM does not respond:            │
│  [Force Stop] (may cause data loss)     │
│                                         │
│  [Wait]                   [Cancel]      │
└─────────────────────────────────────────┘
```

### Button State During Operations

```kotlin
// Kotlin implementation pattern
sealed class OperationState {
    object Idle : OperationState()
    data class InProgress(val message: String, val progress: Float?) : OperationState()
    data class Success(val message: String) : OperationState()
    data class Error(val error: AppError) : OperationState()
}

// View binding - complete state handling
when (state) {
    is OperationState.Idle -> {
        btnStart.isEnabled = true
        btnStart.text = getString(R.string.start_vm)
        progressBar.isVisible = false
        statusText.text = getString(R.string.status_ready)
    }
    is OperationState.InProgress -> {
        btnStart.isEnabled = false
        btnStart.text = getString(R.string.starting)
        progressBar.isVisible = true
        progressBar.progress = (state.progress?.times(100))?.toInt() ?: 0
        progressBar.isIndeterminate = state.progress == null
        statusText.text = state.message
    }
    is OperationState.Success -> {
        btnStart.isEnabled = true
        btnStart.text = getString(R.string.stop_vm)
        progressBar.isVisible = false
        statusText.text = state.message
        showSuccessSnackbar(state.message)
    }
    is OperationState.Error -> {
        btnStart.isEnabled = true
        btnStart.text = getString(R.string.start_vm)
        progressBar.isVisible = false
        statusText.text = getString(R.string.status_error)
        showErrorDialog(state.error)
    }
}
```

---

## Accessibility Standards

### Minimum Requirements (WCAG 2.1 AA)

#### 1. Content Descriptions

Every interactive element must have meaningful content descriptions:

```xml
<!-- ❌ Bad -->
<ImageButton
    android:id="@+id/btn_settings"
    android:src="@drawable/ic_gear" />

<!-- ✓ Good -->
<ImageButton
    android:id="@+id/btn_settings"
    android:src="@drawable/ic_gear"
    android:contentDescription="@string/settings_button_description" />
```

```xml
<!-- strings.xml -->
<string name="settings_button_description">Open virtual machine settings</string>
```

#### 2. Touch Target Sizes

Minimum touch target: 48dp × 48dp (Android recommendation)

```xml
<ImageButton
    android:id="@+id/btn_action"
    android:layout_width="48dp"
    android:layout_height="48dp"
    android:minWidth="48dp"
    android:minHeight="48dp"
    android:padding="12dp" />
```

#### 3. Color Contrast

Minimum contrast ratios:
- Normal text: 4.5:1
- Large text (18sp+ or 14sp+ bold): 3:1
- UI components and graphics: 3:1

```xml
<!-- colors.xml - ISO compliant palette -->
<color name="text_primary">#212121</color>      <!-- On white: 16.1:1 -->
<color name="text_secondary">#757575</color>    <!-- On white: 4.6:1 -->
<color name="error_red">#B00020</color>         <!-- On white: 7.0:1 -->
<color name="success_green">#1B5E20</color>     <!-- On white: 8.1:1 -->
<color name="warning_amber">#E65100</color>     <!-- On white: 4.6:1 -->
```

#### 4. Icon + Text Combinations

Avoid ambiguous icons without accompanying text:

```
❌ Bad:  [⚙️]  (What does this do?)
✓ Good: [⚙️ Settings]

❌ Bad:  [🔄]  (Refresh? Restart? Sync?)
✓ Good: [🔄 Restart VM]
```

#### 5. Screen Reader Announcements

```kotlin
// Announce important state changes
private fun announceToAccessibility(message: String) {
    binding.root.announceForAccessibility(message)
}

// Usage
when (vmState) {
    VMState.RUNNING -> announceToAccessibility(
        getString(R.string.vm_started_announcement, vmName)
    )
    VMState.STOPPED -> announceToAccessibility(
        getString(R.string.vm_stopped_announcement, vmName)
    )
}
```

---

## Error Handling UX

### Error Classification

| Error Type | User Impact | Required Response |
|-----------|-------------|-------------------|
| Recoverable | Can retry | Show retry option |
| Configuration | User can fix | Show specific guidance |
| Permission | User action needed | Guide to settings |
| System | App cannot fix | Explain and suggest alternatives |
| Critical | App must close | Save state, explain, offer bug report |

### Error Dialog Pattern

```
┌─────────────────────────────────────────┐
│  ❌ VM Start Failed                     │
│  ─────────────────────────────────────  │
│                                         │
│  The virtual machine could not start    │
│  due to insufficient memory.            │
│                                         │
│  Current configuration:                 │
│  • Requested RAM: 4 GB                  │
│  • Available RAM: 2.5 GB                │
│                                         │
│  Suggestions:                           │
│  1. Reduce VM memory to 2 GB            │
│  2. Close other applications            │
│  3. Restart your device                 │
│                                         │
│  [Adjust Settings]        [Try Again]   │
│                                         │
│  [▼ Technical Details]                  │
│  ─────────────────────────────────────  │
│  Error Code: QEMU_OOM_001               │
│  Message: Cannot allocate memory        │
│  Stack: QemuProcess.start():234         │
│         VMManager.startVM():89          │
│  Timestamp: 2026-01-10T23:30:00.000Z    │
│                                         │
│  [Copy to Clipboard]  [Send Bug Report] │
└─────────────────────────────────────────┘
```

### Implementation Pattern

```kotlin
sealed class AppError(
    val code: String,
    val userMessage: String,
    val technicalDetails: String,
    val recoveryActions: List<RecoveryAction>
) {
    class MemoryError(
        requested: Long,
        available: Long
    ) : AppError(
        code = "QEMU_OOM_001",
        userMessage = "Insufficient memory to start VM",
        technicalDetails = "Requested: ${requested}MB, Available: ${available}MB",
        recoveryActions = listOf(
            RecoveryAction.REDUCE_VM_MEMORY,
            RecoveryAction.CLOSE_OTHER_APPS,
            RecoveryAction.RESTART_DEVICE
        )
    )
    
    class StorageError(
        path: String,
        cause: Throwable
    ) : AppError(
        code = "STORAGE_ACCESS_001",
        userMessage = "Cannot access storage location",
        technicalDetails = "Path: $path\nCause: ${cause.message}",
        recoveryActions = listOf(
            RecoveryAction.CHECK_PERMISSIONS,
            RecoveryAction.SELECT_DIFFERENT_LOCATION
        )
    )
    
    // ... more error types
}

enum class RecoveryAction {
    RETRY,
    REDUCE_VM_MEMORY,
    CLOSE_OTHER_APPS,
    RESTART_DEVICE,
    CHECK_PERMISSIONS,
    SELECT_DIFFERENT_LOCATION,
    CONTACT_SUPPORT
}
```

### Never Do

1. **Never fail silently**
   ```kotlin
   // ❌ Bad
   try {
       startVM()
   } catch (e: Exception) {
       Log.e(TAG, "Failed to start VM", e)
       // User sees nothing!
   }
   
   // ✓ Good
   try {
       startVM()
   } catch (e: Exception) {
       Log.e(TAG, "Failed to start VM", e)
       showErrorDialog(AppError.fromException(e))
   }
   ```

2. **Never crash without explanation**
   ```kotlin
   // ✓ Use global exception handler
   Thread.setDefaultUncaughtExceptionHandler { _, throwable ->
       CrashReporter.logCrash(throwable)
       showCrashDialog(throwable)
   }
   ```

3. **Never show only Logcat errors to users**
   ```kotlin
   // ❌ Bad
   showToast("Error: ${exception.message}")
   
   // ✓ Good
   showErrorDialog(
       userMessage = getString(R.string.generic_error),
       technicalDetails = exception.stackTraceToString()
   )
   ```

---

## Implementation Patterns

### ViewModel State Management

```kotlin
data class VMScreenState(
    val vms: List<VM> = emptyList(),
    val selectedVM: VM? = null,
    val operationState: OperationState = OperationState.Idle,
    val errorState: ErrorState? = null
)

sealed class OperationState {
    object Idle : OperationState()
    data class Loading(val message: String, val progress: Float? = null) : OperationState()
    data class Success(val message: String) : OperationState()
}

data class ErrorState(
    val error: AppError,
    val dismissed: Boolean = false
)
```

### Loading State Composable (Jetpack Compose)

```kotlin
@Composable
fun OperationProgressDialog(
    state: OperationState.Loading,
    onCancel: () -> Unit
) {
    AlertDialog(
        onDismissRequest = { /* Prevent accidental dismissal */ },
        title = {
            Text(
                text = stringResource(R.string.operation_in_progress),
                style = MaterialTheme.typography.titleLarge
            )
        },
        text = {
            Column {
                Text(state.message)
                Spacer(modifier = Modifier.height(16.dp))
                if (state.progress != null) {
                    LinearProgressIndicator(
                        progress = state.progress,
                        modifier = Modifier.fillMaxWidth()
                    )
                    Text(
                        text = "${(state.progress * 100).toInt()}%",
                        style = MaterialTheme.typography.bodySmall,
                        modifier = Modifier.align(Alignment.End)
                    )
                } else {
                    LinearProgressIndicator(modifier = Modifier.fillMaxWidth())
                }
            }
        },
        confirmButton = {
            TextButton(onClick = onCancel) {
                Text(stringResource(R.string.cancel))
            }
        }
    )
}
```

---

## Quality Assurance Checklist

### Pre-Release Checklist (ISO 9001 Compliance)

#### Navigation & Flow
- [ ] All screens have clear, descriptive titles
- [ ] Step indicators present for multi-step flows
- [ ] Back navigation works correctly at all steps
- [ ] Dangerous actions are visually distinguished
- [ ] Confirmation dialogs for destructive operations

#### Permissions & Storage
- [ ] Clear explanation when permissions are needed
- [ ] Graceful handling of permission denial
- [ ] SAF picker has contextual guidance
- [ ] Invalid path errors show actionable steps

#### Feedback & Progress
- [ ] Progress indicators for operations > 2 seconds
- [ ] Buttons disabled during operations
- [ ] Cancelable operations where safe
- [ ] Clear success/failure feedback

#### Accessibility
- [ ] All images have content descriptions
- [ ] Touch targets ≥ 48dp
- [ ] Color contrast meets WCAG 2.1 AA
- [ ] Screen reader testing completed
- [ ] Icons have accompanying text

#### Error Handling
- [ ] No silent failures
- [ ] All errors show user-friendly messages
- [ ] Technical details available but hidden by default
- [ ] Recovery actions provided where possible
- [ ] Crash handler provides user feedback

#### Data Quality (ISO 8000)
- [ ] Input validation with clear error messages
- [ ] Required fields marked
- [ ] Consistent units and formats
- [ ] Timestamps on cached data

### Testing Requirements

| Test Type | Frequency | Responsible |
|-----------|-----------|-------------|
| UI/UX review | Each release | Design team |
| Accessibility audit | Quarterly | QA team |
| Error flow testing | Each release | QA team |
| Permission flow testing | On Android updates | Dev team |
| Performance testing | Each major release | Dev team |

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0.0 | 2026-01-10 | Initial release |

---

## References

- [ISO 9001:2015](https://www.iso.org/standard/62085.html) - Quality Management Systems
- [ISO 8000](https://www.iso.org/standard/60805.html) - Data Quality
- [WCAG 2.1](https://www.w3.org/WAI/WCAG21/quickref/) - Web Content Accessibility Guidelines
- [Android Accessibility](https://developer.android.com/guide/topics/ui/accessibility)
- [Material Design Guidelines](https://material.io/design)
- [Android Storage Best Practices](https://developer.android.com/training/data-storage)

---

**FIAT LUX ΣΩΔΦBITRAF** 💎
