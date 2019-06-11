SET(material_weighting_implementation_files)

SET(material_weighting_header_files
  IMaterialWeighter.hxx
  LinearXSecWeighter.hxx
WeightedRayPath.hxx
DensityWeighter.hxx)

#add_library(material_weighting SHARED ${material_weighting_implementation_files})

#install(TARGETS material_weighting DESTINATION lib)

install(FILES ${material_weighting_header_files} DESTINATION include/material_weighting)
