import os
import re
from threading import Lock

safe_print_lock = Lock()


# Synchronised print to use in multi-threaded script
def safe_print(*a, **b):
    # Thread safe print function
    with safe_print_lock:
        print(*a, **b)


# Class containing information about weather file information
class WeatherFileInfo:
    def __init__(self, location, model, scenario, latitude, longitude):
        self.location = location
        self.model = model
        self.scenario = scenario
        self.latitude = latitude
        self.longitude = longitude

    def __str__(self):
        return self.location + ':' + \
               '\n\tmodel: ' + self.model + \
               '\n\tscenario: ' + self.scenario + \
               '\n\tlocation: ' + self.latitude + ',' + self.longitude


# Retrieve the WeatherFileInfo from a filename when formatted as <LOCATION>_<MODEL>_<SCENARIO>_<LAT>_<LONG>
def parse_weather_filename(filename):
    try:
        location, model, scenario, latitude, longitude = filename.split('_', 5)
        return WeatherFileInfo(location, model, scenario, latitude, longitude)
    except:
        safe_print('Filename: ' + filename + ' is not formatted as <LOCATION>_<MODEL>_<SCENARIO>_<LAT>_<LONG>')


# Extracts binary format from filename if specified as <SCENARIO>-<WEATHER_FILE>.txt
def parse_binary_format(filename):
    binary_format = {'observed': 'Observed', 'modeled': 'Modeled', 'rcp85': 'Rcp85', 'rcp45': 'Rcp45'}
    filename = os.path.basename(filename)
    for item in re.split('-|/|.txt', filename):
        if item in binary_format.keys():
            return binary_format[item]
    item = os.path.splitext(os.path.basename(filename))[0]
    if item in binary_format.keys():
        return binary_format[item]
    raise Exception('No valid binary format specifier in input filename (observed|modeled|Rcp85|Rcp45)-<weather_file>.txt')


def get_valid_binary_format_identifier(binary_format):
    binary_format_identifiers = {'observed': 'Observed', 'modeled': 'Modeled', 'rcp85': 'Rcp85', 'rcp45': 'Rcp45'}
    try:
        return binary_format_identifiers[binary_format.lower()]
    except:
        safe_print(binary_format + ' is not a valid binary format Observed|Modeled|Rcp85|Rcp45')