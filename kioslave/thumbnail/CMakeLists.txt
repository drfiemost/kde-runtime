macro_optional_find_package(OpenEXR)
set_package_properties(OpenEXR PROPERTIES DESCRIPTION "API for accessing OpenEXR formatted images"
                       URL "http://www.openexr.com"
                       TYPE OPTIONAL
                       PURPOSE "Provides support for OpenEXR formatted images in the thumbnail kioslave"
                      )

find_package(JPEG)
set_package_properties(JPEG PROPERTIES DESCRIPTION "JPEG image format support"
                       URL "http://www.ijg.org"
                       TYPE REQUIRED
                      )

macro_optional_find_package(Exiv2)
set_package_properties(Exiv2 PROPERTIES DESCRIPTION "A library to access image metadata"
                       URL "http://www.exiv2.org"
                       TYPE OPTIONAL
                       PURPOSE "Provides support for automatic rotation of JPEGs in the thumbnail kioslave"
                      )

macro_bool_to_01(EXIV2_FOUND HAVE_EXIV2)

include_directories(${JPEG_INCLUDE_DIR})
if(EXIV2_FOUND)
    include_directories(${EXIV2_INCLUDE_DIR})
endif(EXIV2_FOUND)

########### next target ###############

set(kio_thumbnail_PART_SRCS thumbnail.cpp imagefilter.cpp)

kde4_add_plugin(kio_thumbnail ${kio_thumbnail_PART_SRCS})

target_link_libraries(kio_thumbnail ${KDE4_KIO_LIBS})

install(TARGETS kio_thumbnail DESTINATION ${PLUGIN_INSTALL_DIR})

########### next target ###############

set(imagethumbnail_PART_SRCS imagecreator.cpp)

kde4_add_plugin(imagethumbnail ${imagethumbnail_PART_SRCS})

target_link_libraries(imagethumbnail ${KDE4_KIO_LIBS})

install(TARGETS imagethumbnail DESTINATION ${PLUGIN_INSTALL_DIR})

########### next target ###############

install( FILES directorythumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})

########### next target ###############

set(jpegthumbnail_PART_SRCS jpegcreator.cpp)
kde4_add_kcfg_files(jpegthumbnail_PART_SRCS jpegcreatorsettings.kcfgc)
kde4_add_plugin(jpegthumbnail ${jpegthumbnail_PART_SRCS})

target_link_libraries(jpegthumbnail Qt4::QtCore Qt4::QtGui ${JPEG_LIBRARIES} ${KDE4_KIO_LIBS})

if (HAVE_EXIV2)
    set_source_files_properties(jpegcreator.cpp PROPERTIES
                                COMPILE_FLAGS "-DHAVE_EXIV2 ${KDE4_ENABLE_EXCEPTIONS}")
    include_directories(${EXIV1_INCLUDE_DIR})
    target_link_libraries(jpegthumbnail ${EXIV2_LIBRARIES})
endif (HAVE_EXIV2)

install(FILES jpegcreatorsettings.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(TARGETS jpegthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})

########### next target ###############

set(svgthumbnail_PART_SRCS svgcreator.cpp)

kde4_add_plugin(svgthumbnail ${svgthumbnail_PART_SRCS})

target_link_libraries(svgthumbnail ${KDE4_KIO_LIBS})

install(TARGETS svgthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})

########### next target ###############

set(textthumbnail_PART_SRCS textcreator.cpp)

kde4_add_plugin(textthumbnail ${textthumbnail_PART_SRCS})

target_link_libraries(textthumbnail ${KDE4_KIO_LIBS})

install(TARGETS textthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})

########### next target ###############

if(QT_QTWEBKIT_FOUND)
set(htmlthumbnail_PART_SRCS htmlcreator.cpp)

kde4_add_plugin(htmlthumbnail ${htmlthumbnail_PART_SRCS})

target_link_libraries(htmlthumbnail ${KDE4_KIO_LIBS} Qt4::QtWebKit)

