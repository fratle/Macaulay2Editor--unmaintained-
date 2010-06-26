project(mautocomplete)
find_package(KDE4 REQUIRED)
#include_directories(${KDE4_INCLUDES} ${editor_SOURCE_DIR}/MainWindow)
set(mautocomplete_SRCS
    mautocomplete.cpp
    mautocompletewidget.cpp    
)

kde4_add_library(mautocomplete ${mautocomplete_SRCS})
#link_libraries(mautocomplete mainwindow) #${KDE4_KDEUI_LIBS}
#${KDE4_KIO_LIBS})

#install(TARGETS editor DESTINATION ${BIN_INSTALL_DIR})
#install(FILES editorui.rc
#DESTINATION ${DATA_INSTALL_DIR}/editor)
