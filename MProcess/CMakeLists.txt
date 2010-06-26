project(MProcess)
find_package(KDE4 REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
include_directories(${KDE4_INCLUDES})
set(mprocess_SRCS
      	mprocess.cpp
	mprocessshellwidget.cpp
	mterminal.cpp
    )

#kde4_automoc( ${mprocess_SRCS} )
#qt4_automoc( ${mprocess_SRCS} )
kde4_add_library(MProcess ${mprocess_SRCS})
#kde4_add_library(MProcessShellWidget MProcessShellWidget.cpp)
#target_link_libraries(MProcess MProcessShellWidget ${KDE4_KIO_LIBS})

#install(TARGETS editor DESTINATION ${BIN_INSTALL_DIR})
#install(FILES editorui.rc
#DESTINATION ${DATA_INSTALL_DIR}/editor)
