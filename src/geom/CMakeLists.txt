SET(geom_implementation_files)

SET(geom_header_files
NuRay.hxx
Material.hxx
NuRayPropagator.hxx
BoundingCuboid.hxx)

#add_library(geom SHARED ${geom_implementation_files})

#install(TARGETS geom DESTINATION lib)

install(FILES ${geom_header_files} DESTINATION include/geom)
