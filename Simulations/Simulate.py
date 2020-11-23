# -*- coding: utf-8 -*-
"""
Created on Sun Oct  6 18:27:47 2019

@author: Reet Barik
"""

import os
os.chdir("D:/Coursework/PACCAR/VarroaPOP/Beepop/Simulations/")

import numpy as np
import pandas as pd
from datetime import datetime
import math
import json
from VarroaPy.VarroaPy.RunVarroaPop import VarroaPop
import matplotlib.pyplot as plt

def photoperiod(latitude, date):
    day = datetime.strptime(str(date), "%Y-%m-%d %H:%M:%S")
    J = day.timetuple().tm_yday
    
    P = math.asin(0.39795 * math.cos(0.2163108 + 2 * math.atan(0.9671396 * math.tan(0.00860 * (J - 186)))))
    D = 24 - (24 / math.pi) * math.acos((math.sin(0.833 * math.pi / 180) + (math.sin(latitude * math.pi / 180) * math.sin(P))) / (math.cos(latitude * math.pi / 180) * math.cos(P)))
    return D
    

obsHist = True
modHist = False
rcp = False

weatherFile = ""
Location = 'Richland'

if Location == 'Omak':
    weatherFile = 'data_48.40625_-119.53125'
if Location == 'Wenatchee':
    weatherFile = 'data_47.40625_-120.34375'
if Location == 'Richland':
    weatherFile = 'data_46.28125_-119.34375'
if Location == 'WallaWalla':
    weatherFile = 'data_46.03125_-118.34375'
    
if obsHist:    
    dt = np.dtype([('PPT', 'ushort'), ('TMAX', 'short'), ('TMIN', 'short'), ('WIND', 'short'), ('SPH', 'short'), ('SRAD', 'short'), ('RMAX', 'short'), ('RMIN', 'short')])
    data = np.fromfile('ObservedHistoricalBinary/' + weatherFile, dtype=dt)
if modHist:    
    dt = np.dtype([('PPT', 'ushort'), ('TMAX', 'short'), ('TMIN', 'short'), ('WIND', 'short')])
    data = np.fromfile('ModeledHistoricalBinary/' + weatherFile, dtype=dt)
if rcp:    
    dt = np.dtype([('PPT', 'ushort'), ('TMAX', 'short'), ('TMIN', 'short'), ('WIND', 'short')])
    data = np.fromfile('Rcp85Binary/' + weatherFile, dtype=dt)
    

df = pd.DataFrame(data)

if obsHist:
    df['PPT'] = df['PPT'] / 40
    df['TMAX'] = df['TMAX'] / 100
    df['TMIN'] = df['TMIN'] / 100
    df['WIND'] = df['WIND'] / 100
    df['SPH'] = df['SPH'] / 1000
    df['SRAD'] = df['SRAD'] /40
    df['RMAX'] = df['RMAX'] / 100
    df['RMIN'] = df['RMIN'] /100
else:
    df['PPT'] = df['PPT'] / 40
    df['TMAX'] = df['TMAX'] / 100
    df['TMIN'] = df['TMIN'] / 100
    df['WIND'] = df['WIND'] / 100


columns = ['Date', 'Max', 'Min', 'Ave', 'Wind', 'Rain']

weather = pd.DataFrame(columns=columns)

if obsHist:
    weather['Date'] = pd.date_range(start='1/1/1979', periods=len(df), freq='D')
if modHist:
    weather['Date'] = pd.date_range(start='1/1/1950', periods=len(df), freq='D')
if rcp:
    weather['Date'] = pd.date_range(start='1/1/2006', periods=len(df), freq='D')

weather['Max'] = df['TMAX']
weather['Min'] = df['TMIN']
weather['Ave'] = (weather['Max'] + weather['Min']) / 2
weather['Wind'] = df['WIND']
weather['Rain'] = df['PPT']

latitude = float(weatherFile.split('_')[1])

weather['Hrs light'] = weather['Date'].apply(lambda x: photoperiod(latitude, x))

f = open("VarroaPy/VarroaPy/files/weather/" + weatherFile + ".wth","w") 
f.write("WEATHER_FILE\n")
f.write("Temperature Scale . . . . . . . . .  = C\n")
f.write("Weather File Name . . . . . . . . .  = " +  weatherFile + ".wth\n")
f.write("Format of the Weather File  . . . .  = MINMAX\n")
f.write("Begin Time of the Weather . . . . .  = 12:00\n")
f.write("Begin Date of the Weather . . . . .  = " + weather['Date'][0].strftime("%#m/%#d/%y") + "\n")
f.write("Ending Time of the Weather  . . . .  = 12:00\n")
f.write("Ending Date of the Weather  . . . .  = " + weather['Date'][len(weather) - 1].strftime("%#m/%#d/%y") + "\n")
f.write("Start Time of the Simulation  . . .  = 12:00\n")
f.write("Start Date of the Simulation  . . .  = " + weather['Date'][0].strftime("%#m/%#d/%y") + "\n")
f.write("Column # of the Temperature . . . .  = 4\n")
f.write("Column # of the Max Temperature . .  = 2\n")
f.write("Column # of the Min Temperature . .  = 3\n")
f.write("Column # of the Daylight . . . . . . = 7\n")
f.write("Column # of the Wind Speed  . . . .  = 5\n")
f.write("Column # of the Rain  . . . . . . .  = 6\n")
f.write("Interval Between Readings . . . . .  = 1440\n")
f.write("----------begin---------\n")

