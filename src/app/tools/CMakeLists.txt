LIST(APPEND APP_TOOLS_HEADERS INFTTool.hxx)

if(USE_GENIE)
  LIST(APPEND APP_TOOLS_IMPL Convert.cxx)

  add_library(Tools SHARED ${APP_TOOLS_IMPL})

  if(USE_GENIE)
    target_link_libraries(Tools -L${GENIE_LIB_DIR} ${GENIE_LIBS})
  endif()
  target_link_libraries(Tools -L${ROOT_LIBDIR} -L${PYTHIA6} ${ROOT_LIBS})

  install(TARGETS Tools DESTINATION plugins)
endif()

install(FILES ${APP_TOOLS_HEADERS} DESTINATION include/app/tools)
