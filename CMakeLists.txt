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

    # Use find_package for all platforms (vcpkg toolchain handles Windows automatically)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image QUIET)
    find_package(SDL2_ttf QUIET)
    find_package(SDL2_mixer QUIET)
    
    if(NOT WIN32)
        # Unix platforms: fallback to PkgConfig if not found
        if(NOT SDL2_image_FOUND OR NOT SDL2_ttf_FOUND OR NOT SDL2_mixer_FOUND)
            find_package(PkgConfig REQUIRED)
            if(NOT SDL2_image_FOUND)
                pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
            endif()
            if(NOT SDL2_ttf_FOUND)
                pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
            endif()
            if(NOT SDL2_mixer_FOUND)
                pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
            endif()
        endif()
    endif()

    # Find OpenGL
    find_package(OpenGL REQUIRED)

    # Find GLEW (vcpkg toolchain handles Windows)
    find_package(GLEW QUIET)
    if(NOT GLEW_FOUND)
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(GLEW QUIET glew)
        endif()
    endif()
    
    if(NOT GLEW_FOUND AND APPLE)
        # If still not found on macOS, assume Homebrew installation
        set(GLEW_INCLUDE_DIRS "/opt/homebrew/include")
        set(GLEW_LIBRARIES "/opt/homebrew/lib/libGLEW.dylib")
    endif()
    
    if(GLEW_FOUND OR GLEW_INCLUDE_DIRS)
        include_directories(${GLEW_INCLUDE_DIRS})
    endif()

    # Include directories
    include_directories(src ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

    add_executable(Dungeons ${SOURCES})

    target_include_directories(Dungeons PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
    
    if(GLEW_FOUND OR GLEW_INCLUDE_DIRS)
        target_include_directories(Dungeons PRIVATE ${GLEW_INCLUDE_DIRS})
    endif()
    
    # Handle SDL2 link libraries - prefer targets, fallback to variables
    if(TARGET SDL2::SDL2)
        target_link_libraries(Dungeons PRIVATE SDL2::SDL2)
    else()
        target_link_libraries(Dungeons PRIVATE ${SDL2_LIBRARIES})
    endif()

    if(TARGET SDL2_image::SDL2_image)
        target_link_libraries(Dungeons PRIVATE SDL2_image::SDL2_image)
    else()
        if(SDL2_IMAGE_LIBRARIES)
            target_link_libraries(Dungeons PRIVATE ${SDL2_IMAGE_LIBRARIES})
        elseif(WIN32)
            # Windows fallback: link libraries directly
            target_link_libraries(Dungeons PRIVATE SDL2_image libpng16 zlib)
        endif()
    endif()

    if(TARGET SDL2_ttf::SDL2_ttf)
        target_link_libraries(Dungeons PRIVATE SDL2_ttf::SDL2_ttf)
    else()
        if(SDL2_TTF_LIBRARIES)
            target_link_libraries(Dungeons PRIVATE ${SDL2_TTF_LIBRARIES})
        elseif(WIN32)
            # Windows fallback: link libraries directly
            target_link_libraries(Dungeons PRIVATE SDL2_ttf freetype)
        endif()
    endif()

    if(TARGET SDL2_mixer::SDL2_mixer)
        target_link_libraries(Dungeons PRIVATE SDL2_mixer::SDL2_mixer)
    else()
        if(SDL2_MIXER_LIBRARIES)
            target_link_libraries(Dungeons PRIVATE ${SDL2_MIXER_LIBRARIES})
        elseif(WIN32)
            # Windows fallback: link libraries directly
            target_link_libraries(Dungeons PRIVATE SDL2_mixer flac ogg vorbis vorbisfile)
        endif()
    endif()

    if(WIN32)
        target_link_libraries(Dungeons PRIVATE opengl32 user32 gdi32)
        # Add vcpkg lib directory as hint for SDL2 extensions
        link_directories(${CMAKE_FIND_ROOT_PATH}/lib)
        # Fix CRT library conflicts - use dynamic CRT matching vcpkg static triplet
        target_link_options(Dungeons PRIVATE /NODEFAULTLIB:LIBCMT)
        set_target_properties(Dungeons PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()

    if(GLEW_FOUND)
        target_link_libraries(Dungeons PRIVATE GLEW::GLEW)
    elseif(GLEW_LIBRARIES)
        target_link_libraries(Dungeons PRIVATE ${GLEW_LIBRARIES})
    endif()

    target_link_libraries(Dungeons PRIVATE ${OPENGL_LIBRARIES})

    # Create a custom target to sync assets every time we build
    add_custom_target(sync_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
        COMMENT "Syncing assets to build directory...")
    
    add_dependencies(Dungeons sync_assets)
endif()
