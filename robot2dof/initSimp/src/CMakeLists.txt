cmake_minimum_required(VERSION 3.14)
project(robot2dof)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_ROOT "/home/devh/Build_EmbSys/Engines/Vendors")

# --- Add GLFW ---
add_subdirectory("${VENDOR_ROOT}/glfw" "${CMAKE_BINARY_DIR}/glfw_build")

# --- Add GLAD ---
add_library(glad STATIC
    "${VENDOR_ROOT}/x86_64_u20/glad/src/gl.c"
)

target_include_directories(glad PUBLIC
    $<BUILD_INTERFACE:${VENDOR_ROOT}/x86_64_u20/include>
    $<INSTALL_INTERFACE:include/glad>
    $<INSTALL_INTERFACE:include/KHR>
)
file(GLOB WINDOW_SRC
    "${CMAKE_SOURCE_DIR}/window/*.cpp"
)
# --- Robot2DOF App ---
add_executable(robot2dof
    application.cpp
    ${WINDOW_SRC}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/window>
    $<INSTALL_INTERFACE:include/robot2dof>
)

# --- Link ---
target_link_libraries(robot2dof PRIVATE glfw glad)

# --- OpenGL ---
if(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(robot2dof PRIVATE OpenGL::GL)
endif()

# --- Include dirs (GLAD & GLFW headers) ---
target_include_directories(robot2dof PRIVATE
    "${VENDOR_ROOT}/glfw/include"
    "${VENDOR_ROOT}/x86_64_u20/glad/include"
)
