cmake_minimum_required(VERSION 3.10)

# set project name, version, and language
project(SandboxSim LANGUAGES CXX)

# set the c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add glfw subdir
add_subdirectory(third_party/glfw)

# add glad
add_library(GLAD STATIC src/glad/src/glad.c)
target_include_directories(GLAD PUBLIC src/glad/include)

# set source files variable
set(SOURCES 
    src/main.cpp 
    src/shader.cpp 
    src/resource_manager.cpp 
    )

# set header files variable
set(HEADERS 
    src/shader.h 
    src/resource_manager.h )

# add the executable with proper source code
add_executable(SandboxSim ${SOURCES} ${HEADERS})

# link libraries: glad, glfw, opengl
target_link_libraries(SandboxSim PRIVATE glfw GLAD opengl32)

# include directories
target_include_directories(SandboxSim PRIVATE 
    src 
    src/glad/include 
    third_party/glfw/include 
    third_party/glm
)

# copy shader files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# change working directory to cmake source dir (must come after the exectuable is made (add_executable()))
set_target_properties(SandboxSim PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
