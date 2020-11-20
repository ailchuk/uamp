file(GLOB SOURCE *.cpp *.h *.ui)

add_library(src STATIC ${SOURCE})

target_link_libraries(src Qt${QT_VERSION_MAJOR}::Core
    tag
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
)