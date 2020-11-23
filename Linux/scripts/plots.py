import argparse
import os
import pandas as pd
import matplotlib.pyplot as plt
from string import Template


column_names = ["Date", "Colony Size","Adult Drones","Adult Workers","Foragers", "Active Foragers", "Capped Drone Brood", "Capped Worker Brood",
             "Drone Larvae", "Worker Larvae", "Drone Eggs", "Worker Eggs", "Total Eggs", "DD", "L", "N", "P", "dd", "l", "n", "Free Mites", "Drone Brood Mites",
             "Worker Brood Mites", "Mites/Drone Cell", "Mites/Worker Cell", "Mites Dying", "Proportion Mites Dying",
             "Colony Pollen (g)", "Pollen Pesticide Concentration", "Colony Nectar", "Nectar Pesticide Concentration",
             "Dead Drone Larvae", "Dead Worker Larvae", "Dead Drone Adults", "Dead Worker Adults", "Dead Foragers",
             "Queen Strength", "Average Temperature (celsius)", "Rain", "Min Temp", "Max Temp", "Daylight hours", "Forage Inc"]

# original verion of varroapop

column_names_original = ["Date", "Colony Size","Adult Drones","Adult Workers", "Foragers", "Capped Drone Brood", "Capped Worker Brood",
             "Drone Larvae", "Worker Larvae", "Drone Eggs", "Worker Eggs", "Total Eggs", "DD", "L", "N", "P", "dd", "l", "n", "Free Mites", "Drone Brood Mites",
             "Worker Brood Mites", "Mites/Drone Cell", "Mites/Worker Cell", "Mites Dying", "Proportion Mites Dying",
             "Colony Pollen (g)", "Pollen Pesticide Concentration", "Colony Nectar", "Nectar Pesticide Concentration",
             "Dead Drone Larvae", "Dead Worker Larvae", "Dead Drone Adults", "Dead Worker Adults", "Dead Foragers",
             "Queen Strength", "Average Temperature (celsius)", "Rain"]

