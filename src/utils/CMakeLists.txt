SET(utils_implementation_files)

SET(utils_header_files
Particle.hxx
PDGUtils.hxx
IEnergyDistribution.hxx
UniformEnergyDistribution.hxx
FileSystemUtils.hxx
RandomTools.hxx)

#add_library(utils SHARED ${utils_implementation_files})

#install(TARGETS utils DESTINATION lib)

install(FILES ${utils_header_files} DESTINATION include/utils)
