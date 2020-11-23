@echo off
setlocal EnableDelayedExpansion

.\Release\VarroaPop.exe ^
-v  ..\..\Simulations\VarroaPy\VarroaPy\files\exe\simplified.vrp ^
-i ..\..\Simulations\VarroaPy\VarroaPy\files\input\rcp85.txt -o cold-storage-future/output.txt ^
-w ..\..\Simulations\Rcp85Binary\data_46.03125_-118.34375 ^
-f --forageDayNoTemp --hourlyTemperaturesEstimation --foragersAlwaysAgeBasedOnForageInc --adultAgingBasedOnLaidEggs  --inOutEvents --binaryWeatherFileFormat Rcp85

for %%s in (09/15 09/22 09/29 10/06 10/13 10/20) do (
    for %%e in (02/15 02/22 03/01 03/08 03/15 03/22) do (
        set start=%%s
        set end=%%e
        set start=!start:/=-!
        set end=!end:/=-!
        .\Release\VarroaPop.exe ^
        -v  ..\..\Simulations\VarroaPy\VarroaPy\files\exe\simplified.vrp ^
        -i ..\..\Simulations\VarroaPy\VarroaPy\files\input\rcp85.txt -o cold-storage-future/output_cold_storage_!start!_!end!.txt ^
        -w ..\..\Simulations\Rcp85Binary\data_46.03125_-118.34375 ^
        -f --forageDayNoTemp --hourlyTemperaturesEstimation --foragersAlwaysAgeBasedOnForageInc --adultAgingBasedOnLaidEggs  --inOutEvents --binaryWeatherFileFormat Rcp85 ^
        --coldStorage --coldStorageStartDate %%s --coldStorageEndDate %%e
    )
)

pause
@echo on
