if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wpedantic
    -DTAGLIB_STATIC
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_library(TAGLIB 3dtarty/libtag)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Quick Sql)


include_directories(
    ./src
    ../build/3dparty/taglib
    ../3dparty/taglib/taglib/ape
    ../3dparty/taglib/taglib/asf
    ../3dparty/taglib/taglib/flac
    ../3dparty/taglib/taglib/it
    ../3dparty/taglib/taglib/mod
    ../3dparty/taglib/taglib/mp4
    ../3dparty/taglib/taglib/mpc
    ../3dparty/taglib/taglib/mpeg
    ../3dparty/taglib/taglib/mpeg/id3v2
    ../3dparty/taglib/taglib/mpeg/id3v1
    ../3dparty/taglib/taglib/mpeg/id3v2/frames
    ../3dparty/taglib/taglib/ogg
    ../3dparty/taglib/taglib/riff
    ../3dparty/taglib/taglib/s3m
    ../3dparty/taglib/taglib/toolkit
    ../3dparty/taglib/taglib/trueaudio
    ../3dparty/taglib/taglib/wavpack
    ../3dparty/taglib/taglib/xm
    ../3dparty/taglib/taglib
    ../3dparty/taglib/
)

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
    #set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/app_win32.rc")

    add_executable(${CMAKE_PROJECT_NAME} main.cpp ${RESOURCES} ${APP_ICON_RESOURCE})
endif()

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    tag
    src
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Sql
)

#target_include_directories(${TARGET} PUBLIC ${PROJECT_BINARY_DIR})
