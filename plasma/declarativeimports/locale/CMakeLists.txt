project(localebindings)

include(KDE4Defaults)

set(localebindings_SRCS
    locale.cpp
    localebindingsplugin.cpp
    calendarsystem.cpp
)

INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${KDE4_INCLUDES}
)

set(CMAKE_AUTOMOC ON)

kde4_add_library(localebindingsplugin SHARED ${localebindings_SRCS})

target_link_libraries(localebindingsplugin Qt4::QtDeclarative ${KDE4_KDECORE_LIBRARY})

install(TARGETS localebindingsplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/locale)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/locale)
