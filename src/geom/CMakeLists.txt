SET(geom_implementation_files)

SET(geom_header_files
IRandomVectors.hxx
NuRay.hxx
RandomConeDirection.hxx
RandomCuboidVolume.hxx
RandomRectangularPlane.hxx
Material.hxx)

#add_library(geom SHARED ${geom_implementation_files})

#install(TARGETS geom DESTINATION lib)

install(FILES ${geom_header_files} DESTINATION include/geom)
