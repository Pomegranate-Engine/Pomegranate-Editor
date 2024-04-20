cmake_minimum_required(VERSION 3.0)

project(Pomegranate_Engine)

set(CMAKE_CXX_STANDARD 17)

# Set the path to the SDL library directory
set(SDL_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/Lib/x64)

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
        src/resources_window.h)

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
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
        src/menu_bar.cpp
        src/menu_bar.h
        src/utils.cpp
        src/utils.h
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
)

# Add executable
add_executable(Pomegranate_Engine ${SOURCE_FILES} ${POMEGRANATE_SOURCES} ${IMGUI_SOURCES})

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
            ${LUAJIT_LIBRARY}
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libvulkan.1.3.275.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libvulkan.1.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libvulkan.dylib
            ${CMAKE_SOURCE_DIR}/Lib/Arm/libMoltenVK.dylib
    )
elseif(WIN32)
    find_package(OpenMP)
    target_link_libraries(Pomegranate_Engine PRIVATE
            ${SDL_LIBRARY_DIR}/SDL3.lib
            ${SDL_LIBRARY_DIR}/SDL3_ttf.lib
            ${SDL_LIBRARY_DIR}/SDL3_image.lib
            ${SDL_LIBRARY_DIR}/SDL3_mixer.lib
            ${SDL_LIBRARY_DIR}/lua51.lib
            OpenMP::OpenMP_CXX
    )
endif()

# Set the output directory for the executable
set_target_properties(Pomegranate_Engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)


# Copy SDL DLL files to the output directory
file(GLOB SDL_DLLS ${SDL_LIBRARY_DIR}/*.dll)
foreach(DLL ${SDL_DLLS})
    add_custom_command(TARGET Pomegranate_Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:Pomegranate_Engine>)
endforeach()
