 #
 # Copyright © 2018 IBM Corporation
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #    http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

enable_testing()
cmake_minimum_required(VERSION 2.8)

if (APPLE)
  cmake_policy(SET CMP0042 OLD)
  link_directories(/usr/local/lib)
endif()

project (DataBroker NONE)
enable_language( C ) # limit cmake to only use the C toolchain

if(NOT DEFINED APPLE)
  add_definitions(-Werror)
#  add_definitions(-Wall)
endif( NOT DEFINED APPLE )
#add_definitions(-v)

if(DEFINED DEVMODE)
  add_definitions(-Wextra -Wenum-compare -Wno-unused-parameter)
endif(DEFINED DEVMODE)

if( DEFINED DBR_DEBUG_LVL )
  add_definitions( -DDEBUG_LEVEL=${DBR_DEBUG_LVL} )
endif( DEFINED DBR_DEBUG_LVL )

if( DEFINED VOIDTAG )
else( DEFINED VOIDTAG )
  add_definitions( -DDBR_INTTAG )
endif( DEFINED VOIDTAG )

set(TRANSPORT_LIBS dbbe_transport )
set(DATABROKER_LIB databroker databroker_int)

include_directories(src/util)
include_directories(include)
include_directories(backend)

add_subdirectory(bindings)
add_subdirectory(backend)


set(BACKEND_LIBS dbbe_redis )
set(TEST_BACKEND ${BACKEND_LIBS} )
set(BACKEND_LIST ${BACKEND_LIBS} )

add_subdirectory(src)
add_subdirectory(test)

install(FILES include/libdatabroker.h
    include/errorcodes.h
	DESTINATION include
)
