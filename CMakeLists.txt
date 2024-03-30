cmake_minimum_required(VERSION 3.21)

project(world)

aux_source_directory(src source)

add_executable(main ${source})

target_include_directories(main PUBLIC
        D:/libs/glew-2.1.0/include
        D:/libs/glfw-3.3.9/include
        D:/libs/glm
        D:/projects/c++/imstilltrying/OpenSimplexNoise)

target_link_libraries(main PUBLIC
        D:/libs/glew-2.1.0/lib/Release/x64/glew32.lib
        opengl32.dll
        D:/libs/glfw-3.3.9/lib-mingw-w64/glfw3.dll
        D:/projects/c++/imstilltrying/cmake-build-debug/libosn_lib.dll)

aux_source_directory(OpenSimplexNoise osn_src)

add_library(osn_lib SHARED ${osn_src})