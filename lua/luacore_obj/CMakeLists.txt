

file(GLOB luacore_obj_src *.c *.h *.cc *.cpp *.hpp *.pkg)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/lib/deps/tolua  
  ${CMAKE_SOURCE_DIR}/lib/include/core_obj
  ${CMAKE_SOURCE_DIR}/lua/luacocos
)


add_library(luacore_obj STATIC ${luacore_obj_src})
target_link_libraries(luacore_obj ${cow_libs})

if(MSVC)	
  set_target_properties(luacore_obj PROPERTIES FOLDER "lua")
endif()
