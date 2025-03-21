cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(RendellUI)

set(BINARY_NAME rendell_ui)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    src/initialization.cpp
    src/Window/Window.cpp
    src/Window/window_input.cpp
    src/Window/window_callbacks.cpp
    src/Viewport.cpp
    src/Time.cpp
    src/Canvas.cpp
    src/Widgets/Widget.cpp
    src/Widgets/Transform2D.cpp
    src/Widgets/RectangleWidget.cpp
    src/Widgets/TextWidget.cpp
    src/Widgets/TextEditWidget.cpp
    src/Widgets/WidgetRegistrator.cpp
    src/Widgets/WidgetHandlePipeline.cpp
    src/Widgets/WidgetRenderPipeline.cpp
    src/Widgets/private/TextEditor.cpp
    src/Widgets/private/TextEditorWord.cpp
    src/Widgets/private/Cursor.cpp
    src/Shaders/Shader.cpp
    src/Shaders/ShaderStorage.cpp
    src/Shaders/RectangleShader.cpp
    src/String/StringBuilder.cpp
    src/String/StringBuilderChunk.cpp
    src/String/StringExtension.cpp
)

set(HEADER_FILES
    src/initialization.h
    src/Window/Window.h
    src/Window/WindowEventHandler.h
    src/Window/window_input.h
    src/Window/window_callbacks.h
    src/Viewport.h
    src/Time.h
    src/Canvas.h
    src/Widgets/Widget.h
    src/Widgets/Transform2D.h
    src/Widgets/RectangleWidget.h
    src/Widgets/TextWidget.h
    src/Widgets/TextEditWidget.h
    src/Widgets/WidgetRegistrator.h
    src/Widgets/WidgetHandlePipeline.h
    src/Widgets/WidgetRenderPipeline.h
    src/Widgets/private/TextEditor.h
    src/Widgets/private/Cursor.h
    src/Widgets/private/TextEditorWord.h
    src/Shaders/Shader.h
    src/Shaders/ShaderStorage.h
    src/Shaders/RectangleShader.h
    src/String/StringBuilder.h
    src/String/StringBuilderChunk.h
    src/String/StringExtension.h

    src/Signal.h
)

add_library(${BINARY_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${BINARY_NAME} PUBLIC include)

target_compile_definitions(${BINARY_NAME}
    PRIVATE
        "FONT_PATH=\"${RENDELL_UI_FONT_PATH}\"">
)

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
