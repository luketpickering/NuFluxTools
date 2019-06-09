SET(TESTAPPS RandomPositions RandomDirections RandomNuRays TreadLoudly DumpTreadLoudlyPlots)

foreach(a ${TESTAPPS})
  add_executable(${a} ${a}.cxx)

  if(NOT "${CMAKE_LINK_FLAGS}" STREQUAL "")
    set_target_properties(${a} PROPERTIES LINK_FLAGS ${CMAKE_LINK_FLAGS})
  endif()
  target_link_libraries(${a} ${NUISANCE_LINK_DIRS})
  target_link_libraries(${a} ${NUISANCE_DEPEND_LIBS})

  install(TARGETS ${a} DESTINATION tests)
endforeach()
