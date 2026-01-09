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

    # Find SDL2 modules - try find_package first, fallback to PkgConfig
    find_package(SDL2_image QUIET)
    find_package(SDL2_ttf QUIET)
    find_package(SDL2_mixer QUIET)

    # On Windows with vcpkg, targets may not be created, so use PkgConfig fallback
    if((NOT TARGET SDL2_image::SDL2_image) OR (NOT TARGET SDL2_ttf::SDL2_ttf) OR (NOT TARGET SDL2_mixer::SDL2_mixer))
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(SDL2_IMAGE QUIET SDL2_image)
            pkg_check_modules(SDL2_TTF QUIET SDL2_ttf)
            pkg_check_modules(SDL2_MIXER QUIET SDL2_mixer)
        endif()
        
        # If still not found, try harder on Windows with vcpkg
        if(WIN32 AND NOT SDL2_IMAGE_FOUND)
            find_package(SDL2_image REQUIRED)
            find_package(SDL2_ttf REQUIRED)
            find_package(SDL2_mixer REQUIRED)
        elseif(NOT WIN32 AND (NOT SDL2_IMAGE_FOUND OR NOT SDL2_TTF_FOUND OR NOT SDL2_MIXER_FOUND))
            message(FATAL_ERROR "SDL2 modules not found")
        endif()
    endif()

    # Find OpenGL
    find_package(OpenGL REQUIRED)

    if(WIN32)
        find_package(GLEW REQUIRED)
        include_directories(${GLEW_INCLUDE_DIRS})
    endif()

    # Include directories
    include_directories(src ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

    add_executable(Dungeons ${SOURCES})

    target_include_directories(Dungeons PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
    
    # Handle SDL2 link libraries - prefer targets, fallback to variables
    if(TARGET SDL2::SDL2)
        target_link_libraries(Dungeons PRIVATE SDL2::SDL2)
    else()
        target_link_libraries(Dungeons PRIVATE ${SDL2_LIBRARIES})
    endif()

    if(TARGET SDL2_image::SDL2_image)
        target_link_libraries(Dungeons PRIVATE SDL2_image::SDL2_image)
    elseif(SDL2_IMAGE_LIBRARIES)
        target_link_libraries(Dungeons PRIVATE ${SDL2_IMAGE_LIBRARIES})
    endif()

    if(TARGET SDL2_ttf::SDL2_ttf)
        target_link_libraries(Dungeons PRIVATE SDL2_ttf::SDL2_ttf)
    elseif(SDL2_TTF_LIBRARIES)
        target_link_libraries(Dungeons PRIVATE ${SDL2_TTF_LIBRARIES})
    endif()

    if(TARGET SDL2_mixer::SDL2_mixer)
        target_link_libraries(Dungeons PRIVATE SDL2_mixer::SDL2_mixer)
    elseif(SDL2_MIXER_LIBRARIES)
        target_link_libraries(Dungeons PRIVATE ${SDL2_MIXER_LIBRARIES})
    endif()

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
