cmake_minimum_required(VERSION 3.10)
project(Dungeons VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Gather source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    # Emscripten flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s ALLOW_MEMORY_GROWTH=1 -s FULL_ES3=1 -s USE_WEBGL2=1 --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets")

    add_executable(Dungeons ${SOURCES})
else()
    # Native Build
    # Link directories for Homebrew on Apple Silicon (keep for user's local setup)
    link_directories(/opt/homebrew/lib)
    include_directories(/opt/homebrew/include)

    # Find SDL2
    find_package(SDL2 REQUIRED)

    # Find SDL2 modules
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    # Find OpenGL
    find_package(OpenGL REQUIRED)

    if(WIN32)
        find_package(GLEW REQUIRED)
        include_directories(${GLEW_INCLUDE_DIRS})
    endif()

    # Include directories
    include_directories(src)

    add_executable(Dungeons ${SOURCES})

    target_include_directories(Dungeons PRIVATE ${SDL2_INCLUDE_DIRS})
    
    # Link SDL2 and components
    target_link_libraries(Dungeons PRIVATE 
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
    )

    if(WIN32)
        target_link_libraries(Dungeons PRIVATE GLEW::GLEW)
    endif()

    target_link_libraries(Dungeons PRIVATE ${OPENGL_LIBRARIES})

    # Set Windows subsystem for GUI application
    if(WIN32)
        set_target_properties(Dungeons PROPERTIES
            WIN32_EXECUTABLE ON
        )
    endif()

    # Create a custom target to sync assets every time we build
    add_custom_target(sync_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
        COMMENT "Syncing assets to build directory...")
    
    add_dependencies(Dungeons sync_assets)
endif()
