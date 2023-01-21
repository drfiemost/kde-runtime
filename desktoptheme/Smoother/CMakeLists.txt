
install(FILES colors metadata.desktop DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/)

FILE(GLOB widgets widgets/*.svgz)
install( FILES ${widgets} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/widgets/ )

FILE(GLOB weatherstation weatherstation/*.svgz)
install( FILES ${weatherstation} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/weatherstation/ )

FILE(GLOB translucent translucent/widgets/*.svgz)
install( FILES ${translucent} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/translucent/widgets/ )

FILE(GLOB translucent translucent/dialogs/*.svgz)
install( FILES ${translucent} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/translucent/dialogs/ )

FILE(GLOB toolbar-icons toolbar-icons/*.svgz)
install( FILES ${toolbar-icons} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/toolbar-icons/ )

FILE(GLOB stylesheets stylesheets/*.css)
install( FILES ${stylesheets} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/stylesheets/ )

FILE(GLOB rssnow rssnow/*.svg)
install( FILES ${rssnow} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/rssnow/ )

FILE(GLOB opaque opaque/widgets/*.svgz)
install( FILES ${opaque} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/opaque/widgets/ )

FILE(GLOB opaque opaque/dialogs/*.svgz)
install( FILES ${opaque} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/opaque/dialogs/ )

FILE(GLOB lancelot lancelot/*)
install( FILES ${lancelot} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/lancelot/ )

FILE(GLOB icons icons/*.svgz)
install( FILES ${icons} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/icons/ )

FILE(GLOB dialogs dialogs/*.svgz)
install( FILES ${dialogs} DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/Smoother5/dialogs/ )
