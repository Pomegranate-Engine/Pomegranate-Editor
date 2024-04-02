#[===[
set(SDL_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/Lib/x64)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
# Add the following lines to enable OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_library(Pomegranate_Library SHARED ${POMEGRANATE_SOURCES})

# Link SDL2 libraries
target_link_libraries(Pomegranate_Library PRIVATE
        ${SDL_LIBRARY_DIR}/SDL3.lib
        ${SDL_LIBRARY_DIR}/SDL3_ttf.lib
        ${SDL_LIBRARY_DIR}/SDL3_image.lib
        ${SDL_LIBRARY_DIR}/SDL3_mixer.lib
)

# Set the output directory for the shared library
set_target_properties(Pomegranate_Library PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Copy SDL DLL files to the output directory for the shared library
foreach(DLL ${SDL_DLLS})
    add_custom_command(TARGET Pomegranate_Library POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:Pomegranate_Library>)
endforeach()
#]===]