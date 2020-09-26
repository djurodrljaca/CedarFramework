# This file is part of Cedar Framework.
#
# Cedar Framework is free software: you can redistribute it and/or modify it under the terms
# of the GNU Lesser General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# Cedar Framework is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with C++ Config
# Framework. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5.0)

project(CedarFramework VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

# --------------------------------------------------------------------------------------------------
# Code Coverage
# --------------------------------------------------------------------------------------------------
option(CedarFramework_CodeCoverage "Cedar Framework Code Coverage" OFF)

if (CedarFramework_CodeCoverage MATCHES ON)
    include(cmake/CodeCoverage.cmake)
    APPEND_COVERAGE_COMPILER_FLAGS()

    set(excludes_cmake_prefix_path)
    foreach(item ${CMAKE_PREFIX_PATH})
        get_filename_component(item ${item} ABSOLUTE)
        list(APPEND excludes_cmake_prefix_path "'${item}/*'")
    endforeach()

    set(CedarFramework_CodeCoverage_EXCLUDES
            "'/opt/*'"
            "'/usr/*'"
            ${excludes_cmake_prefix_path}
            "'${CMAKE_SOURCE_DIR}/tests/*'"
            "'${CMAKE_BINARY_DIR}/*'"
        )
endif()

# --------------------------------------------------------------------------------------------------
# CedarFramework library
# --------------------------------------------------------------------------------------------------
add_library(CedarFramework SHARED
        inc/CedarFramework/Deserialization.hpp
        inc/CedarFramework/LoggingCategories.hpp
        inc/CedarFramework/Query.hpp
        inc/CedarFramework/Serialization.hpp

        src/Deserialization.cpp
        src/LoggingCategories.cpp
        src/Query.cpp
        src/Serialization.cpp
    )

include(GenerateExportHeader)
generate_export_header(CedarFramework
    EXPORT_FILE_NAME exports/CedarFramework/CedarFrameworkExport.hpp)

target_include_directories(CedarFramework PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/inc>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/exports>
        $<INSTALL_INTERFACE:include>
    )

target_link_libraries(CedarFramework PUBLIC
        Qt5::Core
    )

set_target_properties(CedarFramework PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

# --------------------------------------------------------------------------------------------------
# Package
# --------------------------------------------------------------------------------------------------
add_library(CedarFramework::CedarFramework ALIAS CedarFramework)

install(TARGETS CedarFramework EXPORT CedarFrameworkTargets DESTINATION
        LIBRARY  DESTINATION lib
        ARCHIVE  DESTINATION lib
        RUNTIME  DESTINATION bin
        INCLUDES DESTINATION include
    )

install(
        DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}/inc/CedarFramework
        DESTINATION include
        COMPONENT   Devel
    )

install(
        DIRECTORY   ${PROJECT_BINARY_DIR}/exports/CedarFramework
        DESTINATION include
        COMPONENT   Devel
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/CedarFramework/CedarFrameworkConfigVersion.cmake"
        VERSION ${CedarFramework_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

export(EXPORT CedarFrameworkTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/CedarFramework/CedarFrameworkTargets.cmake"
        NAMESPACE CedarFramework::
    )

configure_file(cmake/CedarFrameworkConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/CedarFramework/CedarFrameworkConfig.cmake"
        COPYONLY
    )

set(ConfigPackageLocation lib/cmake/CedarFramework)

install(
        EXPORT      CedarFrameworkTargets
        FILE        CedarFrameworkTargets.cmake
        NAMESPACE   CedarFramework::
        DESTINATION ${ConfigPackageLocation}
    )

install(
        FILES
            cmake/CedarFrameworkConfig.cmake
            "${CMAKE_CURRENT_BINARY_DIR}/CedarFramework/CedarFrameworkConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation}
        COMPONENT   Devel
    )

# --------------------------------------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
