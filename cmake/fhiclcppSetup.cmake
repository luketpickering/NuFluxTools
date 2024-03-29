if(DEFINED USE_FHICL AND USE_FHICL)
  ###### FHICL set up
  ExternalProject_Add(fhiclcpp-simple
    PREFIX "${PROJECT_BINARY_DIR}/Ext"
    GIT_REPOSITORY https://github.com/luketpickering/fhiclcpp-simple.git
    GIT_TAG stable
    UPDATE_DISCONNECTED 1
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DSETUPFILENAME=fhiclcpp.setup.sh)

  LIST(APPEND EXTRA_CXX_FLAGS -DUSE_FHICL)

  ####### fhicl files
  file(GLOB FCL ${CMAKE_SOURCE_DIR}/fcl/*.fcl)
  install(FILES ${FCL} DESTINATION ${CMAKE_INSTALL_PREFIX}/fcl)

  file(GLOB FCL ${CMAKE_SOURCE_DIR}/fcl/flux/*.fcl)
  install(FILES ${FCL} DESTINATION ${CMAKE_INSTALL_PREFIX}/fcl/flux)

  SET(USE_FHICL TRUE)
else()
  SET(USE_FHICL FALSE)
endif()
