project(runnermodel)

set(runnermodel_SRCS
    ../core/runnermodel.cpp
    runnermodelplugin.cpp
    )

set(CMAKE_AUTOMOC ON)

kde4_add_library(runnermodelplugin SHARED ${runnermodel_SRCS})
target_link_libraries(runnermodelplugin
        Qt4::QtCore
        Qt4::QtDeclarative
        ${KDE4_PLASMA_LIBS}
        )

install(TARGETS runnermodelplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/runnermodel)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/runnermodel)

