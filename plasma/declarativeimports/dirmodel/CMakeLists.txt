project(dirmodel)

set(dirmodel_SRCS
    dirmodel.cpp
    dirmodelplugin.cpp
    )

set(CMAKE_AUTOMOC ON)

kde4_add_library(dirmodelplugin SHARED ${dirmodel_SRCS})
target_link_libraries(dirmodelplugin
        Qt4::QtCore
        Qt4::QtDeclarative
        ${KDE4_KIO_LIBS}
        )

install(TARGETS dirmodelplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/dirmodel)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/dirmodel)

#add_subdirectory(test)
