cmake_minimum_required(VERSION 3.22)
project(itk-examples VERSION 0.1.0)

include(CTest)
enable_testing()
set(ITK_DIR, "/home/sshivaditya/vcpkg/packages/itk_x64-linux/share/itk")
find_package(ITK REQUIRED)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkIOImage
  vtkInteractionImage
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  QUIET
)

include(${ITK_USE_FILE})
add_executable(itk-examples QuickDemo.cxx)
find_package(GTest)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

find_package(VTK REQUIRED)
include("${VTK_USE_FILE}")

include(CPack)
target_link_libraries(itk-examples ${ITK_LIBRARIES} ${VTK_LIBRARIES})