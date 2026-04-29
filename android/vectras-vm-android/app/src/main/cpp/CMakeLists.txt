cmake_minimum_required(VERSION 3.22.1)

project(rafaelia_bridge C)

add_library(rafaelia_bridge SHARED
    rafaelia_bridge.c
    rafaelia_bridge_runtime.c
)

find_library(log-lib log)

target_compile_features(rafaelia_bridge PRIVATE c_std_11)

target_link_libraries(rafaelia_bridge
    ${log-lib}
)
