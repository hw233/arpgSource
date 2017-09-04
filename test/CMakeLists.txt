include_directories(
  ${CMAKE_SOURCE_DIR}/src/svrcore
  ${CMAKE_SOURCE_DIR}/src/
)

######################################
##正则表达式库
#cxx_test2(regex)

########################################
##网络协议测试
cxx_test2(svrcore)
target_link_libraries(svrcore_test
	${tea_libs}
	ahocorasick
)
cxx_test2(core_obj)
if (NOT WIN32)
  target_link_libraries(core_obj_test pthread)
endif()

#######################################
add_definitions(-DMONGO_USE__INT64)
add_definitions(-DMONGO_USE_LONG_LONG_INT)
add_definitions(-DMONGO_HAVE_STDINT)
if (WIN32)  
  add_definitions(-DMONGO_DLL_BUILD)
else()
  add_definitions(-DMONGO_HAVE_UNISTD)  
endif()
#cxx_test2(libmongoclient)
