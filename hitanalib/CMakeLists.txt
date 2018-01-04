message(STATUS " current list dir=${CMAKE_CURRENT_LIST_DIR}")
message(STATUS " current source dir=${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE SOURCES src/*.cc)
file(GLOB_RECURSE HEADERS include/hitana/*.hh)

message(STATUS " sources=${SOURCES}")
message(STATUS " headers=${HEADERS}")

add_library(HITANA SHARED ${SOURCES})
target_include_directories(HITANA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(HITANA PUBLIC ${ROOT_LIBRARIES} TreePlayer edepsim JOYROOT)

install(TARGETS HITANA DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/hitana)
