SET(APPS
  NuFluxTools
)

foreach(a ${APPS})
  add_executable(${a} ${a}.cxx)

  if(NOT "${CMAKE_LINK_FLAGS}" STREQUAL "")
    set_target_properties(${a} PROPERTIES LINK_FLAGS ${CMAKE_LINK_FLAGS})
  endif()
  target_link_libraries(${a} nft_plugins -Wl,--no-as-needed nft_flux -Wl,--as-needed nft_utils)
  target_link_libraries(${a} ${NFT_LINK_DIRS})
  target_link_libraries(${a} ${NFT_DEPEND_LIBS})

  install(TARGETS ${a} DESTINATION bin)
endforeach()

add_subdirectory(tools)
