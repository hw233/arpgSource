# Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

option(USE_COREPCH      "Use precompiled headers when compiling servers"				1)
option(USE_SFMT         "Use SFMT as random numbergenerator"            				0)
option(WITH_WARNINGS    "Show all warnings during compile"              				1)
option(WITH_COREDEBUG   "Include additional debug-code in core"         				0)
option(WITH_UNITTEST	"add unit test project"											0)
option(WITH_LOGIND		"compile logind "												1)
option(WITH_APPD		"compile appd "													1)
option(WITH_SCENED		"compile scened "												1)
option(WITH_POLICED		"compile policed "												1)
option(WITH_ROBOTD		"compile robotd "												1)
option(WITH_G_PERFTOOLS "use google perftools " 										0)
option(WITH_LUAJIT 		"use lua jit "			 										1)