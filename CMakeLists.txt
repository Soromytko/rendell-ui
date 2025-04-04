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
    src/Widgets/AnchorableWidget.cpp
    src/Widgets/Widget.cpp
    src/Widgets/Transform2D.cpp
    src/Widgets/RectangleWidget.cpp
    src/Widgets/TextWidget.cpp
    src/Widgets/TextEditWidget.cpp
    src/Widgets/WidgetRegistrator.cpp
    src/Widgets/WidgetHandlePipeline.cpp
    src/Widgets/WidgetRenderPipeline.cpp
    src/Widgets/ScrollbarWidget.cpp
    src/Widgets/private/TextEditor.cpp
    src/Widgets/private/TextEditorWord.cpp
    src/Widgets/private/TextDrawer.cpp
    src/Widgets/private/Cursor.cpp
    src/Shaders/Shader.cpp
    src/Shaders/ShaderStorage.cpp
    src/Shaders/RectangleShader.cpp
    src/String/StringBuilder.cpp
    src/String/StringBuilderChunk.cpp
    src/String/StringExtension.cpp
)

set(HEADER_FILES
    include/rendell_ui/rendell_ui.h
    include/rendell_ui/defines.h
    include/rendell_ui/Signal.h
    include/rendell_ui/initialization.h
    include/rendell_ui/Canvas.h
    include/rendell_ui/Viewport.h

    #Window
    include/rendell_ui/Window/Window.h
    include/rendell_ui/Window/WindowEventHandler.h
    include/rendell_ui/Window/window_input.h

    #Widgets
    include/rendell_ui/Widgets/Anchor.h
    include/rendell_ui/Widgets/IWidget.h
    include/rendell_ui/Widgets/AnchorableWidget.h
    include/rendell_ui/Widgets/Widget.h
    include/rendell_ui/Widgets/RectangleWidget.h
    include/rendell_ui/Widgets/TextWidget.h
    include/rendell_ui/Widgets/TextEditWidget.h
    include/rendell_ui/Widgets/IScrollableWidget.h
    include/rendell_ui/Widgets/ScrollbarWidget.h
    include/rendell_ui/Widgets/private/Cursor.h
    include/rendell_ui/Widgets/private/TextEditor.h
    include/rendell_ui/Widgets/private/TextEditorWord.h
    include/rendell_ui/Widgets/private/TextDrawer.h

    src/Window/window_callbacks.h
    src/Time.h
    src/Widgets/Transform2D.h
    src/Widgets/WidgetRegistrator.h
    src/Widgets/WidgetHandlePipeline.h
    src/Widgets/WidgetRenderPipeline.h
    src/Shaders/Shader.h
    src/Shaders/ShaderStorage.h
    src/Shaders/RectangleShader.h
    src/String/StringBuilder.h
    src/String/StringBuilderChunk.h
    src/String/StringExtension.h
)

include_directories(include)
add_library(${BINARY_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${BINARY_NAME} PUBLIC include)

target_compile_definitions(${BINARY_NAME}
    PRIVATE
        "FONT_ROOT_DIRECTORY=\"${RENDELL_UI_FONT_ROOT_DIRECTORY}\""
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
