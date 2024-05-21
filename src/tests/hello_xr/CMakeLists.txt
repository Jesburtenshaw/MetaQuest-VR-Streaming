# Copyright (c) 2017-2024, The Khronos Group Inc.
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Hint the path to the OpenCVConfig.cmake file
set(OpenCV_DIR "D:/opencv/opencv/build/install/OpenCV-android-sdk/sdk/native/jni")
set(GSTREAMER_ROOT_ANDROID "D:/Downloads/gstreamer-1.0-android-universal-1.24.3.tar/gstreamer-1.0-android-universal-1.24.3")

find_package(OpenCV REQUIRED java)

set(LOCAL_HEADERS
    check.h
    common.h
    geometry.h
    graphicsapi.h
    graphicsplugin.h
    logger.h
    openxr_program.h
    options.h
    pch.h
    platformdata.h
    platformplugin.h
)
set(LOCAL_SOURCE
    graphicsplugin_factory.cpp
    graphicsplugin_vulkan.cpp
    logger.cpp
    main.cpp
    openxr_program.cpp
    pch.cpp
    platformplugin_android.cpp
    platformplugin_factory.cpp
    platformplugin_posix.cpp
    platformplugin_win32.cpp
)
set(VULKAN_SHADERS vulkan_shaders/frag.glsl vulkan_shaders/vert.glsl)

if(ANDROID)
    if (${ANDROID_ABI} STREQUAL "armeabi")
        set(GSTREAMER_ROOT "${GSTREAMER_ROOT_ANDROID}/arm")
    elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a")
        set(GSTREAMER_ROOT "${GSTREAMER_ROOT_ANDROID}/armv7")
    elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
        set(GSTREAMER_ROOT "${GSTREAMER_ROOT_ANDROID}/arm64")
    elseif (${ANDROID_ABI} STREQUAL "x86")
        set(GSTREAMER_ROOT "${GSTREAMER_ROOT_ANDROID}/x86")
    elseif (${ANDROID_ABI} STREQUAL "x86_64")
        set(GSTREAMER_ROOT "${GSTREAMER_ROOT_ANDROID}/x86_64")
    else ()
        message(STATUS "error Target arch ABI not supported: ${ANDROID_ABI}")
    endif()

    link_directories(
            ${GSTREAMER_ROOT}/lib
            ${GSTREAMER_ROOT}/lib/gstreamer-1.0)
    
    SET(GST_LIBS gstreamer-1.0 gmodule-2.0 gobject-2.0 glib-2.0 iconv intl ffi)
    SET(GST_PLUGINS_CORE_LIBS gstaudioresample gstaudiotestsrc gstaudioconvert)
    SET(GST_PLUGINS_BASE gstautodetect gstallocators-1.0 orc-test-0.4 gstapp-1.0 gstaudio-1.0 gstfft-1.0 gstpbutils-1.0 gstriff-1.0 gstrtp-1.0 gstrtsp-1.0 gstsdp-1.0 gsttag-1.0 gstvideo-1.0 gstgl-1.0 orc-0.4 gstopensles OpenSLES gstaudio-1.0 gstbase-1.0)

    add_library(
        hello_xr MODULE
        ${LOCAL_SOURCE}
        ${LOCAL_HEADERS}
        ${VULKAN_SHADERS}
        $<TARGET_OBJECTS:android_native_app_glue>
    )

    target_include_directories(
            hello_xr PRIVATE
            "${GSTREAMER_ROOT}/include/gstreamer-1.0"
            "${GSTREAMER_ROOT}/include/glib-2.0"
            "${GSTREAMER_ROOT}/lib/glib-2.0/include")
    
    target_link_libraries(
        hello_xr PRIVATE ${ANDROID_LIBRARY} ${ANDROID_LOG_LIBRARY} ${GST_LIBS} ${GST_PLUGINS_CORE_LIBS} ${GST_PLUGINS_BASE}
    )

    # Only for Android because it lacks a command line
    set(HELLOXR_DEFAULT_GRAPHICS_PLUGIN "Vulkan")

    # Now handle the value, normally provided by Gradle
    message(STATUS "hello_xr will default to Vulkan graphics plugin")
    target_compile_definitions(
        hello_xr PRIVATE DEFAULT_GRAPHICS_PLUGIN_VULKAN
    )

else()
    add_executable(hello_xr ${LOCAL_SOURCE} ${LOCAL_HEADERS} ${VULKAN_SHADERS})
endif()

set_target_properties(hello_xr PROPERTIES FOLDER ${SAMPLES_FOLDER})
source_group("Headers" FILES ${LOCAL_HEADERS})
source_group("Shaders" FILES ${VULKAN_SHADERS})

target_link_libraries(hello_xr PRIVATE OpenXR::openxr_loader ${OpenCV_LIBS})

compile_glsl(run_hello_xr_glsl_compiles ${VULKAN_SHADERS})

add_dependencies(hello_xr run_hello_xr_glsl_compiles)

target_include_directories(
    hello_xr
    PRIVATE
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/src/common"
        "${PROJECT_SOURCE_DIR}/src/tests/hello_xr/vulkan_shaders"
        # for helper headers
        "${PROJECT_SOURCE_DIR}/external/include"
        # For including compiled shaders
        "${CMAKE_CURRENT_BINARY_DIR}"
		# OpenCV
		${OpenCV_INCLUDE_DIRS}
)


if(GLSLANG_VALIDATOR AND NOT GLSLC_COMMAND)
    target_compile_definitions(hello_xr PRIVATE USE_GLSLANGVALIDATOR)
endif()

if(XR_USE_GRAPHICS_API_VULKAN)
    target_include_directories(hello_xr PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(hello_xr PRIVATE ${Vulkan_LIBRARY})
endif()

if(TARGET openxr-gfxwrapper)
    target_link_libraries(hello_xr PRIVATE openxr-gfxwrapper)
endif()
if(WIN32)
    target_link_libraries(hello_xr PRIVATE ole32)
    if(MSVC)
        target_compile_definitions(hello_xr PRIVATE _CRT_SECURE_NO_WARNINGS)
        target_compile_options(hello_xr PRIVATE /Zc:wchar_t /Zc:forScope /W4)
        if(NOT
           CMAKE_CXX_COMPILER_ID
           STREQUAL
           "Clang"
        )
            # If actually msvc and not clang-cl
            target_compile_options(openxr_c_compile_test PRIVATE /WX)
        endif()

        # Right now can't build d3d features on MinGW because of directxcolors, directxmath, etc.
        target_link_libraries(
            hello_xr
            PRIVATE
                d3d11
                d3d12
                d3dcompiler
                dxgi
        )
    endif()
endif()

if(NOT ANDROID)
    install(
        TARGETS hello_xr RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                                 COMPONENT hello_xr
    )
    if(NOT WIN32)
        install(
            FILES hello_xr.1
            DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/
            COMPONENT ManPages
        )
    endif()
endif()
