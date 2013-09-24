cmake_minimum_required(VERSION 2.8)
project(PowerFailureAlarmConfiguration)

FIND_PACKAGE(Qt4 REQUIRED)
#Include repertories
INCLUDE_DIRECTORIES(
    inc
	${QT_INCLUDES}
	${CMAKE_CURRENT_BINARY_DIR}
)

FILE( GLOB source_files_config ./src/*.cpp )
FILE( GLOB IHM_SOURCES src/*.cpp )
FILE( GLOB IHM_HEADERS inc/*.h )
FILE( GLOB IHM_UI gui/*.ui )

QT4_ADD_RESOURCES(QtApp_RCC_SRCS resources/qled.qrc)

#SET(IHM_SOURCES gui/Interface_Noyau/Thread_IHM_Noyau.cpp gui/Interface_Noyau/IHM_Noyau.cpp )
#SET(IHM_HEADERS gui/Interface_Noyau/Thread_IHM_Noyau.h gui/Interface_Noyau/IHM_Noyau.h )

QT4_WRAP_UI(SOURCES_UI_HEADERS ${IHM_UI})
QT4_WRAP_CPP(IHM_HEADERS_MOC ${IHM_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
set(QT_LIBRARIES ${QT_LIBRARIES} QtSvg)
add_executable( PowerFailureAlarmConfig ${IHM_SOURCES} ${IHM_HEADERS_MOC} ${SOURCES_UI_HEADERS} ${source_files_config} ${QtApp_RCC_SRCS} )
target_link_libraries (PowerFailureAlarmConfig ${QT_LIBRARIES} )