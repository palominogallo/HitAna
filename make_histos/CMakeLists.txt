# RELATIVE remove the path from file name
file(GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc)
foreach( appfile ${APP_SOURCES} )
	# I used a simple string replace, to cut off .cpp.
	string( REPLACE ".cc" "" tmpname ${appfile} )
	message(STATUS " headers=${tmpname}")
	message(STATUS " headers=${appfile}")
	add_executable(${tmpname} ${appfile} )
	target_link_libraries(${tmpname} ${ROOT_LIBRARIES} TreePlayer Geom edepsim_io HITANA )
	install ( TARGETS ${tmpname} RUNTIME DESTINATION bin )
endforeach( appfile ${APP_SOURCES} )

