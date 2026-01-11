/**
 * VectrasVMApp - Main application composable with navigation
 * ISO 9001/8000 Compliant UI/UX
 */
package com.vectras.vm.ui

import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import androidx.navigation.navArgument
import com.vectras.vm.ui.screens.HomeScreen
import com.vectras.vm.ui.screens.LogsScreen
import com.vectras.vm.ui.screens.SettingsScreen
import com.vectras.vm.ui.screens.VMExecutionScreen
import com.vectras.vm.ui.screens.VMLibraryScreen
import com.vectras.vm.ui.wizard.VMWizardScreen

/**
 * Navigation routes for the application.
 */
object Routes {
    const val HOME = "home"
    const val CREATE_VM = "create_vm"
    const val LIBRARY = "library"
    const val LOGS = "logs"
    const val SETTINGS = "settings"
    const val VM_EXECUTION = "vm_execution/{vmId}"
    
    fun vmExecution(vmId: String) = "vm_execution/$vmId"
}

@Composable
fun VectrasVMApp() {
    val navController = rememberNavController()
    
    Scaffold { innerPadding ->
        NavHost(
            navController = navController,
            startDestination = Routes.HOME,
            modifier = Modifier.padding(innerPadding)
        ) {
            composable(Routes.HOME) {
                HomeScreen(
                    onNavigateToCreateVM = { navController.navigate(Routes.CREATE_VM) },
                    onNavigateToLibrary = { navController.navigate(Routes.LIBRARY) },
                    onNavigateToLogs = { navController.navigate(Routes.LOGS) },
                    onNavigateToSettings = { navController.navigate(Routes.SETTINGS) },
                    onNavigateToVM = { vmId -> navController.navigate(Routes.vmExecution(vmId)) }
                )
            }
            
            composable(Routes.CREATE_VM) {
                VMWizardScreen(
                    onNavigateBack = { navController.popBackStack() },
                    onVMCreated = { vmId ->
                        navController.navigate(Routes.vmExecution(vmId)) {
                            popUpTo(Routes.HOME)
                        }
                    }
                )
            }
            
            composable(Routes.LIBRARY) {
                VMLibraryScreen(
                    onNavigateBack = { navController.popBackStack() },
                    onNavigateToVM = { vmId -> navController.navigate(Routes.vmExecution(vmId)) }
                )
            }
            
            composable(Routes.LOGS) {
                LogsScreen(
                    onNavigateBack = { navController.popBackStack() }
                )
            }
            
            composable(Routes.SETTINGS) {
                SettingsScreen(
                    onNavigateBack = { navController.popBackStack() }
                )
            }
            
            composable(
                route = Routes.VM_EXECUTION,
                arguments = listOf(navArgument("vmId") { type = NavType.StringType })
            ) { backStackEntry ->
                val vmId = backStackEntry.arguments?.getString("vmId") ?: ""
                VMExecutionScreen(
                    vmId = vmId,
                    onNavigateBack = { navController.popBackStack() }
                )
            }
        }
    }
}
