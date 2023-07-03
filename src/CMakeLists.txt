set(PROJECT_SOURCES
        main.cpp
	mainwindow.cpp
	mainwindow.h
	mainwindow.ui
	graphicsscene.cpp
	graphicsscene.h
	graphicsview.cpp
	graphicsview.h
	hist.cpp
	hist.h
	rout_scheme.cpp
	rout_scheme.h
	rout_tab.cpp
	rout_tab.h
	short_path.cpp
	short_path.h
)

qt_add_executable(cmake_import
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(cmake_import PRIVATE Qt6::Widgets)

target_include_directories(cmake_import PRIVATE ${BOOST_INCLUDE_DIRS})

set_target_properties(cmake_import PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

install(TARGETS cmake_import
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_finalize_executable(cmake_import)
