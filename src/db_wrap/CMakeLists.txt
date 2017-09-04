   
 
file(GLOB sources_localdir *.cpp *.h *.c *.cc)


add_library(db_wrap STATIC ${sources_localdir})
#add_executable(http_client ${http_STAT_SRCS})
target_link_libraries(db_wrap libmongoclient comm)

if (MSVC)	
  set_target_properties(db_wrap PROPERTIES FOLDER "lib")
endif()
