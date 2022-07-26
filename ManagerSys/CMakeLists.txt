set_property(GLOBAL PROPERTY USE_FOLDERS On)
# Îª¹€³ÌÉèÖÃ×îµÍµÄCmake°æ±ŸÐèÇó
CMAKE_MINIMUM_REQUIRED(VERSION 1.0.0 FATAL_ERROR)

# CMAKE_POLICYÃüÁîÀŽÉèÖÃ°æ±ŸµÄŒæÈÝÐÔ£¬ÒÔ±ÜÃâÒ»Ð©²»±ØÒªµÄŸ¯žæÐÅÏ¢¡£
IF( COMMAND CMAKE_POLICY )
   # Works around warnings libraries linked against that don't
   # have absolute paths 
   CMAKE_POLICY( SET CMP0003 NEW )
   # Works around warnings about escaped quotes in ADD_DEFINITIONS
   # statements.
   CMAKE_POLICY(SET CMP0005 NEW)
   
   CMAKE_POLICY( SET CMP0010 NEW )
ENDIF( COMMAND CMAKE_POLICY )

SET(CMAKE_CXX_COMPILE "/home/dingo/Documents/fsl-linaro-toolchain/bin/arm-none-linux-gnueabi-g++")
# User section
PROJECT (TestRibbon)

#ÉèÖÃ¹€³ÌµÄÍ·ÎÄŒþÄ¿ÂŒ
SET (VIRE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
SET (VIRE_BIN_DIR     ${PROJECT_SOURCE_DIR}/bin )
SET (VIRE_LIB_DIR     ${PROJECT_SOURCE_DIR}/lib )
SET (VIRE_SRC_DIR     ${PROJECT_SOURCE_DIR}/src )

# ÈÃCMakeÉú³ÉÄÜÔÚ¹¹œšÊ±ÏÔÊŸ²ÊÉ«×ŽÌ¬ÐÅÏ¢µÄMakefile¡£
SET(CMAKE_COLOR_MAKEFILE ON)

# ÊÇ·ñÊµÏÖÏà¶ÔÂ·Ÿ¶
SET(CMAKE_USE_RELATIVE_PATHS false )

# set path to additional CMake modules
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")
MESSAGE("********************${CMAKE_MODULE_PATH}********************************")

################################################################################
# Create bin and lib directories if required

IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin ${CMAKE_BINARY_DIR}/lib)
ENDIF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

################################################################################
# Installation stuff
SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows" )
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")

# Here we apparantly do some funky stuff with making the bin/ and lib/
# folders which is probably needed to work around a very old CMake bug?

#SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
SET(OUTPUT_BINDIR ${PROJECT_SOURCE_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_BINDIR})
IF(MSVC AND NOT MSVC_IDE)
    MAKE_DIRECTORY(${OUTPUT_BINDIR}/${OSG_PLUGINS})
ENDIF(MSVC AND NOT MSVC_IDE)


SET(OUTPUT_LIBDIR ${PROJECT_SOURCE_DIR}/lib)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})
IF(NOT MSVC OR MSVC_IDE)
    MAKE_DIRECTORY(${OUTPUT_LIBDIR}/${OSG_PLUGINS})
ENDIF(NOT MSVC OR MSVC_IDE)


SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_LIBDIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})

SET(CMAKE_C_FLAGS  "-O2 -pipe -fPIC -Wall -fmessage-length=0")  
SET(CMAKE_CXX_FLAGS "-O2 -pipe -fPIC -Wall -fmessage-length=0") 

################################################################################
# User Options

# set default cmake build type to (None Debug Release RelWithDebInfo MinSizeRel)
SET( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Choose the type of build, options are: None Debug Release MinSizeRel." FORCE )

# set default install prefix to project root directory
 SET(VIRE_framework "$ENV{VIRE_framework}")
IF(VIRE_framework)
	SET( CMAKE_INSTALL_PREFIX ${VIRE_framework} CACHE PATH "Where to install ${PROJECT_NAME}"  FORCE)
ELSE()
IF (WIN32)
	SET( CMAKE_INSTALL_PREFIX "D:/workspace/VIRE_framework/5.code/trunk" CACHE PATH "Where to install ${PROJECT_NAME}" FORCE )
ELSE()
	SET( CMAKE_INSTALL_PREFIX "/home/dingo/Workspace/VIRE_framework/5.code/trunk" CACHE PATH "Where to install ${PROJECT_NAME}" FORCE )
ENDIF()
ENDIF()

# ÊÇ·ñ¹¹œšµ¥Ôª²âÊÔ whether unit tests should be build
SET (ENABLE_TESTS FALSE CACHE BOOL "Build unit tests?")

FIND_PACKAGE(Qt5Core QUIET)
FIND_PACKAGE(Qt5Widgets QUIET)
FIND_PACKAGE(Qt5Gui QUIET)
IF ( Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5Gui_FOUND )
    SET(QT_INCLUDES ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS})
	 SET(QT_ALL_LIBRARIES ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTXML_LIBRARY} )
