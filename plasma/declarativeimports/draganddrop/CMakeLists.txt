project(draganddrop)

include(KDE4Defaults)

set(declarativedragdrop_SRCS
    draganddropplugin.cpp
    DeclarativeDragArea.cpp
    DeclarativeDragDropEvent.cpp
    DeclarativeDropArea.cpp
    DeclarativeMimeData.cpp
    )

INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${KDE4_INCLUDES}
)

set(CMAKE_AUTOMOC ON)

kde4_add_library(draganddropplugin SHARED ${declarativedragdrop_SRCS})
target_link_libraries(draganddropplugin Qt4::QtCore Qt4::QtGui Qt4::QtDeclarative)

install(TARGETS draganddropplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/draganddrop)

install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/draganddrop)