class Plotter:

    display_temperature_data = False
    display_activity_ratio = False
    start_date = None
    end_date = None

    def do_plot(self, output_directory, output_filename):
        global column_names
        global column_names_original
        # override column_names if working with previous version of the VarroaPop application
        #column_names = column_names_original

        # read output file skipping the first 6 lines
        output = pd.read_table(output_filename, delim_whitespace=True, header=None, names=column_names, skiprows=6)
        # drop the line number column
        output = output.drop(output.index[0])
        # build a datetime column to be able to restraint to a specific time period
        date_time_series = pd.to_datetime(output["Date"], format='%m/%d/%Y')
        output["Datetime"] = date_time_series
        # remove the prefix from output file name to get the plot title
        # plot_title = os.path.splitext(output_file)[0][len(prefix)+1:]
        # if len(plot_title) == 0:
        #     plot_title = 'current'
        plot_title = os.path.splitext(os.path.basename(output_filename))[0]
        # add Inactive Foragers column
        output['Inactive Foragers'] = output['Foragers'] - output['Active Foragers']
        #colunms = ['Colony Size', 'Adult Workers', 'Adult Drones', 'Foragers', 'Worker Larvae', 'Worker Eggs', 'Capped Worker Brood']
        colunms = ['Colony Size', 'Adult Workers', 'Adult Drones', 'Foragers']

        # original data lines
        # output['Inactive Foragers'] = output['Colony Size'] - output['Adult Drones'] - output['Adult Workers'] - output['Foragers']
        # colunms = ['Colony Size', 'Adult Workers', 'Adult Drones', 'Foragers', 'Inactive Foragers']

        plt.figure()

        bees_plot_pd = output.plot(x='Datetime', y=colunms, legend=True, figsize=(20, 10), title=plot_title)
        bees_plot_pd.set_ylabel('Bees')

        # Display Temperature Data

        if self.display_temperature_data and 'Min Temp' in output.keys():
            # Use weighted averages to reduce noise of min and max temperature data
            #output['Min Temp'] = output['Min Temp'].ewm(span=7, adjust=True).mean()
            min_temp_plot_pd = output.plot(x='Datetime', y='Min Temp', secondary_y=True, color='lightgrey', ax=bees_plot_pd)
            #output['Max Temp'] = output['Max Temp'].ewm(span=7, adjust=True).mean()
            max_temp_plot_pd = output.plot(x='Datetime', y='Max Temp', secondary_y=True, color='darkgrey', ax=bees_plot_pd)
            min_temp_plot_pd.axhline(12, color="lightgrey", linestyle="--")
            max_temp_plot_pd.axhline(43.3, color="lightgrey", linestyle="--")

        # Display Active Ration
        if self.display_activity_ratio:
            # output['Activity Ratio'] = output['Activity Ratio'].ewm(span=7, adjust=True).mean()
            ratio_plot = output.plot(x='Datetime', y='Forage Inc', secondary_y=True, color='darkgrey', ax=bees_plot_pd)
            ratio_plot.set_ylabel('Forage Inc')

        # The following line allow us to get a specific window from the data
        if self.start_date is not None and self.end_date is not None:
            bees_plot_pd.set_xlim(pd.Timestamp(self.start_date), pd.Timestamp(self.end_date))

        plt.savefig(os.path.join(output_directory, plot_title + '.svg'), format='svg')
        plt.close()

    def do_plots(self, directory, prefix):
        global column_names

        # gather valid output files
        output_files = []
        for entry in os.scandir(directory):
            if entry.name.startswith(prefix) and os.path.isfile(os.path.join(directory, entry.name)):
                output_files.append(entry.name)

        # make sure plots directory exists
        plots_directory = os.path.join(directory, prefix)
        if not os.path.exists(plots_directory):
            os.makedirs(plots_directory)

        for output_file in output_files:
            output_file_path = os.path.join(directory, output_file)
            self.do_plot(plots_directory, output_file_path)

        # Aggregate plots in an html file
        html_template = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'plots.html')
        with open(html_template) as html_template_file:
            html_template_content = Template(html_template_file.read())
            img_tag = "<img src='" + prefix + "/{0}.svg'/>"
            html_template_content = html_template_content.substitute(title=prefix, images='\n'.join([
                img_tag.format(os.path.splitext(output_file)[0]) for output_file in output_files
            ]))
            html_target = os.path.join(directory, 'plots.html')
            with open(html_target, 'w') as html_target_file:
                html_target_file.write(html_template_content)


if __name__ == '__main__':
    plotter = Plotter()

    parser = argparse.ArgumentParser(description='Generate plots for previously ran simulation.')
    parser.add_argument('--directory', type=str, help='directory where output files are located', required=True)
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--prefix', type=str, help='prefix of the output files in the same simulation')
    group.add_argument('--simulation_output', type=str, help='if this option is specified a single plot is generated')
    optional_group = parser.add_mutually_exclusive_group()
    optional_group.add_argument('--display_temperature_data', action='store_true', default=False,
                                help='Display Min / Max temperature data in output graph')
    optional_group.add_argument('--display_activity_ratio', action='store_true', default=False,
                                help='Display Activity Ratio')
    date_group = parser.add_argument_group()
    date_group.add_argument('--start_date', default=None, help='Start date for output graphs using YYYY-MM-DD format')
    date_group.add_argument('--end_date', default=None, help='End date for output graphs using YYYY-MM-DD format')
    arguments = parser.parse_args()

    # move options values to Plotter class
    plotter.display_temperature_data = arguments.display_temperature_data
    plotter.display_activity_ratio = arguments.display_activity_ratio
    plotter.start_date = arguments.start_date
    plotter.end_date = arguments.end_date

    if not arguments.simulation_output:
        plotter.do_plots(arguments.directory, arguments.prefix)
    else:
        plotter.do_plot(arguments.directory, arguments.simulation_output)

