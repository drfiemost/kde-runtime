project(runtime_kimgio)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

##################################
# WebP image IO support

find_library(FOUND_WEBP webp)
if ( FOUND_WEBP )
  set(kimg_webp_LIB_SRCS webp.cpp)
  kde4_add_plugin(kimg_webp ${kimg_webp_LIB_SRCS})
  target_link_libraries(kimg_webp ${KDE4_KDECORE_LIBS} Qt4::QtGui webp)

  add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
  add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

  include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

  install(TARGETS kimg_webp  DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/imageformats/)
  install(FILES webp.desktop DESTINATION ${SERVICES_INSTALL_DIR}/qimageioplugins/)

  find_package(SharedMimeInfo REQUIRED)
  install(FILES webp.xml DESTINATION ${XDG_MIME_INSTALL_DIR})
  update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})
endif()