weather['Date'] = weather['Date'].apply(lambda x: x.strftime("%#m/%#d/%Y"))
weather = weather.set_index('Date')
f.write(weather.to_string())       
f.close()

with open('parameters.json', 'r') as f:
    params = json.load(f)
    
    
vp = VarroaPop(parameters = params, weather_file = "VarroaPy/VarroaPy/files/weather/" + weatherFile + ".wth")
output = vp.run_model()
output = output.drop(output.index[0])
output['Inactive Foragers'] = output['Colony Size'] - output['Adult Drones'] - output['Adult Workers'] - output['Foragers']

variableList = ['Colony Size', 'Total Eggs', 'DD', 'L', 'N', 'P', 'dd', 'l', 'n']
variable = variableList[0]

output['%Colony'] = output['Colony Size'] / output['Colony Size'] * 100 

ax = output.plot(y = variable, color = 'b', legend = False, alpha = 0.75)
ax.axvspan(1, 31, facecolor='gray', alpha=0.3)
ax.axvspan(60, 90, facecolor='gray', alpha=0.3)
ax.axvspan(121, 151, facecolor='gray', alpha=0.3)
ax.axvspan(182, 212, facecolor='gray', alpha=0.3)
ax.axvspan(244, 273, facecolor='gray', alpha=0.3)
ax.axvspan(305, 334, facecolor='gray', alpha=0.3)
ax.axvspan(366, 396, facecolor='gray', alpha=0.3)
ax.axvspan(425, 455, facecolor='gray', alpha=0.3)
ax.axvspan(486, 516, facecolor='gray', alpha=0.3)


for year in range(2002, 2011):
    params["SimStart"] = "01/01/" + str(year)
    params["SimEnd"] = "06/30/" + str(year + 1)
    
    vp = VarroaPop(parameters = params, weather_file = "VarroaPy/VarroaPy/files/weather/" + weatherFile + ".wth")
    output = vp.run_model()
    output = output.drop(output.index[0])
    output['Inactive Foragers'] = output['Colony Size'] - output['Adult Drones'] -output['Adult Workers'] - output['Foragers']
    
    output.plot(y = variable, ax = ax, color = 'b', legend = False, alpha = 0.75)

#ax.set_ylim(0, 150)
plt.title(variable + ' vs Time')
#plt.savefig('Plots/' + Location + '/OnePlus_Stacked_Colony_Size_2001%.png', dpi = 1000)
#plt.close()
    
    
obsHist = False
modHist = False
rcp = True

    
if Location == 'Omak':
    weatherFile = 'data_48.40625_-119.53125'
if Location == 'Wenatchee':
    weatherFile = 'data_47.40625_-120.34375'
if Location == 'Richland':
    weatherFile = 'data_46.28125_-119.34375'
if Location == 'WallaWalla':
    weatherFile = 'data_46.03125_-118.34375'
    
if obsHist:    
    dt = np.dtype([('PPT', 'ushort'), ('TMAX', 'short'), ('TMIN', 'short'), ('WIND', 'short'), ('SPH', 'short'), ('SRAD', 'short'), ('RMAX', 'short'), ('RMIN', 'short')])
    data = np.fromfile('ObservedHistoricalBinary/' + weatherFile, dtype=dt)
if modHist:    
    dt = np.dtype([('PPT', 'ushort'), ('TMAX', 'short'), ('TMIN', 'short'), ('WIND', 'short')])
    data = np.fromfile('ModeledHistoricalBinary/' + weatherFile, dtype=dt)
if rcp:    
    dt = np.dtype([('PPT', 'ushort'), ('TMAX', 'short'), ('TMIN', 'short'), ('WIND', 'short')])
    data = np.fromfile('Rcp85Binary/' + weatherFile, dtype=dt)
    

df = pd.DataFrame(data)

if obsHist:
    df['PPT'] = df['PPT'] / 40
    df['TMAX'] = df['TMAX'] / 100
    df['TMIN'] = df['TMIN'] / 100
    df['WIND'] = df['WIND'] / 100
    df['SPH'] = df['SPH'] / 1000
    df['SRAD'] = df['SRAD'] /40
    df['RMAX'] = df['RMAX'] / 100
    df['RMIN'] = df['RMIN'] /100
else:
    df['PPT'] = df['PPT'] / 40
    df['TMAX'] = df['TMAX'] / 100
    df['TMIN'] = df['TMIN'] / 100
    df['WIND'] = df['WIND'] / 100


columns = ['Date', 'Max', 'Min', 'Ave', 'Wind', 'Rain']

