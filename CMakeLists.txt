cmake_minimum_required(VERSION 3.0)

project(Converter CXX)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(Converter_SRCS
  Converter.cxx
  vtkOBJWriter.h
  vtkOBJWriter.cxx
)

add_executable( ${PROJECT_NAME} ${Converter_SRCS})
target_link_libraries( ${PROJECT_NAME}  ${VTK_LIBRARIES})
