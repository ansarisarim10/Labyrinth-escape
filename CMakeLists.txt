cmake_minimum_required(VERSION 3.10)
project(LabyrinthEscape)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include dirs (match your folders)
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Libraries/glfw/include")
set(GLM_INCLUDE_DIR  "${CMAKE_SOURCE_DIR}/Libraries")           # glm/glm.hpp lives under Libraries/
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Libraries/glad/include")
include_directories(${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

# Sources
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/Libraries/glad/src/glad.c")

add_executable(LabyrinthEscape
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/Shader.cpp
    ${CMAKE_SOURCE_DIR}/Camera.cpp
    ${CMAKE_SOURCE_DIR}/Mesh.cpp
    ${GLAD_SRC}
)

# Link 32-bit GLFW (course libs)
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/Libraries/glfw/lib")
set(GLFW3_LIB_RELEASE "${GLFW_LIB_DIR}/glfw3.lib")
set(GLFW3_LIB_DEBUG   "${GLFW_LIB_DIR}/glfw3d.lib")

target_link_libraries(LabyrinthEscape PRIVATE
    $<$<CONFIG:Debug>:${GLFW3_LIB_DEBUG}>
    $<$<NOT:$<CONFIG:Debug>>:${GLFW3_LIB_RELEASE}>
    opengl32 gdi32 user32 shell32 kernel32 imm32 winmm
)

# Copy shaders next to the exe
add_custom_command(TARGET LabyrinthEscape POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/basic.vert
        ${CMAKE_SOURCE_DIR}/basic.frag
        $<TARGET_FILE_DIR:LabyrinthEscape>/
)
