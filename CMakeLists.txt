cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)


project(tetris)
file(GLOB SRC 
"./src/main.cpp"
"./src/GLFW_wnd.cpp"
"./src/scene.cpp"
"./src/opengl.cpp"
"./src/tetris.cpp"
"./src/glad.c"
)

find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} ${SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw GL)