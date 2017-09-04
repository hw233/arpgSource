
add_subdirectory(svrcore)
add_subdirectory(http)
add_subdirectory(db_wrap)
add_subdirectory(libItem)
if(WITH_LOGIND)
  add_subdirectory(logind)
endif()

if(WITH_APPD)
  add_subdirectory(appd)
endif()

if(WITH_SCENED)
  add_subdirectory(scened)
endif()

if(WITH_POLICED)
  add_subdirectory(policed)
endif()

if(WITH_ROBOTD)
  add_subdirectory(robotd)
endif()

if (WITH_UNITTEST)
  add_subdirectory(test)
endif()

