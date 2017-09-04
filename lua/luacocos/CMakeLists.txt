file(GLOB sources_localdir *.cpp *.h *.c)
include_directories(
  ${CMAKE_SOURCE_DIR}/lib/deps/tolua  
  ${CMAKE_SOURCE_DIR}/lua
)
add_library(luacocos STATIC ${sources_localdir})
target_link_libraries(luacocos tolua lua)

if(MSVC)	
  set_target_properties(luacocos PROPERTIES FOLDER "lua")
endif()
