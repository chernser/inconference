

add_library(inconference SHARED 			  
			 Endpoint.cpp Endpoint.h 
			 GenericServer.cpp GenericServer.h
			 library.cpp library.h
			 Mediator.cpp Mediator.h
			 SharedBuffer.cpp SharedBuffer.h
			 SystemIO.cpp SystemIO.h 
			)
target_link_libraries(inconference event_core${LIB_EVENT_CORE_VERSION})			
