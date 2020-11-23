import argparse
import datetime
import multiprocessing as mp
import os
import re
import utilities
import subprocess
from threading import Lock


def run_simulation(output_directory, command, configuration):
    if not configuration['Name'] and not configuration['Options']:
        raise Exception('Incorrect configuration object passed')
    output_file = configuration['Name'] + '.txt'
    output_filename = os.path.join(output_directory, output_file)
    sub_command = command + ' -o ' + output_filename + ' '
    if len(configuration['Options']) > 0:
        sub_command += ' '.join(['--{0} '.format(option) for option in configuration['Options']])
    # Measure simulation duration
    start_simulation = datetime.datetime.now()
    # Call simulation
    process = os.popen(sub_command)
    process.close()
    # Compute elapsed time and update total simulation time
    end_simulation = datetime.datetime.now()
    elapsed_time = end_simulation - start_simulation
    utilities.safe_print('\tCommand      :' + sub_command +
                         '\n\tDuration (s):' + '%.2f' % elapsed_time.total_seconds())
    return elapsed_time.total_seconds()


total_simulations_time = 0


def sum_simulation_time(result):
    global total_simulations_time
    total_simulations_time += result


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Generate plots for previously ran simulation.')
    parser.add_argument('--exe', type=str, help='Path of the VarroaPop command line application', required=True)
    parser.add_argument('--vrp', type=str, help='Path of the vrp file to use for simulations', required=True)
    parser.add_argument('--output_directory', type=str,
                        help='Output files will be written in an autogenerated folder within OUT_DIR',
                        metavar='OUT_DIR', required=True)
    parser.add_argument('--input_file', type=str, help='Run the simulation for the given input FILE', metavar='FILE',
                        required=True)
    parser.add_argument('--weather_file', type=str, help='Use WEATHER_FILE to generate data', metavar='WEATHER_FILE')
    arguments = parser.parse_args()

    command = arguments.exe + ' -f -v ' + arguments.vrp + ' -i ' + arguments.input_file + ' --binaryWeatherFileFormat '
    command += utilities.parse_binary_format(arguments.input_file)
    if arguments.weather_file:
        command += ' -w ' + arguments.weather_file

    '''
    configurations = [
        {'Name': 'Current', 'Options': []},
        {'Name': 'AdultAgingWhenEggLaid', 'Options': ['adultAgingBasedOnLaidEggs']},
        {'Name': 'AdultAgingWhenEggLaidFixedLarvaeAndBrood', 'Options': ['adultAgingBasedOnLaidEggs', 'larvaeAndBroodBecomeBeesAfterAdultsStopedAging']},
        {'Name': 'AdultAgingWhenEggLaidFixedLarvaeAndBroodFixedAdults', 'Options': ['adultAgingBasedOnLaidEggs', 'larvaeAndBroodBecomeBeesAfterAdultsStopedAging', 'forageIncThresholdForAdultsAgingWhenLaidEggs 0.5']},
        {'Name': 'HourlyTemp', 'Options': ['hourlyTemperaturesEstimation']},
        {'Name': 'HourlyTempForageDayWindAndRain', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation']},
        {'Name': 'ForagersFirst', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'pendingForagerFirst']},
        {'Name': 'AdultAgingAsForagers', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'pendingForagerFirst', 'forageDayAdultBeesAging']},
        {'Name': 'AdultAgingAsForagersProgressiveForagers', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'pendingForagerFirst', 'forageDayAdultBeesAging', 'foragersFinerAging']},
        {'Name': 'ForagersAgingAdultAgingWhenEggLaid', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'pendingForagerFirst', 'forageDayAdultBeesAging', 'adultAgingBasedOnLaidEggs', 'larvaeAndBroodBecomeBeesAfterAdultsStopedAging']},
        {'Name': 'ForagersAgingAdultAgingWhenEggLaidProgressiveForagers', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'pendingForagerFirst', 'forageDayAdultBeesAging', 'adultAgingBasedOnLaidEggs', 'larvaeAndBroodBecomeBeesAfterAdultsStopedAging', 'foragersFinerAging']},
        {'Name': 'ForagersAgingAdultAgingWhenEggLaidFixedLarvaeAndBroodFixedAdults', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'pendingForagerFirst', 'forageDayAdultBeesAging', 'adultAgingBasedOnLaidEggs', 'larvaeAndBroodBecomeBeesAfterAdultsStopedAging', 'forageIncThresholdForAdultsAgingWhenLaidEggs 0.5']}
    ]
    '''

    # let's simulate for the different options we have
    configurations = [
        {'Name': 'Current', 'Options': []},
        {'Name': 'ForagersAging', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'foragersAlwaysAgeBasedOnForageInc']},
        {'Name': 'ForagersAgingAdultAgingWhenEggLaid', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'foragersAlwaysAgeBasedOnForageInc', 'adultAgingBasedOnLaidEggs']},
        {'Name': 'ForagersAgingAdultAgingWhenEggLaid_InOut', 'Options': ['forageDayNoTemp', 'hourlyTemperaturesEstimation', 'foragersAlwaysAgeBasedOnForageInc', 'adultAgingBasedOnLaidEggs', 'inOutEvents']}
    ]

    # set prefix for output data
    prefix = os.path.splitext(os.path.basename(arguments.input_file))[0]
    # add location information to prefix if not present
    if not re.search(r'\d+.\d+_-?\d+.\d+', prefix):
        if arguments.weather_file:
            prefix += '-' + os.path.basename(arguments.weather_file)
        else:
            raise Exception('Location information need to be present either in input_file name or weather_file ex:data_46.03125_-118.34375')

    output_directory = os.path.join(arguments.output_directory, prefix)
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    print('Executing simulations for input file: ' + arguments.input_file);
    user_simulation_time = datetime.datetime.now()

    # Step 1: Init multiprocessing.Pool()
    pool = mp.Pool(mp.cpu_count())

    # Step 2: Use loop to parallelize
    for configuration in configurations:
        pool.apply_async(run_simulation, args=(output_directory, command, configuration), callback=sum_simulation_time)

    # Step 3: Don't forget to close
    pool.close()

    # Step 4: Wait for processes to complete
    pool.join()

    print('')
    print('Total duration             (s):' + '%.2f' % (datetime.datetime.now()-user_simulation_time).total_seconds())
    print('Total duration accumulated (s):' + '%.2f' % total_simulations_time)

    #print('Generating plots')
    #plotter = plots.Plotter()
    # plotter.display_temperature_data = True
    #plotter.display_activity_ratio = True
    #plotter.do_plots(output_directory, prefix)



