cmake_minimum_required(VERSION 3.17.0)
project(MiniJam150 VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

set(MiniJam150_VERSION "0.1a")

if(WIN32)
    set(SDL2_INCLUDE_DIR "C:/Program Files/mingw64/include/SDL2")
    set(SDL2_LIBRARY_DIR "C:/Program Files/mingw64/lib")
    set(SDL2_IMAGE_INCLUDE_DIR "C:/SDL_image/include/SDL2")
    set(SDL2_IMAGE_LIBRARY_DIR "C:/SDL_image/lib")
    set(SDL2_TTF_INCLUDE_DIR "C:/SDL_ttf/include/SDL2")
    set(SDL2_TTF_LIBRARY_DIR "C:/SDL_ttf/lib")
elseif(APPLE)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2TTF REQUIRED)
endif()

set(TMXLITE_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib/tmxlite")

set(SOURCE_INCLUDES
    ${TMXLITE_LIBRARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/lib/nlohmann
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Dialogue
    ${PROJECT_SOURCE_DIR}/src/Engine
    ${PROJECT_SOURCE_DIR}/src/Engine/Audio
    ${PROJECT_SOURCE_DIR}/src/Engine/RandomUtilities
    ${PROJECT_SOURCE_DIR}/src/Engine/StandardTypes
    ${PROJECT_SOURCE_DIR}/src/Entity
    ${PROJECT_SOURCE_DIR}/src/Entity/Components
    ${PROJECT_SOURCE_DIR}/src/Entity/Components/Generic
    ${PROJECT_SOURCE_DIR}/src/Entity/Components/Input
    ${PROJECT_SOURCE_DIR}/src/Entity/Components/Physics
    ${PROJECT_SOURCE_DIR}/src/Entity/Components/Render
    ${PROJECT_SOURCE_DIR}/src/Entity/Components/Specialized
    ${PROJECT_SOURCE_DIR}/src/Entity/Components/Trigger
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Enemies
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Environment
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Items
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Triggers
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/TypeEnums
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Generic
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Input
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Physics
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Render
    ${PROJECT_SOURCE_DIR}/src/GUI
    ${PROJECT_SOURCE_DIR}/src/Input
    ${PROJECT_SOURCE_DIR}/src/Level
    ${PROJECT_SOURCE_DIR}/src/LightMap
    ${PROJECT_SOURCE_DIR}/src/Localization
    ${PROJECT_SOURCE_DIR}/src/Render
    ${PROJECT_SOURCE_DIR}/src/Render/Effects
    ${PROJECT_SOURCE_DIR}/src/States
    )
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Dialogue/DialogueParser.cpp
    ${PROJECT_SOURCE_DIR}/src/Dialogue/DialogueBox.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/FileIO.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/Game.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/Settings.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/Timer.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/Audio/Audio.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/RandomUtilities/RandomGen.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/RandomUtilities/RDSNullValue.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/RandomUtilities/RDSObject.cpp
    ${PROJECT_SOURCE_DIR}/src/Engine/RandomUtilities/RDSTable.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Components/Render/SpritesheetPropertiesComponent.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/ParticleEffect.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Player.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Enemies/Slug.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Environment/Beacon.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Environment/Mineral.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Environment/Torch.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Items/ItemPickup.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Triggers/DialogueTrigger.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Prefabs/Triggers/PrefabSpawnTrigger.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Generic/InventorySystem.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Generic/ScriptSystem.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Input/InputSystem.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Physics/CollisionSystem.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Physics/PhysicsSystem.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Render/CameraSystem.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Render/LightSystem.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity/Systems/Render/RenderSystem.cpp
    ${PROJECT_SOURCE_DIR}/src/GUI/BlankElement.cpp
    ${PROJECT_SOURCE_DIR}/src/GUI/Button.cpp
    ${PROJECT_SOURCE_DIR}/src/GUI/GUI.cpp
    ${PROJECT_SOURCE_DIR}/src/GUI/GUIElement.cpp
    ${PROJECT_SOURCE_DIR}/src/GUI/TextContainer.cpp
    ${PROJECT_SOURCE_DIR}/src/Input/Controller.cpp
    ${PROJECT_SOURCE_DIR}/src/Input/Keyboard.cpp
    ${PROJECT_SOURCE_DIR}/src/Input/Mouse.cpp
    ${PROJECT_SOURCE_DIR}/src/Level/Level.cpp
    ${PROJECT_SOURCE_DIR}/src/Level/LevelParser.cpp
    ${PROJECT_SOURCE_DIR}/src/LightMap/FloatingPointLightMap.cpp
    ${PROJECT_SOURCE_DIR}/src/Render/Spritesheet.cpp
    ${PROJECT_SOURCE_DIR}/src/Render/Text.cpp
    ${PROJECT_SOURCE_DIR}/src/Render/Effects/ScreenShake.cpp
    ${PROJECT_SOURCE_DIR}/src/States/GameState.cpp
    ${PROJECT_SOURCE_DIR}/src/States/State.cpp
    )
    
if(WIN32)
    set(TMX_SOURCES
        ${TMXLITE_LIBRARY_DIR}/src/detail/pugixml.cpp
        ${TMXLITE_LIBRARY_DIR}/src/FreeFuncs.cpp
        ${TMXLITE_LIBRARY_DIR}/src/ImageLayer.cpp
        ${TMXLITE_LIBRARY_DIR}/src/Map.cpp
        ${TMXLITE_LIBRARY_DIR}/src/miniz.c
        ${TMXLITE_LIBRARY_DIR}/src/Object.cpp
        ${TMXLITE_LIBRARY_DIR}/src/ObjectGroup.cpp
        ${TMXLITE_LIBRARY_DIR}/src/Property.cpp
        ${TMXLITE_LIBRARY_DIR}/src/TileLayer.cpp
        ${TMXLITE_LIBRARY_DIR}/src/LayerGroup.cpp
        ${TMXLITE_LIBRARY_DIR}/src/Tileset.cpp
        ${TMXLITE_LIBRARY_DIR}/src/ObjectTypes.cpp
        )
endif()

if(WIN32)
    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SOURCE_INCLUDES})
    add_executable(MiniJam150 ${SOURCES} ${TMX_SOURCES})
    # remove -mconsole for release builds
    target_link_libraries(MiniJam150 -lmingw32 ${SDL2_LIBRARY_DIR}/libSDL2main.a ${SDL2_LIBRARY_DIR}/libSDL2.dll.a ${SDL2_IMAGE_LIBRARY_DIR}/libSDL2_image.dll.a ${SDL2_TTF_LIBRARY_DIR}/libSDL2_ttf.dll.a -mwindows)
    add_custom_command(TARGET MiniJam150 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:MiniJam150>/res/)
    add_custom_command(TARGET MiniJam150 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/settings.cfg $<TARGET_FILE_DIR:MiniJam150>/settings.cfg)
