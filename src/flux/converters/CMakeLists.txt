LIST(APPEND CONVERTERS_HEADERS IFluxConverter.hxx)

if(USE_DK2NU)
  LIST(APPEND CONVERTERS_IMPL ToDk2Nu.cxx)
endif()

if(USE_GENIE)
  LIST(APPEND CONVERTERS_IMPL ToGSimpleNtp.cxx)


endif()

LIST(LENGTH CONVERTERS_IMPL NCONVI)
if(NCONVI GREATER 0)
  add_library(FluxConverters SHARED ${CONVERTERS_IMPL})

  if(USE_GENIE)
    target_link_libraries(FluxConverters -L${GENIE_LIB_DIR} ${GENIE_LIBS})
  endif()

  target_link_libraries(FluxConverters -L${ROOT_LIBDIR} -L${PYTHIA6} ${ROOT_LIBS})

  install(TARGETS FluxConverters DESTINATION plugins)
endif()

install(FILES ${CONVERTERS_HEADERS} DESTINATION include/flux/converters)
