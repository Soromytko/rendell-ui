cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(RendellUI)

set(BINARY_NAME rendell_ui)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    src/initialization.cpp
    src/Window.cpp
    src/Viewport.cpp
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

    rendell-text/src/rendell_text.cpp
    rendell-text/src/FontRaster.cpp
    rendell-text/src/TextRenderer.cpp
    rendell-text/src/TextBuffer.cpp
    rendell-text/src/GlyphBuffer.cpp
    rendell-text/src/TextBatch.cpp
    rendell-text/src/RasteredFontStorage.cpp
    rendell-text/src/RasteredFontStorageManager.cpp
)

set(HEADER_FILES
    src/initialization.h
    src/Window.h
    src/Viewport.h
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

    rendell-text/src/rendell_text.h
    rendell-text/src/FontRaster.h
    rendell-text/src/TextRenderer.h
    rendell-text/src/TextBuffer.h
    rendell-text/src/GlyphBuffer.h
    rendell-text/src/TextBatch.h
    rendell-text/src/RasteredFontStorage.h
    rendell-text/src/RasteredFontStorageManager.h
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

# Rendell-text
target_include_directories(${BINARY_NAME} PUBLIC rendell-text/include)

# rendell
target_include_directories(${BINARY_NAME} PUBLIC ${RENDELL_INCLUDE_DIRECTORIES})

# Glm
target_include_directories(${BINARY_NAME} PUBLIC ${GLM_INCLUDE_DIRECTORIES})
