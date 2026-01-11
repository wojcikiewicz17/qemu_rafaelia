/**
 * VMService - Foreground service for VM execution
 * ISO 9001/8000 Compliant
 * 
 * Runs VMs in the background with proper lifecycle management.
 */
package com.vectras.vm.core

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Intent
import android.os.Build
import android.os.IBinder
import androidx.core.app.NotificationCompat

class VMService : Service() {
    
    companion object {
        const val CHANNEL_ID = "vm_execution_channel"
        const val NOTIFICATION_ID = 1
        const val ACTION_START_VM = "com.vectras.vm.START_VM"
        const val ACTION_STOP_VM = "com.vectras.vm.STOP_VM"
        const val EXTRA_VM_ID = "vm_id"
    }
    
    override fun onCreate() {
        super.onCreate()
        createNotificationChannel()
    }
    
    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        val vmId = intent?.getStringExtra(EXTRA_VM_ID)
        
        when (intent?.action) {
            ACTION_START_VM -> {
                startForeground(NOTIFICATION_ID, createNotification(vmId ?: "Unknown"))
                // TODO: Start QEMU process here
            }
            ACTION_STOP_VM -> {
                // TODO: Stop QEMU process here
                stopForeground(STOP_FOREGROUND_REMOVE)
                stopSelf()
            }
        }
        
        return START_STICKY
    }
    
    override fun onBind(intent: Intent?): IBinder? = null
    
    private fun createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                CHANNEL_ID,
                "VM Execution",
                NotificationManager.IMPORTANCE_LOW
            ).apply {
                description = "Shows when a virtual machine is running"
            }
            
            val notificationManager = getSystemService(NotificationManager::class.java)
            notificationManager.createNotificationChannel(channel)
        }
    }
    
    private fun createNotification(vmName: String): Notification {
        return NotificationCompat.Builder(this, CHANNEL_ID)
            .setContentTitle("VM Running")
            .setContentText("Virtual machine '$vmName' is running")
            .setSmallIcon(android.R.drawable.ic_menu_compass)
            .setOngoing(true)
            .setForegroundServiceBehavior(NotificationCompat.FOREGROUND_SERVICE_IMMEDIATE)
            .build()
    }
}
