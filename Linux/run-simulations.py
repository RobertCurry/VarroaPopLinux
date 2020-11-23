import datetime
import os
import subprocess

class Location:

    def __init__(self, city, weather_filename):
        self.city = city
        self.weather_filename = weather_filename


class Weather:

    def __init__(self, type, input_filename):
        self.type = type
        self.input_filename = input_filename

    def input_filename_without_extension(self):
        return os.path.splitext(self.input_filename)[0]


places = [
    Location('walla-walla', 'data_46.03125_-118.34375'),
    Location('omak', 'data_48.40625_-119.53125'),
    Location('wenatchee', 'data_47.40625_-120.34375')
]

weathers = [
    Weather('ObservedHistoricalBinary', 'observed.txt'),
    Weather('Rcp85Binary', 'rcp85.txt')
]

build_directory = 'build/'

now = datetime.datetime.now()
output_directory = now.strftime("results-%Y%m%d%H%M%S")

python = 'py'
prefix_executable = ''
executable = 'VarroaPop.exe'
if os.name != 'nt':
    python = 'python3'
    prefix_executable = './'
    executable = 'VarroaPop'

full_executable = prefix_executable + build_directory + executable

if __name__ == '__main__':
    for place in places:
        for weather in weathers:
            sub_command = python + ' scripts/simulations.py --exe ' + full_executable
            sub_command += ' --vrp ../Simulations/VarroaPy/VarroaPy/files/exe/simplified.vrp'
            sub_command += ' --output_directory ' + os.path.join(output_directory, place.city)
            sub_command += ' --input_file ../Simulations/VarroaPy/VarroaPy/files/input/' + weather.input_filename
            sub_command += ' --weather_file ../Simulations/' + weather.type + '/' + place.weather_filename
            subprocess.call(os.path.normpath(sub_command), shell=True)
