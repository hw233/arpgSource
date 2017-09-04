project(arpg)
set_property(GLOBAL PROPERTY USE_FOLDERS On)

# CMake policies (can not be handled elsewhere)
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0005 OLD)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

# build in Release-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Release")
endif()

#include(CheckCXXSourceRuns)
#include(CheckIncludeFiles)
include(CheckPlatform)
include(CheckBuildDir)
include(EnsureVersion)
# basic packagesearching and setup (further support will be needed, this is a preliminary release!)
find_package(PCHSupport)
find_package(TestSupport)

# set default buildoptions and print them
include(cmake/options.cmake)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if( NOPCH )
  set(USE_COREPCH 0)  
endif()

# Find current revision of downloaded sourcetree
execute_process(
  COMMAND hg tip --template {rev}
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE HG_REVISION
)

#输出路径
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

########################################################
#所有的项目都包含这些路径

if( USE_COREPCH )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}/lib/include
  ${CMAKE_SOURCE_DIR}/lib/deps  
  ${CMAKE_SOURCE_DIR}/lib/deps/gtest/include  
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/object
  ${CMAKE_SOURCE_DIR}/src/svrcore  
  ${CMAKE_SOURCE_DIR}/lua
  ${CMAKE_SOURCE_DIR}/lua/luacocos
  ${CMAKE_SOURCE_DIR}/lib/deps/tolua  
)

LINK_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/lib/${PLATFORM_FOLDER}-${ARCH_DIR}
)

set(cow_libs
  ${cow_libs}
  comm
  net
  memdb
  zlib
  core_obj  
  libmongoclient
  db_wrap

  #for lua  
  tolua
  luacocos
)

if (NOT WIN32)
  set(cow_libs ${cow_libs} mongoclient boost_filesystem boost_system boost_regex boost_thread)
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(cow_libs ${cow_libs} jemalloc)
  else()
    set(cow_libs ${cow_libs} iconv)
  endif()
else()
  SET(WIN32_LOCAL_INCLUDE "D:/win32_local_lib/include")
  SET(WIN32_LOCAL_LIB ${CMAKE_LIBRARY_PATH} "D:/win32_local_lib/lib")
  include_directories(${WIN32_LOCAL_INCLUDE})
  LINK_DIRECTORIES(${WIN32_LOCAL_LIB})
endif()
#endif()


if (WITH_LUAJIT)
  include_directories(${CMAKE_SOURCE_DIR}/lib/deps/luajit/src)
  set(cow_libs ${cow_libs} lua)
else()
  include_directories( ${CMAKE_SOURCE_DIR}/lib/deps/lualite/include)
  include_directories( ${CMAKE_SOURCE_DIR}/lib/deps/lualite/include/lua)
  set(cow_libs ${cow_libs} lualite)
endif()

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

########################################################

# add core sources
add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(lua)
