cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(CMakeSFMLProject src/Main.cpp
        src/Headers/ConvertSketch.hpp
        src/Headers/DegToRad.hpp
        src/Headers/GetDegrees.hpp
        src/Headers/Global.hpp
        src/Headers/MapCollision.hpp
        src/Headers/Player.hpp
        src/Headers/RadToDeg.hpp
        src/Headers/Steven.hpp
        src/ConvertSketch.cpp
        src/DegToRad.cpp
        src/GetDegrees.cpp
        src/MapCollision.cpp
        src/Player.cpp
        src/RadToDeg.cpp
        src/Steven.cpp
)
target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET CMakeSFMLProject
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS CMakeSFMLProject)
