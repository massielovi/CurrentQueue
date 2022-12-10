# CMakeList.txt: proyecto de CMake para QueueParallel, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

project ("QueueParallel")

# Agregue un origen al ejecutable de este proyecto.
add_executable (QueueParallel "QueueParallel.cpp" "QueueParallel.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET QueueParallel PROPERTY CXX_STANDARD 20)
endif()

# TODO: Agregue pruebas y destinos de instalación si es necesario.
