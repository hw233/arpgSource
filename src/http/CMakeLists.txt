   
 
file(GLOB sources_localdir *.cpp *.h *.c)
file(GLOB sources_client client/*.cpp client/*.h client/*.c)
file(GLOB sources_server server/*.hpp server/*.cpp server/*.h server/*.c)

set(http_STAT_SRCS
  ${sources_client}
  ${sources_localdir}
  ${sources_server}
) 

include_directories(
  ${CMAKE_BINARY_DIR}  
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}/client
  ${CMAKE_CURRENT_SOURCE_DIR}/server
)

add_library(http STATIC ${http_STAT_SRCS})
#add_executable(http_client ${http_STAT_SRCS})
target_link_libraries(http ${cow_libs})

if (MSVC)	
  SOURCE_GROUP(client FILES ${sources_client})	
  SOURCE_GROUP(server FILES ${sources_server})	
  set_target_properties(http PROPERTIES FOLDER "lib")
endif()
