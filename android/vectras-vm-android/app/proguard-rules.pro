# Vectras VM Android - ProGuard Rules
# ISO 9001/8000 Compliant

# Keep mathematical utility classes
-keep class com.vectras.vm.core.MathUtils { *; }
-keep class com.vectras.vm.core.GoldenTestVectors { *; }
-keep class com.vectras.vm.util.PerformanceMetrics { *; }

# Keep data classes
-keepclassmembers class * {
    @kotlinx.serialization.* <methods>;
}
