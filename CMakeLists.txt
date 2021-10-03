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
set(CMAKE_CXX_FLAGS   "-O3 ${WARNING_FLAGS} -fdeclspec -march=native") #-fsanitize=address

find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)

add_executable(moga
        src/main.cpp

        # interface
        src/view/view.cpp

        # chemistry
        src/chemistry/chem_engine.cpp
        src/chemistry/chem_reactions.cpp

        # engine
        src/engine/object.cpp
        src/engine/moga_engine.cpp

        # visual
        src/visual/color/color.cpp
        src/visual/color/rgba.cpp
        src/visual/color/smart_color.cpp
        src/visual/renderer/renderer.cpp

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

        src/collections/views/button.cpp
        src/collections/views/toggler.cpp

        # components
        src/visual/visual_engine.cpp

        src/engine/tickable.h

        # utils
        src/utils/vec3d.cpp
        )

target_link_libraries(moga sfml-graphics sfml-window sfml-system)        

add_custom_target(convert
        COMMAND convert image.ppm image.png
        )

add_custom_target(view
        COMMAND eog -f image.png
        )

