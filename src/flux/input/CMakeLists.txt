SET(INPUT_HANDLERS_IMPL dk2nuliteReader.cxx)

if(USE_DK2NU)
  LIST(APPEND INPUT_HANDLERS_IMPL Dk2NuReader.cxx)
endif()

add_library(FluxInputs SHARED ${INPUT_HANDLERS_IMPL})
# target_link_libraries(FluxInputs ${INPUT_HANDLERS_LINK_LIBS})

target_link_libraries(FluxInputs -L${ROOT_LIBDIR} -L${PYTHIA6} ${ROOT_LIBS})

install(TARGETS FluxInputs DESTINATION plugins)
