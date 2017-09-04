# output generic information about the core and buildtype chosen

message("")
message("* tea revision   : ${HG_REVISION}")
if( UNIX )
  message("* Build binaries in      : ${CMAKE_BUILD_TYPE} mode")
endif()
message("")

# output information about installation-directories and locations

message("* Install core to        : ${CMAKE_INSTALL_PREFIX}")
if( UNIX )
  message("* Install libraries to   : ${LIBSDIR}")
  message("* Install configs to     : ${CONF_DIR}")
endif()
message("")

# Show infomation about the options selected during configuration

if( USE_COREPCH )
  message("* Build core w/PCH       : Yes (default)")
else()
  message("* Build core w/PCH       : No")
endif()

if( USE_SFMT )
  message("* Use SFMT for RNG       : Yes")
  add_definitions(-DUSE_SFMT_FOR_RNG)
else()
  message("* Use SFMT for RNG       : No  (default)")
endif()

if( WITH_WARNINGS )
  message("* Show all warnings      : Yes")
else()
  message("* Show compile-warnings  : No  (default)")
endif()

if( WITH_COREDEBUG )
  message("* Use coreside debug     : Yes")
  add_definitions(-DTRINITY_DEBUG)
else()
  message("* Use coreside debug     : No  (default)")
endif()

if (WITH_UNITTEST)
  message("Use UnitTest 			: Yes (default)")
else()
  message("Use UnitTest				: No")
endif()

if (WITH_G_PERFTOOLS)
  message("use google perftools	: Yes")
else()
  message("use google perftools	: No (default)")
endif()

if (WITH_LUAJIT)
  message("use lua jit : Yes")
else()
  message("use lua jit : No (default)")
endif()

message("")
