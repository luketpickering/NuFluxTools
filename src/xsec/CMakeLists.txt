SET(xsec_implementation_files)

SET(xsec_header_files
IXSecProvider.hxx)

#add_library(xsec SHARED ${xsec_implementation_files})

#install(TARGETS xsec DESTINATION lib)

install(FILES ${xsec_header_files} DESTINATION include/xsec)
