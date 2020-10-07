include(CMakeFindDependencyMacro)
find_dependency(Qt5 COMPONENTS Core)

include("${CMAKE_CURRENT_LIST_DIR}/CedarFrameworkTargets.cmake")