ELSE()
    FIND_PACKAGE(Qt4)
    IF (QT4_FOUND)
        INCLUDE(${QT_USE_FILE})
		SET(QT_INCLUDES ${QT_INCLUDES} ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
        SET(QT_ALL_LIBRARIES ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTXML_LIBRARY})
    ENDIF (QT4_FOUND)
ENDIF ()


# ÔöŒÓ¹€Ÿß°üCMakeÎÄŒþ
INCLUDE(DefineCompilerFlags) 
INCLUDE(VIREUtilityMacros)
INCLUDE(FindPkgMacros)



# Disable automatic conversion from QString to ASCII 8-bit strings (char *)
# (Keeps code compatible with Qt/Mac/64bit)
	# full paths for the installation
	SET (DEFAULT_SUBDIR     ${PROJECT_SOURCE_DIR}) 
	SET (DEFAULT_BIN_SUBDIR     ${PROJECT_SOURCE_DIR}/bin) 
	SET (DEFAULT_INCLUDE_SUBDIR ${PROJECT_SOURCE_DIR}/include)
	SET (DEFAULT_LIB_SUBDIR     ${PROJECT_SOURCE_DIR}/lib)
	SET (DEFAULT_SRC_SUBDIR     ${PROJECT_SOURCE_DIR}/src)

IF (WIN32)





	IF (MSVC)
		# Turn on defines for non standard maths stuff
		ADD_DEFINITIONS(-D_USE_MATH_DEFINES)

		# Turn off deprecation warnings
		ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
		ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS)
	ENDIF(MSVC)
	 # This option is to enable the /DYNAMICBASE switch
	 # It is used to workaround a bug in Windows 7 when linking in release, which results in corrupt
	 # binaries. See this page for details: http://www.wintellect.com/CS/blogs/jrobbins/archive/2009/01/24/the-case-of-the-corrupt-pe-binaries.aspx
	 OPTION(WIN32_USE_DYNAMICBASE "Set to ON to build osgOcean with the /DYNAMICBASE option to work around a bug when linking release executables on Windows 7." OFF)
	 MARK_AS_ADVANCED(WIN32_USE_DYNAMICBASE)
	 IF(WIN32_USE_DYNAMICBASE)
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DYNAMICBASE")
	 ENDIF(WIN32_USE_DYNAMICBASE)
ENDIF()

SET (VIRE_DIR     ${DEFAULT_SUBDIR} )
SET (VIRE_BIN_DIR     ${DEFAULT_BIN_SUBDIR} )
SET (VIRE_INCLUDE_DIR ${DEFAULT_INCLUDE_SUBDIR} )
SET (VIRE_LIB_DIR     ${DEFAULT_LIB_SUBDIR} )
SET (VIRE_SRC_DIR     ${DEFAULT_SRC_SUBDIR} )
SET (VIRE_3RDPARTY_PATH "$ENV{VIRE_3RDPARTY_PATH}")

MESSAGE( STATUS "VIRE_BIN_DIR                = ${VIRE_BIN_DIR}" )
MESSAGE( STATUS "VIRE_INCLUDE_DIR            = ${VIRE_INCLUDE_DIR}" )
MESSAGE( STATUS "VIRE_LIB_DIR                = ${VIRE_LIB_DIR}" )
MESSAGE( STATUS "VIRE_SRC_DIR                = ${VIRE_SRC_DIR}" )

#############################################################
# process subdirs
ADD_SUBDIRECTORY (src)


#############################################################
# Uninstall targe Ð¶ÔØ
CONFIGURE_FILE(
  "${CMAKE_MODULE_PATH}/Templates/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
  
