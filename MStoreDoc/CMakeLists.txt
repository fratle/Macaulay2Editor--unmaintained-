project(mstoredoc)
find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})
set(mstore_SRCS
    mstoredoc.cpp
    mdocvalues.cpp
    )

kde4_add_library(MStoredoc ${mstore_SRCS})
#target_link_libraries(mstoredoc ${KDE4_KDEUI_LIBS}
#${KDE4_KIO_LIBS})

#install(TARGETS editor DESTINATION ${BIN_INSTALL_DIR})
#install(FILES editorui.rc
#DESTINATION ${DATA_INSTALL_DIR}/editor)
