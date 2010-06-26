project(MParser)
FIND_PACKAGE(Qt4 REQUIRED)
set(MPARSER_SRCS
      	mparser.cpp
	mdocumentnode.cpp
	mupdatedbthread.cpp
    )

SET(QT_USE_QTSQL 1)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS( -lgdbm )

kde4_add_library(MParser ${MPARSER_SRCS})

#kde4_add_library(MProcessShellWidget MProcessShellWidget.cpp)
target_link_libraries(MParser ${QT_LIBRARIES} gdbm)

#install(TARGETS editor DESTINATION ${BIN_INSTALL_DIR})
#install(FILES editorui.rc
#DESTINATION ${DATA_INSTALL_DIR}/editor)
