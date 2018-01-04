file( GLOB APP_SOURCES *.cc )
foreach( appfile ${APP_SOURCES} )
	# I used a simple string replace, to cut off .cpp.
	string( REPLACE ".cc" "" tmpname ${appfile} )
	add_executable( ${tmpname} ${appfile} )
	target_link_libraries( ${tmpname} ${ROOT_LIBRARIES} TreePlayer edepsim JOYROOT HITANA )
	install ( TARGETS ${tmpname} RUNTIME DESTINATION bin )
endforeach( appfile ${APP_SOURCES} )

