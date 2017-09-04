
########################################################################
#
# Defines the gtest & gtest_main libraries.  User tests should link
# with one of them.
function(cxx_library_with_type name type cxx_flags)
  # type can be either STATIC or SHARED to denote a static or shared library.
  # ARGN refers to additional arguments after 'cxx_flags'.
  add_library(${name} ${type} ${ARGN})
  set_target_properties(${name}
    PROPERTIES
    COMPILE_FLAGS "${cxx_flags}")
    if (CMAKE_USE_PTHREADS_INIT)
      target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT})
    endif()
endfunction()

function(cxx_static_library name cxx_flags)
  cxx_library_with_type(${name} STATIC "${cxx_flags}" ${ARGN})
endfunction()

function(cxx_shared_library name cxx_flags)
  cxx_library_with_type(${name} SHARED "${cxx_flags}" ${ARGN})
endfunction()

function(cxx_library name cxx_flags)
  # TODO(vladl@google.com): Make static/shared a user option.
  cxx_static_library(${name} "${cxx_flags}" ${ARGN})
endfunction()



# cxx_executable_with_flags(name cxx_flags lib srcs...)
#
# creates a named C++ executable that depends on the given library and
# is built from the given source files with the given compiler flags.
function(cxx_executable_with_flags name cxx_flags lib)
  add_executable(${name} ${ARGN})
  if (cxx_flags)
    set_target_properties(${name}
      PROPERTIES
      COMPILE_FLAGS "${cxx_flags}")
  endif()
  target_link_libraries(${name} ${lib})
endfunction()

# cxx_executable(name dir lib srcs...)
#
# creates a named target that depends on the given lib and is built
# from the given source files.  dir/name.cc is implicitly included in
# the source file list.
function(cxx_executable name dir lib)
  cxx_executable_with_flags(
    ${name} "${cxx_default}" ${lib} "${dir}/${name}.cc" ${ARGN})
endfunction()


############################################################
# C++ tests built with standard compiler flags.

# cxx_test_with_flags(name cxx_flags libs srcs...)
#
# creates a named C++ test that depends on the given libs and is built
# from the given source files with the given compiler flags.
function(cxx_test_with_flags name cxx_flags libs)
  add_executable(${name} ${ARGN})
  set_target_properties(${name}
    PROPERTIES
    COMPILE_FLAGS "${cxx_flags}")
  # To support mixing linking in static and dynamic libraries, link each
  # library in with an extra call to target_link_libraries.
  foreach (lib "${libs}")
    target_link_libraries(${name} ${lib})
  endforeach()
  add_test(${name} ${name})
endfunction()

# cxx_test(name libs srcs...)
#
# creates a named test target that depends on the given libs and is
# built from the given source files.  Unlike cxx_test_with_flags,
# test/name.cc is already implicitly included in the source file list.
function(cxx_test name libs)
  cxx_test_with_flags("${name}" "${cxx_default}" "${libs}"
    "test/${name}.cc" ${ARGN})
endfunction()

function(__cxx_test name libs)
  file(GLOB_RECURSE sources_localdir ${name}/*.cpp ${name}/*.h ${name}/*.c ${name}/*.cc)
  set(test_SRCS ${sources_localdir} gtest_main.cc)
  cxx_test_with_flags("${name}_test" "${cxx_default}" "${libs}"
    "${test_SRCS}" ${ARGN})
  
  if (WIN32 AND MSVC)	
	SOURCE_GROUP(Test FILES ${sources_localdir})
  endif()
endfunction()

function(cxx_test2 name)
  file(GLOB_RECURSE sources_localdir ${name}/*.cpp ${name}/*.h ${name}/*.c ${name}/*.cc)
  set(test_SRCS ${sources_localdir} gtest_main.cc)
#  message(${sources_localdir})
#  message(${libs})
  add_executable("${name}_test" ${test_SRCS})
  set_target_properties("${name}_test" PROPERTIES FOLDER "test")
  set_target_properties("${name}_test" PROPERTIES LINK_FLAGS "${test_LINK_FLAGS}")
#  target_link_libraries("${name}_test" ${libs}) 
  target_link_libraries("${name}_test" gtest)
  target_link_libraries("${name}_test" ${name})
  if (WIN32 AND MSVC)	
    set_target_properties("${name}_test" PROPERTIES FOLDER "test")
  endif()
endfunction()

##linbc add 20110411
function(cxx_test3 name srcs libs)
  set(test_PATH ${CMAKE_SOURCE_DIR}/test/${name})
  file(GLOB_RECURSE test_sources_localdir ${test_PATH}/*.cpp ${test_PATH}/*.h ${test_PATH}/*.c ${test_PATH}/*.cc)
  
  set(test_SRCS ${CMAKE_SOURCE_DIR}/test/gtest_main.cc)
  set(test_SRCS ${test_SRCS} ${srcs} ${test_sources_localdir})
 
  add_executable("${name}_test" ${test_SRCS})
  if (WIN32 AND MSVC)	
    set_target_properties("${name}_test" PROPERTIES FOLDER "test")
  endif()
  target_link_libraries("${name}_test" gtest)
  target_link_libraries("${name}_test" ${libs})
  if (WIN32 AND MSVC)	
	SOURCE_GROUP(Test FILES ${test_sources_localdir} ${CMAKE_SOURCE_DIR}/test/gtest_main.cc)
  endif()
endfunction()

##linbc add 20110411
function(cxx_test_mock name srcs libs)
  set(test_PATH ${CMAKE_SOURCE_DIR}/test/${name})
  file(GLOB_RECURSE test_sources_localdir ${test_PATH}/*.cpp ${test_PATH}/*.h ${test_PATH}/*.c ${test_PATH}/*.cc)
  
  set(test_SRCS ${CMAKE_SOURCE_DIR}/test/gmock_main.cc)
  set(test_SRCS ${test_SRCS} ${srcs} ${test_sources_localdir})
  
  include_directories(
    ${CMAKE_SOURCE_DIR}/dep/gmock/include
  )
 
  add_executable("${name}_test" ${test_SRCS})
  if (WIN32 AND MSVC)	
    set_target_properties("${name}_test" PROPERTIES FOLDER "test")
  endif()
  target_link_libraries("${name}_test" gmock gtest)
  target_link_libraries("${name}_test" ${libs})
  if (WIN32 AND MSVC)	
	SOURCE_GROUP(Test FILES ${test_sources_localdir} ${CMAKE_SOURCE_DIR}/test/gmock_main.cc)
  endif()
endfunction()
