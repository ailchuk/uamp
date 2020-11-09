set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wpedantic
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets Core Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Gui REQUIRED)

include_directories(src)

add_subdirectory(src)

qt5_add_resources(RESOURCES resources/myres.qrc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../)

#Set app icon
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX_BUNDLE_ICON_FILE utext_icon.icns)
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/resources/utext_icon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
               MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${APP_ICON_MACOSX} main.cpp ${RESOURCES})
else()
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/app_win32.rc")

    add_executable(${CMAKE_PROJECT_NAME} main.cpp ${RESOURCES} ${APP_ICON_RESOURCE})
endif()


target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    src
)