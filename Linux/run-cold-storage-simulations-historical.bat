@echo off
setlocal EnableDelayedExpansion

.\Release\VarroaPop.exe ^
-v  ..\..\Simulations\VarroaPy\VarroaPy\files\exe\simplified.vrp ^
-i ..\..\Simulations\VarroaPy\VarroaPy\files\input\observed.txt -o cold-storage-historical/output.txt ^
-w ..\..\Simulations\ObservedHistoricalBinary\data_46.03125_-118.34375 ^
--binaryWeatherFileFormat Observed ^
-f --forageDayNoTemp --hourlyTemperaturesEstimation --foragersAlwaysAgeBasedOnForageInc --adultAgingBasedOnLaidEggs  --inOutEvents 

for %%s in (09/15 09/22 09/29 10/06 10/13 10/20) do (
    for %%e in (02/15 02/22 03/01 03/08 03/15 03/22) do (
        set start=%%s
        set end=%%e
        set start=!start:/=-!
        set end=!end:/=-!
        .\Release\VarroaPop.exe ^
        -v  ..\..\Simulations\VarroaPy\VarroaPy\files\exe\simplified.vrp ^
        -i ..\..\Simulations\VarroaPy\VarroaPy\files\input\observed.txt -o cold-storage-historical/output_cold_storage_!start!_!end!.txt ^
        -w ..\..\Simulations\ObservedHistoricalBinary\data_46.03125_-118.34375 ^
        --binaryWeatherFileFormat Observed ^
        -f --forageDayNoTemp --hourlyTemperaturesEstimation --foragersAlwaysAgeBasedOnForageInc --adultAgingBasedOnLaidEggs  --inOutEvents ^
        --coldStorage --coldStorageStartDate %%s --coldStorageEndDate %%e
    )
)

pause
@echo on
