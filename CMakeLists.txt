cmake_minimum_required(VERSION 3.10)
project(MyProject C)
set(CMAKE_C_STANDARD 11)

if(WIN32)
    # On Windows, attempt to find the libraries using CMake's find_package
    find_package(JPEG REQUIRED)
    find_library(HEIF_LIBRARY NAMES heif)

    if(NOT HEIF_LIBRARY)
        message(FATAL_ERROR "libheif not found. Please install libheif and make sure it's in your PATH.")
    endif()

    # Add the library and include directories manually if needed
    include_directories(${JPEG_INCLUDE_DIR})
    set(LIBRARIES ${HEIF_LIBRARY} ${JPEG_LIBRARIES})

    # Specify the source files
    set(SOURCES
        src/main.c
        src/another_file.c
    )

    # Add the executable target
    add_executable(my_program ${SOURCES})

    # Link the libraries to the executable
    target_link_libraries(my_program ${LIBRARIES})

    # Make sure that the linker can find the libraries
    if(NOT WIN32)
        link_directories(${LIBHEIF_LIBRARY_DIRS} ${JPEG_LIBRARY_DIRS})
    endif()

    # Compiler flags for the dependencies
    if(DEFINED LIBHEIF_CFLAGS_OTHER)
        target_compile_options(my_program PRIVATE ${LIBHEIF_CFLAGS_OTHER})
    endif()

else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBHEIF REQUIRED libheif)
    find_package(JPEG REQUIRED)
    include_directories(${LIBHEIF_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})
    set(SOURCES
        main.c
        )
    add_executable(main ${SOURCES})
    target_link_libraries(main ${LIBHEIF_LIBRARIES} ${JPEG_LIBRARIES})
    link_directories(${LIBHEIF_LIBRARY_DIRS} ${JPEG_LIBRARY_DIRS})

    target_compile_options(main PRIVATE ${LIBHEIF_CFLAGS_OTHER})
endif()