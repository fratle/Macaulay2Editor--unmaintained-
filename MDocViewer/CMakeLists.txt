project(mdocviewer)
FIND_PACKAGE(Qt4 REQUIRED)
set(mdocviewer_SRCS
      	mdocviewer.cpp
    )

#kde4_automoc( ${mprocess_SRCS} )
#qt4_automoc( ${mprocess_SRCS} )

kde4_add_library(mdocviewer ${mdocviewer_SRCS})

#kde4_add_library(MProcessShellWidget MProcessShellWidget.cpp)
#target_link_libraries(MParser ${QT_LIBRARIES})

#install(TARGETS editor DESTINATION ${BIN_INSTALL_DIR})
#install(FILES editorui.rc
#DESTINATION ${DATA_INSTALL_DIR}/editor)
