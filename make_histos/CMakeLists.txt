add_executable (Pi0Histos Pi0Histos.cc)
target_link_libraries(Pi0Histos ${ROOT_LIBRARIES} TreePlayer edepsim JOYROOT HITANA)

install (TARGETS Pi0Histos RUNTIME DESTINATION bin)
#install (TARGETS InfoHistos RUNTIME DESTINATION bin)
