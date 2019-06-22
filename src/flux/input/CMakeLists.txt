SET(INPUT_HANDLERS_IMPL dk2nuliteReader.cxx)

add_library(FluxInputs SHARED ${INPUT_HANDLERS_IMPL})
# target_link_libraries(FluxInputs ${INPUT_HANDLERS_LINK_LIBS})

target_link_libraries(FluxInputs -L${ROOT_LIBDIR} ${ROOT_LIBS})

install(TARGETS FluxInputs DESTINATION plugins)
