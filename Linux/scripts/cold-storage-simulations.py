import argparse
import datetime
import multiprocessing as mp
import os
import utilities


class ExecConfiguration:
    def __init__(self):
        self.config = None


class DateConfig:
    def __init__(self, date_str):
        self.date_with_slash = date_str
        self.date_with_hyphen = date_str.replace('/', '-')

    def slash(self):
        return self.date_with_slash

    def hyphen(self):
        return self.date_with_hyphen


def run_simulation(command_str):
    # Measure simulation duration
    start_simulation = datetime.datetime.now()
    # Call simulation
    process = os.popen(command_str)
    process.close()
    # Compute elapsed time and update total simulation time
    end_simulation = datetime.datetime.now()
    elapsed_time = end_simulation - start_simulation
    utilities.safe_print('\tCommand:' + command_str + ' \n\t\tElapsed:' + '%.2f seconds' % elapsed_time.total_seconds())
    return elapsed_time.total_seconds()


# total simulation time for the record
total_simulations_time = 0
total_simulations_run = 0


def sum_simulation_time(result):
    global total_simulations_time
    global total_simulations_run
    total_simulations_time += int(result)
    total_simulations_run += 1


def simulation_error(error):
    utilities.safe_print(error)


def to_normalize_path(path):
    return r'"%s"' % path


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Cold storage simulation.')
    parser.add_argument('--exe', type=str, help='Path of the VarroaPop command line application', required=True)
    parser.add_argument('--vrp', type=str, help='Path of the vrp file to use for simulations', required=True)
    parser.add_argument('--output_directory', type=str,
                        help='Output files will be written in an autogenerated folder within OUT_DIR',
                        metavar='OUT_DIR', required=True)
    parser.add_argument('--input_directory', type=str, help='Input directory expecting IN_DIR/SCENARIO.txt',
                        metavar='IN_DIR', required=True)
    parser.add_argument('--weather_directory', type=str, help='Get weather files from WEATHER_DIRECTORY',
                        metavar='WEATHER_DIRECTORY', required=True)
    arguments = parser.parse_args()

    print('Working directory: ' + os.getcwd())

    if not os.path.isfile(arguments.exe):
        print('Cannot find VarroaPop executable at: ' + arguments.exe)
        exit(-1)

    if not os.path.isfile(arguments.vrp):
        print('Cannot find VRP file at: ' + arguments.vrp)
        exit(-1)

    if os.path.isfile(arguments.output_directory):
        print(arguments.output_directory + ' is not a directory')
        exit(-1)

    if not os.path.isdir(arguments.input_directory):
        print('Cannot find input directory at: ' + arguments.input_directory)
        exit(-1)

    if not os.path.isdir(arguments.weather_directory):
        print('Cannot find weather directory at: ' + arguments.weather_directory)
        exit(-1)

    start_dates = [
        DateConfig('09/15'),
        DateConfig('09/22'),
        DateConfig('09/29'),
        DateConfig('10/06'),
        DateConfig('10/13'),
        DateConfig('10/20')]

    end_dates = [
        DateConfig('02/15'),
        DateConfig('02/22'),
        DateConfig('02/29'),
        DateConfig('03/01'),
        DateConfig('03/08'),
        DateConfig('03/15')]

    exec_configurations = []

    default_command = arguments.exe + ' -f -v ' + to_normalize_path(arguments.vrp) + \
                      ' --forageDayNoTemp --hourlyTemperaturesEstimation --foragersAlwaysAgeBasedOnForageInc' + \
                      ' --adultAgingBasedOnLaidEggs  --inOutEvents'

    input_files_exists = {}

    # gather configurations for simulations
    weather_files = os.listdir(arguments.weather_directory)
    for weather_file in weather_files:
        info = utilities.parse_weather_filename(weather_file)
        output_directory = os.path.join(arguments.output_directory, os.path.join(info.location, info.scenario))

        # get input filename and check if it exists
        input_file = os.path.join(arguments.input_directory, info.scenario + '.txt')
        if not input_file in input_files_exists:
            input_files_exists[input_file] = os.path.exists(input_file)
        if not input_files_exists[input_file]:
            print('Missing input file ' + input_file)
            exit(-1)

        command = default_command + ' -i ' + to_normalize_path(input_file)
        command += ' -w ' + to_normalize_path(os.path.join(arguments.weather_directory, weather_file))
        command += ' --binaryWeatherFileFormat ' + utilities.get_valid_binary_format_identifier(info.scenario)

        # add configuration without cold storage
        output_filename = info.model + '_default'
        output_file = os.path.join(output_directory, output_filename + '.txt')
        exec_command = command + ' -o ' + to_normalize_path(output_file)
        exec_configurations.append(exec_command)

        # add configurations for cold storage
        for start_date in start_dates:
            for end_date in end_dates:
                output_filename = info.model + '_cold_storage_' + start_date.hyphen() + '_' + end_date.hyphen()
                output_file = os.path.join(output_directory, output_filename + '.txt')
                exec_command = command + ' -o ' + to_normalize_path(output_file)
                exec_command += ' --coldStorage --coldStorageStartDate %s --coldStorageEndDate %s' \
                                % (start_date.slash(), end_date.slash())
                exec_configurations.append(exec_command)

    # run simulations
    print('Executing Cold Storage Simulations: ')
    simulation_time = datetime.datetime.now()

    # Step 1: Init multiprocessing.Pool()
    pool = mp.Pool(mp.cpu_count())

    # Step 2: Use loop to parallelize
    for configuration in exec_configurations:
        pool.apply_async(run_simulation,
                         args=(configuration,),
                         callback=sum_simulation_time,
                         error_callback=simulation_error)

    # Step 3: Don't forget to close
    pool.close()

    # Step 4: Wait for processes to complete
    pool.join()

    print('Total duration             (s):' + '%.2f' % (datetime.datetime.now() - simulation_time).total_seconds())
    print('Total duration accumulated (s):' + '%.2f' % total_simulations_time)
    print('Total simulations executed    :' + '%d' % total_simulations_run)
