set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. ${KDE4_INCLUDES} )

########### filetypestest ###############

kde4_add_unit_test(filetypestest 
 filetypestest.cpp
 ../mimetypedata.cpp
 ../mimetypewriter.cpp
 ../sharedmimeinfoversion.cpp
)
target_link_libraries(filetypestest ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBRARY} ${KDE4_KDEUI_LIBRARIES}
                      Qt4::QtCore Qt4::QtGui Qt4::QtTest)
