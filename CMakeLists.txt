cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(RendellUI)

set(BINARY_NAME rendell_ui)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    src/initialization.cpp
    src/Window.cpp
    src/Viewport.cpp
    src/Time.cpp
    src/input.cpp
    src/Canvas.cpp
    src/Widgets/Widget.cpp
    src/Widgets/Transform2D.cpp
    src/Widgets/Rectangle.cpp
    src/Widgets/Text.cpp
    src/Widgets/WidgetRegistrator.cpp
    src/Widgets/WidgetHandlePipeline.cpp
    src/Widgets/WidgetRenderPipeline.cpp
    src/Shaders/Shader.cpp
    src/Shaders/ShaderStorage.cpp
    src/Shaders/RectangleShader.cpp
)

set(HEADER_FILES
    src/WindowEventHandler.h
    src/initialization.h
    src/Window.h
    src/Viewport.h
    src/Time.h
    src/input.h
    src/Canvas.h
    src/Widgets/Widget.h
    src/Widgets/Transform2D.h
    src/Widgets/Rectangle.h
    src/Widgets/Text.h
    src/Widgets/WidgetRegistrator.h
    src/Widgets/WidgetHandlePipeline.h
    src/Widgets/WidgetRenderPipeline.h
    src/Shaders/Shader.h
    src/Shaders/ShaderStorage.h
    src/Shaders/RectangleShader.h
)

add_library(${BINARY_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${BINARY_NAME} PUBLIC include)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
target_link_libraries(${BINARY_NAME} glfw)

# FreeType
add_subdirectory(freetype)
target_link_libraries(${BINARY_NAME} freetype)

# rendell
target_link_libraries(${BINARY_NAME} ${RENDELL_LIB})

# Glm
target_link_libraries(${BINARY_NAME} ${GLM_LIB})

# Rendell-text
set(FREETYPE_LIB freetype)
add_subdirectory(rendell-text)
target_link_libraries(${BINARY_NAME} rendell_text)
