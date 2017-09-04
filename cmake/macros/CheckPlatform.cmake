# default to x86 platform.  We'll check for X64 in a bit
set(PLATFORM X86)

if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM X64)
    MESSAGE(STATUS "Detected 64-bit platform")
	set(ARCH_DIR "64bit")
else()
    MESSAGE(STATUS "Detected 32-bit platform")
	set(ARCH_DIR "32bit")
endif()

include("${CMAKE_SOURCE_DIR}/cmake/platform/settings.cmake")

if(WIN32)
  include("${CMAKE_SOURCE_DIR}/cmake/platform/settings.win32.cmake")
  set(PLATFORM_FOLDER win32)
elseif(APPLE)
  include("${CMAKE_SOURCE_DIR}/cmake/platform/settings.osx.cmake")
  set(PLATFORM_FOLDER mac)
elseif(UNIX)
  include("${CMAKE_SOURCE_DIR}/cmake/platform/settings.unix.cmake")
  set(PLATFORM_FOLDER linux)
endif()
