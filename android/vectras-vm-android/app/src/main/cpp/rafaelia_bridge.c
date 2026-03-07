#include <jni.h>
#include <stdbool.h>
#include "hw/core/rafaelia_bridge.h"

JNIEXPORT jboolean JNICALL
Java_com_vectras_vm_core_VMService_00024Companion_nativeStartVm(JNIEnv *env,
                                                                 jclass clazz,
                                                                 jstring vm_id)
{
    const char *id = NULL;
    bool ok;

    (void)clazz;
    if (vm_id != NULL) {
        id = (*env)->GetStringUTFChars(env, vm_id, NULL);
    }

    ok = rafaelia_bridge_start_vm(id);

    if (id != NULL) {
        (*env)->ReleaseStringUTFChars(env, vm_id, id);
    }

    return ok ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_vectras_vm_core_VMService_00024Companion_nativeStopVm(JNIEnv *env,
                                                                jclass clazz)
{
    (void)env;
    (void)clazz;
    return rafaelia_bridge_stop_vm() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jfloatArray JNICALL
Java_com_vectras_vm_core_VMService_00024Companion_nativeCollectMetrics(JNIEnv *env,
                                                                        jclass clazz)
{
    rafaelia_bridge_metrics_t metrics;
    jfloat values[9];
    jfloatArray out;

    (void)clazz;
    if (!rafaelia_bridge_collect_metrics(&metrics)) {
        return NULL;
    }

    values[0] = metrics.cpu_usage_percent;
    values[1] = (jfloat)metrics.memory_used_mb;
    values[2] = (jfloat)metrics.memory_total_mb;
    values[3] = metrics.disk_read_mbps;
    values[4] = metrics.disk_write_mbps;
    values[5] = metrics.network_rx_kbps;
    values[6] = metrics.network_tx_kbps;
    values[7] = (jfloat)metrics.fps;
    values[8] = metrics.vnc_connected ? 1.0f : 0.0f;

    out = (*env)->NewFloatArray(env, 9);
    if (out == NULL) {
        return NULL;
    }

    (*env)->SetFloatArrayRegion(env, out, 0, 9, values);
    return out;
}
