project(graphicslayoutsbindings)

set(graphicslayoutsbindings_SRCS
    graphicslayoutsbindingsplugin.cpp
    gridlayout.cpp
    linearlayout.cpp
    )

INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${KDE4_INCLUDES}
)

set(CMAKE_AUTOMOC ON)

add_library(graphicslayoutsbindingsplugin SHARED ${graphicslayoutsbindings_SRCS})
target_link_libraries(graphicslayoutsbindingsplugin Qt4::QtCore Qt4::QtGui Qt4::QtScript Qt4::QtDeclarative)

install(TARGETS graphicslayoutsbindingsplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/graphicslayouts)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/graphicslayouts)
