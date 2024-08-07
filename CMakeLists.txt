cmake_minimum_required(VERSION 3.5)

project(Pomegranate_Engine)
project(Pomegranate)
project(Pomegranate_LiveShareServer)

set(CMAKE_CXX_STANDARD 23)

# Set the path to the SDL library directory
set(LIBRARIES_DIR ${CMAKE_SOURCE_DIR}/Lib/x64)

link_directories("${CMAKE_SOURCE_DIR}/bin/")

# Include directories
include_directories(
        "Include/"
        "Include/Imgui"
        "Include/Lua"
)

# Source files
set(SOURCE_FILES src/main.cpp
        src/entity_hierarchy_window.cpp
        src/entity_hierarchy_window.h
        src/editor_window.cpp
        src/editor_window.h
        src/windows_manager.cpp
        src/windows_manager.h
        src/scene_window.cpp
        src/scene_window.h
        src/inspector_window.cpp
        src/inspector_window.h
        src/resources_window.cpp
        src/resources_window.h
        src/menu_bar.cpp
        src/menu_bar.h
        Include/Pomegranate/utils.cpp
        Include/Pomegranate/utils.h
        src/scene.cpp
        src/scene.h
        src/theme.cpp
        src/theme.h
        src/editor.cpp
        src/editor.h
        src/compile_and_run.cpp
        src/compile_and_run.h
        src/notifications.cpp
        src/notifications.h
        src/hotkey_manager.cpp
        src/hotkey_manager.h
        src/lua.cpp
        src/lua.h
        src/extension_handler.cpp
        src/extension_handler.h
        src/live_share.cpp
        src/live_share.h
        src/LiveShare/live_share_shared_data.cpp
        src/LiveShare/live_share_shared_data.h
        src/run.cpp
        src/run.h
)

set(POMEGRANATE_DIR ${CMAKE_SOURCE_DIR}/Include/Pomegranate)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/Include/Imgui)

add_subdirectory("Include/Pomegranate")

set(POMEGRANATE_SOURCES
        ${POMEGRANATE_DIR}/engine.cpp
        ${POMEGRANATE_DIR}/standard_ecs.cpp
        ${POMEGRANATE_DIR}/ecs.cpp
        ${POMEGRANATE_DIR}/input_manager.cpp
        ${POMEGRANATE_DIR}/vec_math.cpp
        ${POMEGRANATE_DIR}/window.cpp
        ${POMEGRANATE_DIR}/event_system.cpp
        ${POMEGRANATE_DIR}/color.cpp
        ${POMEGRANATE_DIR}/standard_ecs_rendering.cpp
        ${POMEGRANATE_DIR}/standard_ecs_physics.cpp
        ${POMEGRANATE_DIR}/standard_ecs_ui.cpp
        ${POMEGRANATE_DIR}/resource_manager.cpp
        ${POMEGRANATE_DIR}/texture.cpp
        ${POMEGRANATE_DIR}/audio.cpp
        ${POMEGRANATE_DIR}/ttf_font.cpp
        ${POMEGRANATE_DIR}/debug.cpp
        ${POMEGRANATE_DIR}/resource_manager.cpp
        ${POMEGRANATE_DIR}/pomegranate_type_names.h
        ${POMEGRANATE_DIR}/draw.cpp
)

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

set(LIVE_SHARE_SERVER_SOURCES
        src/LiveShare/server.cpp
        src/LiveShare/live_share_shared_data.cpp
)

set(MINE_SOURCES
    Include/mine/mine.cc
    Include/mine/mine.h
)

add_compile_definitions(EDITOR_MODE)

# Add executable
add_executable(Pomegranate_Engine ${POMEGRANATE_SOURCES} ${IMGUI_SOURCES} ${SOURCE_FILES} ${MINE_SOURCES})

add_library(Pomegranate SHARED ${POMEGRANATE_SOURCES} ${IMGUI_SOURCES})

add_executable(Pomegranate_LiveShareServer ${LIVE_SHARE_SERVER_SOURCES} ${MINE_SOURCES})

# Set the path to LuaJIT (Mac)
set(LUAJIT_LIBRARY ${CMAKE_SOURCE_DIR}/Lib/Arm/libluajit-5.1.dylib)  # Adjust the library name if necessary

# Link SDL libraries
if(APPLE)
    # OpenMP is not supported on Apple Silicon to my knowledge.
    target_link_libraries(Pomegranate_Engine PRIVATE
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libSDL3.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libSDL3_image.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libSDL3_mixer.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libSDL3_ttf.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libenet.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libssl.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libcrypto.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libtea.a
            ${LUAJIT_LIBRARY}
    )

    target_link_libraries(Pomegranate PRIVATE
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libSDL3.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libSDL3_image.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libSDL3_mixer.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libSDL3_ttf.dylib
            ${LUAJIT_LIBRARY}
    )

    target_link_libraries(Pomegranate_LiveShareServer PRIVATE
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libenet.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libssl.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libcrypto.dylib
    )
elseif(WIN32)
    find_package(OpenMP)
    target_include_directories(Pomegranate_Engine PRIVATE ${OpenGL_INCLUDE_DIRS})

    target_link_libraries(Pomegranate_Engine PRIVATE
            ${LIBRARIES_DIR}/SDL3.lib
            ${LIBRARIES_DIR}/SDL3_ttf.lib
            ${LIBRARIES_DIR}/SDL3_image.lib
            ${LIBRARIES_DIR}/SDL3_mixer.lib
            ${LIBRARIES_DIR}/lua51.lib
            OpenMP::OpenMP_CXX
            ${LIBRARIES_DIR}/glew32.lib
            ${LIBRARIES_DIR}/glew32s.lib
            ${LIBRARIES_DIR}/enet64.lib
            ${LIBRARIES_DIR}/libssl.lib
            ${LIBRARIES_DIR}/libcrypto.lib
            winmm.lib
            ws2_32.lib
    )
    target_link_libraries(Pomegranate PRIVATE
            ${LIBRARIES_DIR}/SDL3.lib
            ${LIBRARIES_DIR}/SDL3_ttf.lib
            ${LIBRARIES_DIR}/SDL3_image.lib
            ${LIBRARIES_DIR}/SDL3_mixer.lib
            ${LIBRARIES_DIR}/lua51.lib
            OpenMP::OpenMP_CXX
            winmm.lib
            ws2_32.lib
    )
    target_link_libraries(Pomegranate_LiveShareServer PRIVATE
            ${LIBRARIES_DIR}/enet64.lib
            ${LIBRARIES_DIR}/libssl.lib
            ${LIBRARIES_DIR}/libcrypto.lib
            winmm.lib
            ws2_32.lib
    )

endif()

# Set the output directory for the executable
set_target_properties(Pomegranate_Engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)
set_target_properties(Pomegranate_LiveShareServer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)


# Copy SDL DLL files to the output directory
file(GLOB SDL_DLLS ${LIBRARIES_DIR}/*.dll)
foreach(DLL ${SDL_DLLS})
    add_custom_command(TARGET Pomegranate_Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:Pomegranate_Engine>)
endforeach()
