cmake_minimum_required(VERSION 3.12)
project(KDEBASE_RUNTIME)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options("$<$<CONFIG:DEBUG>:-Wall;-Wextra;-pedantic>")
endif()

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

include(CTest)
include(CTestConfig.cmake)
include(CheckIncludeFiles)
include(CheckFunctionExists)

#search packages used by KDE
find_package(KDE4 4.9.95 REQUIRED)
include(KDE4Defaults)

find_package(KDeclarative QUIET CONFIG)
set_package_properties(KDeclarative PROPERTIES DESCRIPTION "KDE Declarative (QML) support from kdelibs"
                       URL "http://www.kde.org"
                       TYPE REQUIRED
                       PURPOSE "Required by corebindingsplugin (org.kde.plasma.core)"
                      )

find_package(LibGcrypt 1.5.0 REQUIRED QUIET)
set_package_properties(LibGcrypt PROPERTIES DESCRIPTION "Libgcrypt is a general purpose cryptographic library based on the code from GnuPG."
                       URL "http://www.gnu.org/software/libgcrypt/"
                       TYPE REQUIRED
                       PURPOSE "kwalletd needs libgcrypt to perform PBKDF2-SHA512 hashing"
                      )

check_include_files(sys/wait.h HAVE_SYS_WAIT_H)
check_include_files(sys/time.h HAVE_SYS_TIME_H)

configure_file (config-runtime.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-runtime.h )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DKDE_NO_DEPRECATED)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

# Build infrastructure
add_subdirectory(cmake)

# Background processes
add_subdirectory(kpasswdserver)
add_subdirectory(kdontchangethehostname)
macro_optional_add_subdirectory(kglobalaccel)
add_subdirectory(knotify)
add_subdirectory(ktimezoned)

macro_optional_add_subdirectory(kuiserver)
add_subdirectory(kwalletd)

if ( UNIX )
   add_subdirectory(soliduiserver)
   add_subdirectory(solidautoeject)
   add_subdirectory(solid-device-automounter)
   add_subdirectory(solid-networkstatus)
endif ( UNIX )

# Plugins/components required for basic utility
add_subdirectory(kcontrol)
add_subdirectory(kioslave)
add_subdirectory(kurifilter-plugins)
add_subdirectory(phonon)
add_subdirectory(kimgio)
macro_optional_add_subdirectory(renamedlgplugins)

# UI Helper applications
add_subdirectory(kcmshell)
add_subdirectory(kdebugdialog)
macro_optional_add_subdirectory(khelpcenter)
add_subdirectory(knetattach)
macro_optional_add_subdirectory(keditfiletype)
if ( UNIX )
   add_subdirectory(kdesu)
endif ( UNIX )

# Command-line tools (e.g. for shell scripts)
if ( UNIX )
   add_subdirectory(kdeeject)
endif ( UNIX )
add_subdirectory(kfile)
add_subdirectory(kiconfinder)
add_subdirectory(kioclient)
add_subdirectory(kioexec)
add_subdirectory(ktraderclient)
add_subdirectory(kreadconfig)
add_subdirectory(kmimetypefinder)
if (Q_WS_X11)
  add_subdirectory(kstart)
endif (Q_WS_X11)
add_subdirectory(kquitapp)
add_subdirectory(menu)
add_subdirectory(plasma)
add_subdirectory(solid-hardware)

# Platform-specific integration
add_subdirectory(platforms)

# Default settings, content and config
add_subdirectory(l10n)
add_subdirectory(localization)
add_subdirectory(kde-menu)
macro_optional_add_subdirectory(doc)

# Artwork
add_subdirectory(desktoptheme)
add_subdirectory(pics)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

########### install files ###############
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/kde4 DESTINATION ${BIN_INSTALL_DIR})

