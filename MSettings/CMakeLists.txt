project(MSettings)
find_package(KDE4 REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
include_directories(${KDE4_INCLUDES})
set(msettings_SRCS
      	msettings.cpp
    )

#kde4_automoc( ${mprocess_SRCS} )
#qt4_automoc( ${mprocess_SRCS} )
kde4_add_library(MSettings ${msettings_SRCS})
#kde4_add_library(MProcessShellWidget MProcessShellWidget.cpp)
#target_link_libraries(MProcess MProcessShellWidget ${KDE4_KIO_LIBS})

#install(TARGETS editor DESTINATION ${BIN_INSTALL_DIR})
#install(FILES editorui.rc
#DESTINATION ${DATA_INSTALL_DIR}/editor)
