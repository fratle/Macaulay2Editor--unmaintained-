project(Macaulay2Editor)
find_package(KDE4 REQUIRED)
find_package(Qt4)
include (KDE4Defaults)
include_directories(${KDE4_INCLUDES} ${PROJECT_SOURCE_DIR})
set(editor_SRCS
      main.cpp
    )

SET(QT_USE_QTSQL 1)
INCLUDE(${QT_USE_FILE})

# set default cmake build type to RelWithDebInfo (None Debug Release RelWithDebInfo MinSizeRel)
IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE "Release" )
ENDIF()

# require proper c++
ADD_DEFINITIONS (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
ADD_DEFINITIONS( "-Wall -ansi -pedantic -fexceptions" )
#ADD_DEFINITIONS(" -ggdb -DBOOST_DEBUG -DDEBUG -DDebug -debug")
remove_definitions(-DQT_NO_STL)
# add debug definitions
#IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR
#CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
#ADD_DEFINITIONS( "-DDEBUG" )
#ENDIF()

#SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
#  "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )

add_subdirectory (MainWindow)
add_subdirectory (MAutocomplete)
add_subdirectory (FileBrowserWidget)
add_subdirectory (MStoreDoc)
add_subdirectory (MSettings)
add_subdirectory (MProcess)
add_subdirectory (MParser)
add_subdirectory (MDocViewer)

kde4_add_executable(editor ${editor_SRCS})

target_link_libraries(editor mainwindow mautocomplete mdocviewer filebrowserwidget MParser MProcess MSettings MStoredoc ${KDE4_KDEUI_LIBS}
${KDE4_KIO_LIBS}
${KDE4_KTEXTEDITOR_LIBS}
${KDE4_KPTY_LIBS})

#install(TARGETS editor DESTINATION ${BIN_INSTALL_DIR})
#install(FILES editorui.rc
#DESTINATION ${DATA_INSTALL_DIR}/editor)
