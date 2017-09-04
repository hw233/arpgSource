

file(GLOB luahttp_src *.c *.h *.cc *.cpp *.hpp *.pkg)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/lib/deps/tolua  
  ${CMAKE_SOURCE_DIR}/src/http
  ${CMAKE_SOURCE_DIR}/lua/luacocos
)


add_library(luahttp STATIC ${luahttp_src})
target_link_libraries(luahttp ${LIBS} http)

if(MSVC)	
  set_target_properties(luahttp PROPERTIES FOLDER "lua")
endif()
