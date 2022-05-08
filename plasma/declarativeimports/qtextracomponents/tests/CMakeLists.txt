INCLUDE_DIRECTORIES(.. .)

kde4_add_unit_test(fullmodelaccesstest columnproxymodeltest.cpp ../columnproxymodel.cpp ../../core/tests/modeltest.cpp)
target_link_libraries(fullmodelaccesstest
    Qt4::QtCore
    Qt4::QtGui
    Qt4::QtTest
    ${KDE4_KDECORE_LIBS}
    )
