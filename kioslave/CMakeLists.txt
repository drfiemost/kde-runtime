
if(NOT WIN32)
# we need a version of samba which has already smbc_set_context(), Alex
set(SAMBA_REQUIRE_SMBC_SET_CONTEXT TRUE)
set(SAMBA_REQUIRE_SMBC_OPTION_SET TRUE)
macro_optional_find_package(Samba)
set_package_properties(Samba PROPERTIES DESCRIPTION "the SMB client library, a version with smbc_set_context() and smbc_option_set()"
                       URL "http://www.samba.org"
                       TYPE OPTIONAL
                       PURPOSE "Needed to build the SMB kioslave"
                      )
endif(NOT WIN32)

macro_optional_find_package(LibSSH 0.6.0)
set_package_properties(LibSSH PROPERTIES DESCRIPTION "the SSH library with SFTP support"
                       URL "http://www.libssh.org/"
                       TYPE OPTIONAL
                       PURPOSE "Needed to build the SFTP kioslave"
                      )

add_subdirectory( about )
add_subdirectory( bookmarks )
add_subdirectory( cgi )
add_subdirectory( filter )
add_subdirectory( info )
add_subdirectory( archive )
add_subdirectory( settings )
add_subdirectory( applications )
add_subdirectory( trash )
if(NOT WIN32)
add_subdirectory( network )
endif(NOT WIN32)
add_subdirectory( remote )
add_subdirectory( desktop )
add_subdirectory( recentdocuments )
add_subdirectory( fish )
add_subdirectory( thumbnail )
add_subdirectory( docfilter )
if (LIBSSH_FOUND)
  add_subdirectory(sftp)
endif (LIBSSH_FOUND)

if(NOT WIN32)
   add_subdirectory( floppy )
   add_subdirectory( finger )
   add_subdirectory( man )
endif(NOT WIN32)

if(SAMBA_FOUND OR WIN32)
    add_subdirectory(smb)
endif(SAMBA_FOUND OR WIN32)

