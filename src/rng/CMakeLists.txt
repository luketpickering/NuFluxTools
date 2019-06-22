SET(rand_implementation_files)

SET(rand_header_files
IRandomVectors.hxx
RandomDirectionDistributions.hxx
RandomPositionDirections.hxx
RandomPositionDistributions.hxx
IEnergyDistribution.hxx
UniformEnergyDistribution.hxx)

#add_library(rng SHARED ${rand_implementation_files})

#install(TARGETS rng DESTINATION lib)

install(FILES ${rand_header_files} DESTINATION include/rng)
