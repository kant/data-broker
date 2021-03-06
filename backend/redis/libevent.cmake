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

# sets:
#  LIBEVENT_INCLUDE_DIR
#  LIBEVENT_LIBRARIES



message("Will search for libevent includes")
find_path( LIBEVENT_INCLUDE_DIR event2/event.h
	PATH_SUFFIXES include
)

if( NOT LIBEVENT_INCLUDE_DIR )
 message( FATAL_ERROR "Libevent header files not found" )
endif( NOT LIBEVENT_INCLUDE_DIR )

message( "Found libevent headers in: ${LIBEVENT_INCLUDE_DIR}" )

find_library( LIBEVENT_LIBRARIES NAMES event
	PATH_SUFFIXES lib
)

if( NOT LIBEVENT_LIBRARIES )
 message( FATAL_ERROR "Libevent library not found" )
endif( NOT LIBEVENT_LIBRARIES )

message( "Found libevent: ${LIBEVENT_LIBRARIES}" )
