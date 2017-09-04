   
######################## 项目源文件 ############################
file(GLOB_RECURSE sources_PCH PCH/*.cpp  PCH/*.h)
file(GLOB_RECURSE sources_server server/*.cpp  server/*.h server/*.c)
file(GLOB_RECURSE sources_handler handler/*.cpp  handler/*.h handler/*.c handler/*.inl)
file(GLOB_RECURSE sources_localdir *.cpp *.h *.c *.dist *.lua) 
 
set (prj_target	cow_policed) 

set(policed_SRCS
  ${sources_server}
  ${sources_handler}
)

 set(sources_SRC
   ${sources_server}
	${sources_handler}
 )
  
if ( USE_COREPCH AND MSVC )
  set(policed_SRCS 
	${policed_SRCS}
	${sources_PCH} 
  )
endif()

if( WIN32 )
  set(policed_SRCS
	${policed_SRCS}
	${sources_Win32}
  )
endif()

######################## 包含文件 ########################

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/server
	${CMAKE_CURRENT_SOURCE_DIR}/handler
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_BINARY_DIR}	
	${CMAKE_CURRENT_SOURCE_DIR}
)

######################## 编译选项及目标 ########################
set(policed_LINK_FLAGS "")

add_executable(${prj_target} ${policed_SRCS} ${sources_localdir})
set_target_properties(${prj_target} PROPERTIES LINK_FLAGS "${policed_LINK_FLAGS}")


######################## 项目依赖库 ########################
set(libs
  ${cow_libs}
  svrcore
  ahocorasick
  libmongoclient
  http
)

target_link_libraries(${prj_target} ${libs})

######################## 预编译支持 #########################
# Generate precompiled header
if( USE_COREPCH )
  if(CMAKE_COMPILER_IS_GNUCXX)
	add_precompiled_header(${prj_target} ${CMAKE_CURRENT_SOURCE_DIR}/PCH/policedPCH.h)
  elseif(MSVC)
	add_native_precompiled_header(${prj_target} ${CMAKE_CURRENT_SOURCE_DIR}/PCH/policedPCH)
	string (REPLACE "/Zm1000" "/Zm500" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  endif()
endif()

######################## 其他设置 ########################

if( UNIX )
  install(TARGETS ${prj_target} DESTINATION bin)
  install(FILES ${prj_target}.conf.dist DESTINATION etc)
  #install(FILES cow_policed.lua DESTINATION etc)
elseif( WIN32 )
  install(TARGETS ${prj_target} DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES ${prj_target}.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
  #install(FILES cow_policed.lua DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

if( WIN32 )
  add_custom_command(TARGET ${prj_target}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${prj_target}.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )  
endif()

if (WIN32 AND MSVC)
  SOURCE_GROUP(main FILES ${sources_localdir})
  SOURCE_GROUP(server FILES ${sources_server})
  SOURCE_GROUP(handler FILES ${sources_handler})  

  set_target_properties(${prj_target} PROPERTIES FOLDER "src") 
endif()

####################### 测试项目 #####################
#if(WITH_UNITTEST)
#  cxx_test3(${prj_target}
#	"${sources_SRC}"
#	"${libs}"
#  )
#endif()


