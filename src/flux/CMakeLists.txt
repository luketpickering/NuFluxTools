SET(flux_implementation_files)

SET(flux_header_files
DecayParent.hxx
IDecayParentReader.hxx
INuRaySource.hxx
NuRayGun.hxx)

#add_library(flux SHARED ${flux_implementation_files})

#install(TARGETS flux DESTINATION lib)

install(FILES ${flux_header_files} DESTINATION include/flux)

add_subdirectory(input)
