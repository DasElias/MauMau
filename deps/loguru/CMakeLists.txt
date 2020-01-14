add_library(loguru 
	loguru/loguru.hpp 
	loguru/loguru.cpp
)
target_link_libraries(loguru ${CMAKE_DL_LIBS})