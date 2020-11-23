Created by Julien Pireaud 

This file is to guide you through compile VarroaPop simulator on Linux / macOS

## Compilation

Run setup.py to prepare the Linux folder to be ready to build

For C++ dependency management we use Hunter package manager.

To compile in release:

1. create a directory FOO
1. move in FOO directory
1. execute `python ../setup.py` to generate appropriate CMakeLists.txt file for datamodel
1. prepare cmake folder `cmake -DCMAKE_BUILD_TYPE=Release ..`
1. cd to parent of FOO directory
1. call `cmake --build FOO --target VarroaPop -- -j <number of threads>`
1. call program `./FOO/VarroaPop -h`

To time an execution you can use:

- On Linux: `time ./<build>/VarroaPop -d VarroaPop/files/ -v exe/default.vrp -o output/vp_output_linux.txt -i input/vp_linux.txt -f`
- On Windows (PowerShell): `Measure-Command {start-process Release\VarroaPop.exe -argumentlist "exe\default.vrp /b /or output\vp_output_win.txt /i input\vp_input.txt" -wait}` 

## Run Simulations

The options that we keep for now are:
 - `-f` to force overwriting existing files
 - `-v simplified.vrp` minimal VRP file
 - `-i rcp85-input.txt` 
 - `--binaryWeatherFileFormat Rcp85` specifies the binary format of the weather file Observed|Modeled|Rcp85|Rcp45 
 - `-w data_46.03125_-118.34375` weather file 
 - `-o output.txt` output file 
 - `--forageDayNoTemp  --hourlyTemperaturesEstimation  --foragersAlwaysAgeBasedOnForageInc  --adultAgingBasedOnLaidEggs  --inOutEvents` options to properly run the simulations with the selected aging model

## Helper scrips 

- `run-simulations.py` will run simulations for omak, wenatchee, walla-walla with historical and future weather simulations. Make sure the path to executable in the script is correct.
- `scripts/plots.py` sample script which aim to automate plots generation, this is out of date but is a good source of inspiration
- `scripts/simulations.py` sample script to call VarroaPop simulations with different sets of options

## Weather format supported 

- ObservedHistorical starting 1/1/1979 
- ModeledHistorical starting 1/1/1950
- Rcp85 starting 1/1/2006
- Rcp45 starting 1/1/2006

## Changes made to the original VarroaPop codebase

### Changes to existing codebase

- Adult
    - Changed current age from being an Integer to a Float number. Currently the m_CurrentAge attribute is not used, but it would be useful if we want to use it and age Adults with a portion of a day;
    - Added missing attributes initialisation in constructors.
- Beed
    - Added missing attributes initialisation in constructor.
- Colony
    - Added a CAdultList::Add method to be able to add new Adults to the first box card without making other Adults age;
    - Added InOutEvent structure to add addtional output data on what's moved from one aging structure to the other;
    - Changed Foragers aging structure with `--foragerAgingBasedOnHourlyTemperatureEstimate`.
    - Changed Adult aging structure with `--adultAgingBasedOnLaidEggs` option.
- Queen
    - Extracted L daylight hours based component of the egg laying equation in CQueen::ComputeL method.
- VarroaPopSession
    - Enhanced output formating by extracting parameter specification and adding delimiter afterwards.
- WeatherEvents
    - Added ability to load binary data from WSU grid weather data;
    - Added a common utility function UpdateForageAttributeForEvent to set m_ForageDay and m_ForageInc with the same logic for all types of input weather data;
    - Added functionnality to CEvent GetTemp, GetMaxTemp, GetMinTemp, IsForageDay and GetForageInc methods to behave correctly in the case we activated the CColdStorageSimulator;
    - Fixed bad interpretation of Rainfall and Windspeed for WSU grid weather files;
    - Made Windspeed and Rainfall threasholds customizable.

### Addition to existing codebase

- GlobalOptions
    - Singleton object used to set global parameters used to control behavior of simulation.
- ColdStorageSimulator
    - Singleton object used to simulate cold storage.
- WeatherGridData
    - Class used to load different WSU weather grid data files.


## Locations

- Walla Walla: 46.03125,-118.34375
- Richland: 46.28125, -119.34375
- Wenatchee: 47.40625, -120.34375
- Omak: 48.40625, -119.53125


## Roadmap

- Change the POSITION wrapper which makes allocations and I think there is a better way to do
- Change the Implementation of the COleDateTime which is not portable on Windows (not using Windows SDK) by using Boost::DateTime library
- Enhance Binary serialization feedback to reject invalid files (check if eof is reached in >> operators)
- Add a logger to have several logging levels INFO/WARNING/ERROR so that it can be easily disabled
