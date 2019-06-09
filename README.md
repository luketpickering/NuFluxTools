# NuFluxTools

Tools for predicting neutrino flux through a window/volume from decay parent hadrons. 
Aim to build front ends for in-use beamsim formats.

Statistical tools for combining systematically varied flux predictions into easy-to-use flux uncertainties.

Aim to build generator-agnostic flux/geometry drivers for interation position sampling.

#Build and begin
```
#Ensure your root environment is initialized
git clone https://github.com/luketpickering/NuFluxTools.git
cd NuFluxTools
mkdir build
cd build
cmake ../
make install
source Linux/setup.sh
#Test apps live in Linux/tests, which is not added to the path, add it with
add_to_PATH Linux/tests
#Run a simple ray-tracing test
TreadLoudly
#Plot diagnostic plots
DumpTreadLoudlyPlots
```
