

file(GLOB luasocket_src *.c *.h *.cc *.hpp)
file(GLOB_RECURSE luasocket_win32_src win32/*.c win32/*.h win32/*.cc win32/*.hpp)
file(GLOB_RECURSE luasocket_unix_src unix/*.c unix/*.h unix/*.cc unix/*.hpp)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)
add_definitions("-DLUASOCKET_INET_PTON")
if(WIN32)
  set(luasocket_src ${luasocket_src} ${luasocket_win32_src})
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/win32
  )
else()
  set(luasocket_src ${luasocket_src} ${luasocket_unix_src})
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/unix
  )
endif()

add_library(luasocket STATIC ${luasocket_src})
#target_link_libraries(luasocket ${LIBS})

if(MSVC)	
  set_target_properties(luasocket PROPERTIES FOLDER "lua")
endif()
