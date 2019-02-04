
add_compile_options("-std=c++11")
set(CMAKE_CXX_STANDARD 11)


add_library(inconference SHARED
			 library.cpp library.h 			  
			 Endpoint.cpp Endpoint.h 
			 GenericServer.cpp GenericServer.h
			 Mediator.cpp Mediator.h
			 SharedBuffer.cpp SharedBuffer.h
			 SystemIO.cpp SystemIO.h
			 BasicHttpd.cpp BasicHttpd.h
			 utils.cpp utils.h
			)
target_link_libraries(inconference event_core)			
