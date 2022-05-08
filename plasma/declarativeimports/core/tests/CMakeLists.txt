INCLUDE_DIRECTORIES(
    ${corebindings_SOURCE_DIR}
    )

kde4_add_unit_test(sortfiltermodeltest
    sortfiltermodeltest.cpp
    ${corebindings_SOURCE_DIR}/dataengineconsumer.cpp
    ${corebindings_SOURCE_DIR}/datamodel.cpp
    ${corebindings_SOURCE_DIR}/datasource.cpp
    )

qt4_automoc(sortfiltermodeltest.cpp)

target_link_libraries(sortfiltermodeltest
    ${KDE4_PLASMA_LIBS}
    Qt4::QtGui
    Qt4::QtTest
    )

set(runnermodeltest_SRCS
    main.cpp
    dynamictreemodel.cpp
    modeltest.cpp
    ../runnermodel.cpp
    )

set(CMAKE_AUTOMOC ON)

#kde4_add_library(datamodelsplugin SHARED ${datamodels_SRCS})
kde4_add_executable(runnermodeltest ${runnermodeltest_SRCS})
target_link_libraries(runnermodeltest Qt4::QtTest ${KDE4_PLASMA_LIBS})
