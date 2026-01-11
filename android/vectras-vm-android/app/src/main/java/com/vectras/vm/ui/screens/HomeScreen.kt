/**
 * HomeScreen - Main home screen with navigation cards
 * ISO 9001/8000 Compliant UI/UX
 * 
 * Features:
 * - Quick access cards: Create VM, Import, Library, Logs, Config
 * - Fast navigation flow
 * - Accessibility compliant (touch targets >= 48dp, content descriptions)
 */
package com.vectras.vm.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.contentDescription
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import com.vectras.vm.R

/**
 * Home screen card data.
 */
data class HomeCard(
    val title: String,
    val description: String,
    val icon: ImageVector,
    val contentDescription: String,
    val onClick: () -> Unit
)

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun HomeScreen(
    onNavigateToCreateVM: () -> Unit,
    onNavigateToLibrary: () -> Unit,
    onNavigateToLogs: () -> Unit,
    onNavigateToSettings: () -> Unit,
    onNavigateToVM: (String) -> Unit,
    modifier: Modifier = Modifier
) {
    val cards = remember {
        listOf(
            HomeCard(
                title = "Create VM",
                description = "Create a new virtual machine with the setup wizard",
                icon = Icons.Default.Add,
                contentDescription = "Create a new virtual machine",
                onClick = onNavigateToCreateVM
            ),
            HomeCard(
                title = "Import",
                description = "Import an existing VM configuration or disk image",
                icon = Icons.Default.FileOpen,
                contentDescription = "Import existing virtual machine",
                onClick = { /* TODO: Implement import */ }
            ),
            HomeCard(
                title = "Library",
                description = "View and manage your virtual machines",
                icon = Icons.Default.Storage,
                contentDescription = "Open virtual machine library",
                onClick = onNavigateToLibrary
            ),
            HomeCard(
                title = "Logs",
                description = "View system and VM logs with filtering options",
                icon = Icons.Default.Description,
                contentDescription = "Open log viewer",
                onClick = onNavigateToLogs
            ),
            HomeCard(
                title = "Settings",
                description = "Configure application settings and preferences",
                icon = Icons.Default.Settings,
                contentDescription = "Open settings",
                onClick = onNavigateToSettings
            )
        )
    }

    Scaffold(
        topBar = {
            LargeTopAppBar(
                title = {
                    Text(
                        text = "Vectras VM",
                        style = MaterialTheme.typography.headlineLarge
                    )
                },
                colors = TopAppBarDefaults.largeTopAppBarColors(
                    containerColor = MaterialTheme.colorScheme.primaryContainer,
                    titleContentColor = MaterialTheme.colorScheme.onPrimaryContainer
                )
            )
        },
        modifier = modifier
    ) { innerPadding ->
        LazyColumn(
            contentPadding = PaddingValues(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp),
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
        ) {
            items(cards) { card ->
                HomeCardItem(card = card)
            }
        }
    }
}

@Composable
fun HomeCardItem(
    card: HomeCard,
    modifier: Modifier = Modifier
) {
    Card(
        onClick = card.onClick,
        modifier = modifier
            .fillMaxWidth()
            .heightIn(min = 96.dp) // Ensure adequate touch target
            .semantics { contentDescription = card.contentDescription },
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surface
        ),
        elevation = CardDefaults.cardElevation(defaultElevation = 2.dp)
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalAlignment = Alignment.CenterVertically,
            horizontalArrangement = Arrangement.spacedBy(16.dp)
        ) {
            // Icon with adequate touch target (48dp)
            Surface(
                modifier = Modifier.size(56.dp),
                shape = MaterialTheme.shapes.medium,
                color = MaterialTheme.colorScheme.primaryContainer
            ) {
                Icon(
                    imageVector = card.icon,
                    contentDescription = null, // Handled by parent semantics
                    modifier = Modifier
                        .padding(12.dp)
                        .size(32.dp),
                    tint = MaterialTheme.colorScheme.onPrimaryContainer
                )
            }
            
            Column(
                modifier = Modifier.weight(1f),
                verticalArrangement = Arrangement.spacedBy(4.dp)
            ) {
                Text(
                    text = card.title,
                    style = MaterialTheme.typography.titleMedium,
                    color = MaterialTheme.colorScheme.onSurface
                )
                Text(
                    text = card.description,
                    style = MaterialTheme.typography.bodyMedium,
                    color = MaterialTheme.colorScheme.onSurfaceVariant
                )
            }
            
            Icon(
                imageVector = Icons.Default.ChevronRight,
                contentDescription = null,
                tint = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
    }
}
