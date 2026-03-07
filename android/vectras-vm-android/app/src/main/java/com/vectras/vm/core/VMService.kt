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

    data class BridgeMetrics(
        val cpuUsagePercent: Float,
        val memoryUsedMB: Long,
        val memoryTotalMB: Long,
        val diskReadMBps: Float,
        val diskWriteMBps: Float,
        val networkRxKBps: Float,
        val networkTxKBps: Float,
        val fps: Int,
        val vncConnected: Boolean
    )
    
    companion object {
        const val CHANNEL_ID = "vm_execution_channel"
        const val NOTIFICATION_ID = 1
        const val ACTION_START_VM = "com.vectras.vm.START_VM"
        const val ACTION_STOP_VM = "com.vectras.vm.STOP_VM"
        const val EXTRA_VM_ID = "vm_id"

        private var nativeAvailable = false

        init {
            nativeAvailable = try {
                System.loadLibrary("rafaelia_bridge")
                true
            } catch (_: UnsatisfiedLinkError) {
                false
            }
        }

        @JvmStatic
        fun startVm(vmId: String): Boolean {
            return if (nativeAvailable) {
                nativeStartVm(vmId)
            } else {
                false
            }
        }

        @JvmStatic
        fun stopVm(): Boolean {
            return if (nativeAvailable) {
                nativeStopVm()
            } else {
                false
            }
        }

        @JvmStatic
        fun collectMetrics(): BridgeMetrics? {
            if (!nativeAvailable) {
                return null
            }

            val values = nativeCollectMetrics() ?: return null
            if (values.size < 9) {
                return null
            }

            return BridgeMetrics(
                cpuUsagePercent = values[0],
                memoryUsedMB = values[1].toLong(),
                memoryTotalMB = values[2].toLong(),
                diskReadMBps = values[3],
                diskWriteMBps = values[4],
                networkRxKBps = values[5],
                networkTxKBps = values[6],
                fps = values[7].toInt(),
                vncConnected = values[8] > 0f
            )
        }

        @JvmStatic
        private external fun nativeStartVm(vmId: String): Boolean

        @JvmStatic
        private external fun nativeStopVm(): Boolean

        @JvmStatic
        private external fun nativeCollectMetrics(): FloatArray?
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
                startVm(vmId ?: "default")
            }
            ACTION_STOP_VM -> {
                stopVm()
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
