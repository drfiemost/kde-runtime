add_subdirectory(bookmarks)
add_subdirectory(bzip2)
add_subdirectory(cgi)
add_subdirectory(finger)
add_subdirectory(fish)
add_subdirectory(floppy)
add_subdirectory(gzip)
add_subdirectory(info)
add_subdirectory(man)
add_subdirectory(network)
add_subdirectory(nfs)
add_subdirectory(sftp)
if(NOT WIN32)
add_subdirectory(smb)
endif(NOT WIN32)
add_subdirectory(tar)
add_subdirectory(thumbnail)
add_subdirectory(xz)
