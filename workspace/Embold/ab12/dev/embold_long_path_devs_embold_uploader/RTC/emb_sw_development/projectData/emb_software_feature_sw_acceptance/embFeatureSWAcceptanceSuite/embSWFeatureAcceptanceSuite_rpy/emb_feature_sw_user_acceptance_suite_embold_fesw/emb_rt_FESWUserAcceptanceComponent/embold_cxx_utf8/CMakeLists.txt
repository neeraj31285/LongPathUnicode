CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(emboldCxx)

if(UNIX)
	set (CMAKE_CXX_STANDARD 14)
endif()
if(WIN32)
	set (CMAKE_CXX_STANDARD 17)
endif()

set(LLVM_SRC "" CACHE PATH "")

if (LLVM_SRC STREQUAL "") 
message(FATAL_ERROR ":Required LLVM source directory path")
endif () 

SET(LLVM_DIR_NAME "LLVM")
SET(CLANG_DIR_NAME "CLANG")

STRING(REGEX MATCH "llvm-.*.src$" LLVM_DIR_NAME ${LLVM_SRC})
STRING(REPLACE "llvm" "clang" CLANG_DIR_NAME ${LLVM_DIR_NAME})
MESSAGE(STATUS ${LLVM_DIR_NAME})
MESSAGE(STATUS ${CLANG_DIR_NAME})

if(WIN32)
set(LLVM_BUILD "" CACHE PATH "")
set(BOOST_ROOT "" CACHE PATH "")

if (LLVM_BUILD STREQUAL "") 
message(FATAL_ERROR ":Required LLVM build directory path")
endif () 

if (BOOST_ROOT STREQUAL "") 
message(FATAL_ERROR ":Required Boost source directory path")
endif () 
INCLUDE_DIRECTORIES(${LLVM_SRC}/include)
INCLUDE_DIRECTORIES(${LLVM_SRC}/tools/${CLANG_DIR_NAME}/include)
INCLUDE_DIRECTORIES(${LLVM_SRC}/tools/${CLANG_DIR_NAME}/tools/extra)
INCLUDE_DIRECTORIES(${LLVM_BUILD}/include)
INCLUDE_DIRECTORIES(${LLVM_BUILD}/tools/${CLANG_DIR_NAME}/include)

# Note: All the link_directories should be before add_executable command.
#LINK_DIRECTORIES(${LLVM_BUILD}/$(CONFIGURATION)/lib)
endif(WIN32)

if(UNIX)
set(ENABLE_COVERAGE "OFF" CACHE INTERNAL "OFF")
# Ignored unhelpful warning regarding type attribute.
# Its a bug in gcc Bug 39159 - unhelpful attribute warning on matching declaration after definition
add_definitions( "-Wno-attributes" )
set( CMAKE_CXX_FLAGS "-static-libstdc++" )
endif()

#############################################################################
#                embold-cl Executable                                       #
#############################################################################

SET(CXX_LIB_NAME emboldCxx)

ADD_LIBRARY(${PROJECT_NAME} STATIC "")

# Define the precompiled header header file
set(PCH_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/inc/emboldCxxPch.h")

# Specify the source file that will include the PCH (can be any source file)
set(PCH_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/EmboldTidyModule.cpp")

# Set the precompiled header options
target_precompile_headers(emboldCxx PRIVATE ${PCH_HEADER})

#############################################################################
#                  Internal Includes                                        # 
#############################################################################
INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(inc/rules/c2012)
INCLUDE_DIRECTORIES(inc/rules/c2012/using_db)
INCLUDE_DIRECTORIES(inc/rules/checks)
INCLUDE_DIRECTORIES(inc/rules/common)
INCLUDE_DIRECTORIES(inc/rules/cpp2008)
INCLUDE_DIRECTORIES("../common/inc")

##############################################################################
#                  Third-party Includes                                      #
##############################################################################
INCLUDE_DIRECTORIES("../ThirdParty/logger")
INCLUDE_DIRECTORIES("../ThirdParty")
INCLUDE_DIRECTORIES("../ThirdParty/log4cpp/include")
INCLUDE_DIRECTORIES("${BOOST_ROOT}")

if (UNIX)
###############################################################################
#               LLVM                                                          #
###############################################################################
set( CLANG_COMPILE_FLAGS "-DNDEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D__STRICT_ANSI__")
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CLANG_COMPILE_FLAGS}" )

find_package(LLVM REQUIRED CONFIG)
if (LLVM_FOUND)
	INCLUDE_DIRECTORIES(${LLVM_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${LLVM_SRC}/tools/${CLANG_DIR_NAME}/tools/extra)
  ADD_DEFINITIONS(${LLVM_DEFINITIONS})
#  llvm_map_components_to_libnames(llvm_libs core analysis codegen libdriver support)
#	TARGET_LINK_LIBRARIES(${CXX_LIB_NAME} ${llvm_libs})
endif (LLVM_FOUND)
###############################################################################
#               Boost                                                         #
###############################################################################
set(Boost_USE_STATIC_LIBS ON) # only find static libs
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.60.0 COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${CXX_LIB_NAME} 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)
endif()
#               LOG4CPP                                                           #
###############################################################################
find_package(Log4cpp REQUIRED)
if (Log4cpp_FOUND)
  include_directories(${Log4cpp_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES(${CXX_LIB_NAME} ${Log4cpp_LIBRARIES})
  MESSAGE(STATUS ${Log4cpp_LIBRARIES})
endif (Log4cpp_FOUND)

endif (UNIX)
INCLUDE(src/CMakeLists.txt)
