project(qtextracomponents)

include(KDE4Defaults)

add_subdirectory(tests)

set(qtextracomponents_SRCS
    qtextracomponentsplugin.cpp
    qpixmapitem.cpp
    qimageitem.cpp
    qiconitem.cpp
    mouseeventlistener.cpp
    columnproxymodel.cpp
    )

INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${KDE4_INCLUDES}
)

set(CMAKE_AUTOMOC ON)

add_library(qtextracomponentsplugin SHARED ${qtextracomponents_SRCS})
target_link_libraries(qtextracomponentsplugin ${KDE4_KDEUI_LIBS} Qt4::QtCore Qt4::QtGui Qt4::QtDeclarative)

install(TARGETS qtextracomponentsplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/qtextracomponents)

install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/qtextracomponents)
