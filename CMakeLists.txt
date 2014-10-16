project(vcd2path)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(vcd2path_SRCS src/Vcd2path.cpp src/main.cpp)

set (VCD2PATH_SOURCES
  src/Vcd2path.cpp
  src/StepperDriver.cpp
  src/Signal.cpp
  src/main.cpp
  )

set (VCD2PATH_HEADERS
  src/Vcd2path.h
  )

set (VCD2PATH_FORMS
  src/Vcd2path.ui
  )

set (VCD2PATH_RESOURCES

  )

add_subdirectory(src/PrintBedView)
add_subdirectory(src/SignalAssignment)
add_subdirectory(src/gtkwave)

QT4_WRAP_CPP(VCD2PATH_HEADERS_MOC ${VCD2PATH_HEADERS})
QT4_WRAP_UI(VCD2PATH_FORMS_HEADERS ${VCD2PATH_FORMS})
QT4_ADD_RESOURCES(VCD2PATH_RESOURCES_RCC ${VCD2PATH_RESOURCES})
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(vcd2path ${VCD2PATH_SOURCES} ${VCD2PATH_HEADERS_MOC} ${VCD2PATH_FORMS_HEADERS} ${VCD2PATH_RESOURCES_RCC})
target_link_libraries(vcd2path ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS vcd2path RUNTIME DESTINATION bin)
