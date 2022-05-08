project(plasmaextracomponents)

find_package(KActivities QUIET CONFIG)
set_package_properties(KActivities PROPERTIES DESCRIPTION "Library and infrastructure for using Activities from applications"
                       URL "https://projects.kde.org/kactivities"
		       TYPE OPTIONAL
                       PURPOSE "Provides the ability to access activities from QML."
                      )

set(plasmaextracomponents_SRCS
    appbackgroundprovider.cpp
    resourceinstance.cpp
    plasmaextracomponentsplugin.cpp
    fallbackcomponent.cpp
    )

if(KActivities_FOUND)
  add_definitions(-DENABLE_KACTIVITIES)
  include_directories(
        ${KACTIVITIES_INCLUDE_DIRS}
  )
endif(KActivities_FOUND)

set(CMAKE_AUTOMOC ON)

add_library(plasmaextracomponentsplugin SHARED ${plasmaextracomponents_SRCS})
target_link_libraries(plasmaextracomponentsplugin Qt4::QtCore Qt4::QtDeclarative
                                                    Qt4::QtGui ${KDE4_PLASMA_LIBS} )

if(KActivities_FOUND)
  target_link_libraries(plasmaextracomponentsplugin ${KACTIVITIES_LIBRARY} )
endif(KActivities_FOUND)

install(TARGETS plasmaextracomponentsplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/extras)

install(DIRECTORY qml/ DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/extras)


# The platform specific stuff, overwrites a copy of the desktop one
# it does install some files on top of the old ones, has to be done
# file by file since if some component from the generic set is more
# recent than the specifc ones, it wouldn't be overwritten

#install(TARGETS plasmaextracomponentsplugin DESTINATION ${PLUGIN_INSTALL_DIR}/platformimports/touch/org/kde/plasma/extras)
#install(FILES qml/qmldir DESTINATION ${PLUGIN_INSTALL_DIR}/platformimports/touch/org/kde/plasma/extras)

# install(FILES qml/App.qml DESTINATION ${PLUGIN_INSTALL_DIR}/platformimports/touch/org/kde/plasma/extras)
# install(FILES qml/Heading.qml DESTINATION ${PLUGIN_INSTALL_DIR}/platformimports/touch/org/kde/plasma/extras)
# install(FILES qml/Paragraph.qml DESTINATION ${PLUGIN_INSTALL_DIR}/platformimports/touch/org/kde/plasma/extras)
# install(FILES qml/Title.qml DESTINATION ${PLUGIN_INSTALL_DIR}/platformimports/touch/org/kde/plasma/extras)
# install(DIRECTORY qml/animations/ DESTINATION ${PLUGIN_INSTALL_DIR}/platformimports/touch/org/kde/plasma/extras)

#install(DIRECTORY qml/animations/ DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/extras)

#install platform overrides -- enable as we use this feature for plasmaextras
#install(DIRECTORY platformcomponents/touch/ DESTINATION ${PLUGIN_INSTALL_DIR}/platformimports/touch/org/kde/plasma/extras)
