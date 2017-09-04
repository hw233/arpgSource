file(GLOB sources_localdir *.cpp *.h *.c)
file(GLOB sources_object object/*.cpp object/*.h object/*.c)
file(GLOB sources_server server/*.cpp server/*.h server/*.c server/*.inl PCH/*.h PCH/*.cpp)
file(GLOB sources_shared shared/*.cpp shared/*.h shared/*.inl shared/*.c shared/*.svm)
file(GLOB sources_protocol protocol/*.cpp protocol/*.h protocol/*.c protocol/*.vm)
file(GLOB sources_w_protocol w_protocol/*.cpp w_protocol/*.h w_protocol/*.c w_protocol/*.vm)

set(svrcore_STAT_SRCS
  ${svrcore_STAT_SRCS}
  ${sources_localdir}
  ${sources_object}
  ${sources_server}
  ${sources_shared}
  ${sources_protocol}  
  ${sources_w_protocol}  
  ${sources_db_wrap}
) 

include_directories(
  ${CMAKE_BINARY_DIR}  
  ${CMAKE_CURRENT_SOURCE_DIR}  
  ${CMAKE_SOURCE_DIR}/lua/luacocos
  ${CMAKE_SOURCE_DIR}/lib/deps/tolua  
)

#add_definitions(-DTEA_SVRCORE_EXPORTS)
#add_library(svrcore SHARED ${svrcore_STAT_SRCS})
add_library(svrcore STATIC ${svrcore_STAT_SRCS})
target_link_libraries(svrcore ${cow_libs})

######################## ‘§±‡“Î÷ß≥÷ #########################
# Generate precompiled header
if( USE_COREPCH )
  if(CMAKE_COMPILER_IS_GNUCXX)
#	add_precompiled_header(svrcore ${CMAKE_CURRENT_SOURCE_DIR}/PCH/svrcorePCH.h)
  elseif(MSVC)
#	add_native_precompiled_header(svrcore ${CMAKE_CURRENT_SOURCE_DIR}/PCH/svrcorePCH)
  endif()
endif()

if (WIN32 AND MSVC)	
	SOURCE_GROUP(object FILES ${sources_object} ${sources_include_object})
	SOURCE_GROUP(w_protocol FILES ${sources_w_protocol}  )	
	SOURCE_GROUP(protocol FILES ${sources_include} ${sources_localdir} ${sources_protocol})	
	SOURCE_GROUP(server FILES ${sources_include_server} ${sources_server})	
	SOURCE_GROUP(shared FILES ${sources_include_shared} ${sources_shared})	
	SOURCE_GROUP(db_wrap FILES ${sources_db_wrap} ${sources_db_wrap})	
	set_target_properties(svrcore PROPERTIES FOLDER "src")
endif()
