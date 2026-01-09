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

    # On Windows with vcpkg, manually add all SDL2 module libraries
    if(WIN32)
        # Use the Release (optimized) library directory only
        set(VCPKG_LIB_DIR "${CMAKE_FIND_ROOT_PATH}/lib")
        
        set(SDL2_IMAGE_LIBRARIES 
            ${VCPKG_LIB_DIR}/SDL2_image.lib
            ${VCPKG_LIB_DIR}/libpng16.lib
            ${VCPKG_LIB_DIR}/zlib.lib
        )
        set(SDL2_TTF_LIBRARIES 
            ${VCPKG_LIB_DIR}/SDL2_ttf.lib
            ${VCPKG_LIB_DIR}/freetype.lib
        )
        set(SDL2_MIXER_LIBRARIES 
            ${VCPKG_LIB_DIR}/SDL2_mixer.lib
            ${VCPKG_LIB_DIR}/flac.lib
            ${VCPKG_LIB_DIR}/ogg.lib
            ${VCPKG_LIB_DIR}/vorbis.lib
            ${VCPKG_LIB_DIR}/vorbisfile.lib
        )
    else()
        # On Linux/macOS, try find_package first, fallback to PkgConfig
        find_package(SDL2_image QUIET)
        find_package(SDL2_ttf QUIET)
        find_package(SDL2_mixer QUIET)
        
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
    else()
        target_link_libraries(Dungeons PRIVATE ${SDL2_IMAGE_LIBRARIES})
    endif()

    if(TARGET SDL2_ttf::SDL2_ttf)
        target_link_libraries(Dungeons PRIVATE SDL2_ttf::SDL2_ttf)
    else()
        target_link_libraries(Dungeons PRIVATE ${SDL2_TTF_LIBRARIES})
    endif()

    if(TARGET SDL2_mixer::SDL2_mixer)
        target_link_libraries(Dungeons PRIVATE SDL2_mixer::SDL2_mixer)
    else()
        target_link_libraries(Dungeons PRIVATE ${SDL2_MIXER_LIBRARIES})
    endif()

    if(WIN32)
        target_link_libraries(Dungeons PRIVATE GLEW::GLEW)
        # Link Windows system libraries
        target_link_libraries(Dungeons PRIVATE opengl32 user32 gdi32)
        # Fix CRT library conflicts - use dynamic CRT matching vcpkg static triplet
        target_link_options(Dungeons PRIVATE /NODEFAULTLIB:LIBCMT)
        set_target_properties(Dungeons PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
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