install(TARGETS htmlthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})
endif(QT_QTWEBKIT_FOUND)

########### next target ###############

if(NOT WIN32)
set(djvuthumbnail_PART_SRCS djvucreator.cpp)

kde4_add_plugin(djvuthumbnail ${djvuthumbnail_PART_SRCS})

target_link_libraries(djvuthumbnail ${KDE4_KIO_LIBS})

install(TARGETS djvuthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})
endif(NOT WIN32)

########### next target ###############

if(OPENEXR_FOUND)

   include_directories( ${OPENEXR_INCLUDE_DIR})

   set(exrthumbnail_PART_SRCS exrcreator.cpp)

   kde4_add_plugin(exrthumbnail ${exrthumbnail_PART_SRCS})

   target_link_libraries(exrthumbnail ${KDE4_KIO_LIBS} ${OPENEXR_LIBRARIES})

   # OpenEXR headers use exceptions; at least clang refuses to build the target
   # when exceptions are not enabled.
   set_target_properties(exrthumbnail PROPERTIES COMPILE_FLAGS "${KDE4_ENABLE_EXCEPTIONS}")

   install(TARGETS exrthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})
   install( FILES exrthumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})

endif(OPENEXR_FOUND)

########### next target ###############

if(X11_Xcursor_FOUND)

   set(cursorthumbnail_PART_SRCS cursorcreator.cpp)

   kde4_add_plugin(cursorthumbnail ${cursorthumbnail_PART_SRCS})

   target_link_libraries(cursorthumbnail ${X11_Xcursor_LIB} ${KDE4_KIO_LIBS})

   install(TARGETS cursorthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})
   install( FILES cursorthumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})

endif(X11_Xcursor_FOUND)

########### next target ###############

set ( windowsexethumbnail_SRCS windowsexecreator.cpp icoutils_common.cpp)
set ( windowsimagethumbnail_SRCS windowsimagecreator.cpp icoutils_common.cpp)

if(WIN32)
    set ( windowsexethumbnail_SRCS ${windowsexethumbnail_SRCS} icoutils_win.cpp )
    set ( windowsimagethumbnail_SRCS ${windowsimagethumbnail_SRCS} icoutils_win.cpp )
else()
    set ( windowsexethumbnail_SRCS ${windowsexethumbnail_SRCS} icoutils_wrestool.cpp )
    set ( windowsimagethumbnail_SRCS ${windowsimagethumbnail_SRCS} icoutils_wrestool.cpp )
endif()

kde4_add_plugin ( windowsexethumbnail ${windowsexethumbnail_SRCS} )
target_link_libraries ( windowsexethumbnail ${KDE4_KIO_LIBS} )
install ( TARGETS windowsexethumbnail DESTINATION ${PLUGIN_INSTALL_DIR} )
install ( FILES windowsexethumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR} )

kde4_add_plugin ( windowsimagethumbnail ${windowsimagethumbnail_SRCS} )
target_link_libraries ( windowsimagethumbnail ${KDE4_KIO_LIBS} )
install ( TARGETS windowsimagethumbnail DESTINATION ${PLUGIN_INSTALL_DIR} )
install ( FILES windowsimagethumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR} )

########### next target ###############

set(comicbookthumbnail_SRCS comiccreator.cpp)
kde4_add_plugin(comicbookthumbnail ${comicbookthumbnail_SRCS})
target_link_libraries(comicbookthumbnail ${KDE4_KIO_LIBS})
if (UNIX)
   target_link_libraries(comicbookthumbnail ${KDE4_KPTY_LIBRARY})
endif (UNIX)
install(TARGETS comicbookthumbnail DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install(FILES thumbcreator.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES
    thumbnail.protocol
    svgthumbnail.desktop
    imagethumbnail.desktop
    jpegthumbnail.desktop
    textthumbnail.desktop
    htmlthumbnail.desktop
    djvuthumbnail.desktop
    desktopthumbnail.desktop
    comicbookthumbnail.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})
