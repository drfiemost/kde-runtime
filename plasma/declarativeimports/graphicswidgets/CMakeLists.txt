project(graphicswidgetsbindings)

set(graphicswidgetsbindings_SRCS
    graphicswidgetsbindingsplugin.cpp
    declarativetabbar.cpp
    )

INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${KDE4_INCLUDES}
)

set(CMAKE_AUTOMOC ON)

add_library(graphicswidgetsbindingsplugin SHARED ${graphicswidgetsbindings_SRCS})
target_link_libraries(graphicswidgetsbindingsplugin ${KDE4_PLASMA_LIBS} Qt4::QtScript Qt4::QtDeclarative)

install(TARGETS graphicswidgetsbindingsplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/graphicswidgets)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/graphicswidgets)
