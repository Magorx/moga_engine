cmake_minimum_required(VERSION 3.15)
project(moga_engine LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_GRAPHICS true)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result")

include_directories(src
                    src/engine
                    src/visual
                    src/physics
                    src/collections)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "-O3 ${WARNING_FLAGS} -fdeclspec -march=native -fsanitize=undefined") #-fsanitize=address

find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)
find_package(OpenGL)

add_executable(moga

        # utils
        src/utils/vec3d.cpp
        src/utils/timer.cpp

        src/utils/interpolator/catmullrom.cpp

        src/utils/logger.cpp

        src/utils/text_line/line.cpp
        src/utils/text_line/line_pos.cpp
        src/utils/text_line/cursor.cpp
        src/utils/text_line/line_history.cpp

        src/utils/state_history/history.cpp

        # event

        # interface
        src/view/view.cpp

        # engine
        src/engine/object.cpp
        src/engine/moga_engine.cpp

        # devices
        src/device/keyboard.cpp

        # visual
        src/visual/color/color.cpp
        src/visual/color/rgba.cpp
        src/visual/color/smart_color.cpp
        src/visual/renderer/renderer.cpp
        src/visual/animation/animation.cpp

        src/visual/renderer/appearence/texture.cpp
        src/visual/renderer/appearence/animation.cpp

        # physics
        src/physics/physics_engine.cpp
        src/physics/solid_body/solid_body.cpp

        src/physics/collision/coll_circle_circle.cpp
        src/physics/collision/coll_circle_line.cpp
        
        src/physics/solid_body/sb_circle.cpp
        src/physics/solid_body/sb_line.cpp

        # collections
        src/collections/smart_colors/smart_colors.cpp
        src/collections/renderables/renderables.cpp
        src/collections/objects/objects.cpp

        # styles

        src/collections/styles/button.cpp
        src/collections/styles/util_bar.cpp
        src/collections/styles/window.cpp
        src/collections/styles/text.cpp

        # views

        src/collections/views/button.cpp
        src/collections/views/toggler.cpp
        src/collections/views/highlighter.cpp
        src/collections/views/mouse_catcher.cpp
        src/collections/views/vertical_layout.cpp
        src/collections/views/horizontal_layout.cpp
        src/collections/views/text.cpp
        src/collections/views/hideable.cpp
        src/collections/views/menu.cpp
        src/collections/views/submenu.cpp
        src/collections/views/labeled_view.cpp
        src/collections/views/repeater.cpp
        src/collections/views/magnetic.cpp
        src/collections/views/utility_tab.cpp
        src/collections/views/window.cpp
        src/collections/views/dialog_window.cpp

        src/collections/views/text_field.cpp
        src/collections/views/spline.cpp

        # redactor views

        src/collections/redactor_views/canvas.cpp
        src/collections/redactor_views/color_picker.cpp


        src/collections/actions/toggle.cpp

        # effects

        src/collections/effects/rgb_splined.cpp

        # components
        src/visual/visual_engine.cpp

        src/engine/tickable.h

        # resources
        src/engine/Resources.cpp

        # redactor

        src/redactor/tools/brush.cpp
        src/redactor/tools/color_pipette.cpp
        src/redactor/tools/tool_manager.cpp
        src/redactor/canvas.cpp

        src/redactor/layer/layer.cpp

        src/redactor/engine.cpp

        # chemistry
        # src/chemistry/chem_engine.cpp
        # src/chemistry/chem_reactions.cpp

        src/main.cpp
        )

target_link_libraries(moga sfml-graphics sfml-window sfml-system)
target_link_libraries(moga ${OPENGL_LIBRARIES})

# add_custom_target(rebuild_plugins
#         COMMAND convert image.ppm image.png
#         )
