cmake_minimum_required(VERSION 3.5)

include_directories(BEFORE SYSTEM include)

macro(egl_target_link_libraries target)
    target_link_libraries(${target} 
        /usr/lib/nvidia-361/libEGL.so
        /usr/lib/nvidia-361/libGL.so
        /usr/lib/nvidia-361/libGLESv2.so
        X11
        /usr/lib/x86_64-linux-gnu/libX11.so.6
        m)
endmacro(egl_target_link_libraries)

# add_executable(eglgears
#     eglgears.c)
# egl_target_link_libraries(eglgears)

# add_executable(hourglass
#     hourglass.c)
# egl_target_link_libraries(hourglass)

add_executable(etri
    etri.c)
egl_target_link_libraries(etri)
