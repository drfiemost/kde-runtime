add_definitions(-DTEST_DATA="${CMAKE_CURRENT_SOURCE_DIR}/")

set(testname testpamopen)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
kde4_add_unit_test(${testname} kwalletexecuter.cpp ${testname}.cpp)
target_link_libraries(${testname} Qt4::QtTest Qt4::QtCore Qt4::QtNetwork ${KDE4_KDECORE_LIBS})

set(testname testpamopennofile)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
kde4_add_unit_test(${testname} kwalletexecuter.cpp ${testname}.cpp)
target_link_libraries(${testname} Qt4::QtTest Qt4::QtCore Qt4::QtNetwork ${KDE4_KDECORE_LIBS})