weather = pd.DataFrame(columns=columns)

if obsHist:
    weather['Date'] = pd.date_range(start='1/1/1979', periods=len(df), freq='D')
if modHist:
    weather['Date'] = pd.date_range(start='1/1/1950', periods=len(df), freq='D')
if rcp:
    weather['Date'] = pd.date_range(start='1/1/2006', periods=len(df), freq='D')

weather['Max'] = df['TMAX']
weather['Min'] = df['TMIN']
weather['Ave'] = (weather['Max'] + weather['Min']) / 2
weather['Wind'] = df['WIND']
weather['Rain'] = df['PPT']

latitude = float(weatherFile.split('_')[1])

weather['Hrs light'] = weather['Date'].apply(lambda x: photoperiod(latitude, x))

f = open("VarroaPy/VarroaPy/files/weather/" + weatherFile + ".wth","w") 
f.write("WEATHER_FILE\n")
f.write("Temperature Scale . . . . . . . . .  = C\n")
f.write("Weather File Name . . . . . . . . .  = " +  weatherFile + ".wth\n")
f.write("Format of the Weather File  . . . .  = MINMAX\n")
f.write("Begin Time of the Weather . . . . .  = 12:00\n")
f.write("Begin Date of the Weather . . . . .  = " + weather['Date'][0].strftime("%#m/%#d/%y") + "\n")
f.write("Ending Time of the Weather  . . . .  = 12:00\n")
f.write("Ending Date of the Weather  . . . .  = " + weather['Date'][len(weather) - 1].strftime("%#m/%#d/%y") + "\n")
f.write("Start Time of the Simulation  . . .  = 12:00\n")
f.write("Start Date of the Simulation  . . .  = " + weather['Date'][0].strftime("%#m/%#d/%y") + "\n")
f.write("Column # of the Temperature . . . .  = 4\n")
f.write("Column # of the Max Temperature . .  = 2\n")
f.write("Column # of the Min Temperature . .  = 3\n")
f.write("Column # of the Daylight . . . . . . = 7\n")
f.write("Column # of the Wind Speed  . . . .  = 5\n")
f.write("Column # of the Rain  . . . . . . .  = 6\n")
f.write("Interval Between Readings . . . . .  = 1440\n")
f.write("----------begin---------\n")

weather['Date'] = weather['Date'].apply(lambda x: x.strftime("%#m/%#d/%Y"))
weather = weather.set_index('Date')
f.write(weather.to_string())       
f.close()

with open('parameters.json', 'r') as f:
    params = json.load(f)
    
params["SimStart"] = "01/01/2090"
params["SimEnd"] = "06/30/2091" 

vp = VarroaPop(parameters = params, weather_file = "VarroaPy/VarroaPy/files/weather/" + weatherFile + ".wth")
output = vp.run_model()
output = output.drop(output.index[0])
output['Inactive Foragers'] = output['Colony Size'] - output['Adult Drones'] - output['Adult Workers'] - output['Foragers']

variableList = ['Colony Size', 'Total Eggs', 'DD', 'L', 'N', 'P', 'dd', 'l', 'n']
variable = variableList[0]

output['%Colony'] = output['Colony Size'] / output['Colony Size'] * 100 

ax = output.plot(y = variable, color = 'b', legend = False, alpha = 0.75)
ax.axvspan(1, 31, facecolor='gray', alpha=0.3)
ax.axvspan(60, 90, facecolor='gray', alpha=0.3)
ax.axvspan(121, 151, facecolor='gray', alpha=0.3)
ax.axvspan(182, 212, facecolor='gray', alpha=0.3)
ax.axvspan(244, 273, facecolor='gray', alpha=0.3)
ax.axvspan(305, 334, facecolor='gray', alpha=0.3)
ax.axvspan(366, 396, facecolor='gray', alpha=0.3)
ax.axvspan(425, 455, facecolor='gray', alpha=0.3)
ax.axvspan(486, 516, facecolor='gray', alpha=0.3)


for year in range(2090, 2100):
    params["SimStart"] = "01/01/" + str(year)
    params["SimEnd"] = "06/30/" + str(year + 1)
    
    vp = VarroaPop(parameters = params, weather_file = "VarroaPy/VarroaPy/files/weather/" + weatherFile + ".wth")
    output = vp.run_model()
    output = output.drop(output.index[0])
    output['Inactive Foragers'] = output['Colony Size'] - output['Adult Drones'] -output['Adult Workers'] - output['Foragers']
    
    output.plot(y = variable, ax = ax, color = 'b', legend = False, alpha = 0.75)

#ax.set_ylim(0, 150)
plt.title(variable + ' vs Time')
#plt.savefig('Plots/' + Location + '/OnePlus_Stacked_Colony_Size_2001%.png', dpi = 1000)
#plt.close()
plt.savefig('Plots/' + Location + '/DD_ObsHist_OnePlus_Overlay_Superimposed_2001_2092.png', dpi = 1000)
plt.close()
