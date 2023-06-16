project(cdraw)
cmake_minimum_required(VERSION 3.5)

if(CDRAW_USE_CAIRO)
	include(CMakeLists.cairo.txt)	
elseif(CDRAW_USE_QT) 
	include(CMakeLists.qt.txt)	
endif()
