cmake_minimum_required(VERSION 3.16)
project(excute)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_ROOT "/home/devh/Build_EmbSys/Engines/Vendors")
set(platform "x86_64_u20")
# --- Add GLFW ---
add_subdirectory("${VENDOR_ROOT}/glfw" "${CMAKE_BINARY_DIR}/glfw_build")

# --- Add GLAD ---
add_library(glad STATIC
    "${VENDOR_ROOT}/x86_64_u20/glad/src/gl.c"
)

target_include_directories(glad PUBLIC
    $<BUILD_INTERFACE:${VENDOR_ROOT}/x86_64_u20/include>
    $<BUILD_INTERFACE:${VENDOR_ROOT}/x86_64_u20/include/glad>
    $<INSTALL_INTERFACE:include/glad>
    $<INSTALL_INTERFACE:include/KHR>
)

# file(GLOB SOURCE_FILES_DIR
#     "${CMAKE_SOURCE_DIR}/robot/2DOF/*.cpp"
#     "${CMAKE_SOURCE_DIR}/window/*.cpp"
#     "${CMAKE_SOURCE_DIR}/gcode/*.cpp"
# )
# --- Robot2DOF App ---
add_executable(${PROJECT_NAME}
    2_main.cpp
    ${SOURCE_FILES_DIR}
)

# target_include_directories(${PROJECT_NAME} PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/base>
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/robot/2DOF>
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/window>
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gcode>
#     $<INSTALL_INTERFACE:include/robot2dof>
# )

# --- Link ---
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
# --- OpenGL ---
if(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif()

# --- Include dirs (GLAD & GLFW headers) ---
target_include_directories(${PROJECT_NAME} PRIVATE
    "${VENDOR_ROOT}/glfw/include"
    "${VENDOR_ROOT}/x86_64_u20/glad/include"
)
