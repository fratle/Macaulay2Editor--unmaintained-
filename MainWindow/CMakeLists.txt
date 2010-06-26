project(mainwindow)
find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})
set(mainwindow_SRCS
    mainwindow.cpp
    meditormanager.cpp
    meditor.cpp
)

kde4_add_library(mainwindow ${mainwindow_SRCS})
#target_link_libraries(mainwindow ${KDE4_KDEUI_LIBS}
#${KDE4_KIO_LIBS})
 
