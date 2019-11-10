LIST(APPEND CONVERTERS_HEADERS IFluxConverter.hxx)

if(USE_GENIE)
  LIST(APPEND CONVERTERS_IMPL ToGSimpleNtp.cxx)

  add_library(FluxConverters SHARED ${CONVERTERS_IMPL})

  target_link_libraries(FluxConverters -L${GENIE_LIB_DIR} ${GENIE_LIBS})
  target_link_libraries(FluxConverters -L${ROOT_LIBDIR} -L${PYTHIA6} ${ROOT_LIBS})

  install(TARGETS FluxConverters DESTINATION plugins)
endif()

install(FILES ${CONVERTERS_HEADERS} DESTINATION include/flux/converters)