elseif(APPLE)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR} ${SOURCE_INCLUDES})
    add_executable(MiniJam150 MACOSX_BUNDLE ${SOURCES})
    target_link_libraries(MiniJam150 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${TMXLITE_LIBRARY_DIR}/libtmxlite.a)
    set_target_properties(MiniJam150 PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_EXECUTABLE_NAME MiniJam150
        MACOSX_BUNDLE_ICON_FILE icon.png
        MACOSX_BUNDLE_GUI_IDENTIFIER com.straub.MiniJam150
        MACOSX_BUNDLE_LONG_VERSION_STRING ${MiniJam150_VERSION}
        MACOSX_BUNDLE_BUNDLE_NAME MiniJam150
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${MiniJam150_VERSION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${MiniJam150_VERSION}
        MACOSX_BUNDLE_COPYRIGHT "© Tyler Straub 2022"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Info.plist.in)
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"${CMAKE_BINARY_DIR}/MiniJam150.app\"   \"\"   \"${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY}\")
        " COMPONENT Runtime)
    add_custom_command(TARGET MiniJam150 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:MiniJam150>/../Resources/res/)
    add_custom_command(TARGET MiniJam150 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/settings.cfg $<TARGET_FILE_DIR:MiniJam150>/../Resources/settings.cfg)
endif()

target_include_directories(MiniJam150 PRIVATE ${CMAKE_SOURCE_DIR}/lib/entt/src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
