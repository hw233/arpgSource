
file(GLOB SRC *.c *.h *.cc *.cpp *.hpp *.pkg)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/lib/deps/tolua  
  ${CMAKE_SOURCE_DIR}/lua/luacocos
)

add_library(lua_mongo_wrap STATIC ${SRC})
target_link_libraries(lua_mongo_wrap ${LIBS})

if(MSVC)	
  set_target_properties(lua_mongo_wrap PROPERTIES FOLDER "lua")
endif()
