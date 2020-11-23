import filecmp, os, re, shutil, stat
from string import Template

def get_linux_root():
    return os.path.dirname(os.path.abspath(__file__))


def get_data_model_root():
    return os.path.abspath(os.path.join(get_linux_root(), "datamodel"))


def get_port_code_root():
    return os.path.abspath(os.path.join(get_linux_root(), "portcode"))


def get_project_root():
    return os.path.abspath(os.path.join(get_linux_root(), ".."))


def is_source_file(filename):
    return os.path.splitext(filename)[1].lower() in [".cpp", ".cxx", ".c"]


def is_header_file(filename):
    return os.path.splitext(filename)[1].lower() in [".h", ".hpp"]


def is_code_file(filename):
    return is_header_file(filename) or is_source_file(filename)


def set_file_as_readonly(path):
    if os.path.exists(path):
        readonly_mode = stat.S_IREAD | stat.S_IRGRP
        os.chmod(path, readonly_mode)
    

def set_file_as_writable(path):
    if os.path.exists(path):
        readonly_mode = stat.S_IREAD | stat.S_IRGRP
        readwrite_mode = readonly_mode | stat.S_IWRITE | stat.S_IWGRP
        os.chmod(path, readwrite_mode)


def get_data_model_files():
    return [
        "Adult.h",
        "Adult.cpp",
        "Bee.h",
        "Bee.cpp",
        "Brood.h",
        "Brood.cpp",
        "Colony.h",
        "Colony.cpp",
        "ColdStorageSimulator.h",
        "ColdStorageSimulator.cpp",
        "ColonyResource.h",
        "ColonyResource.cpp",
        "CreateWeatherHdr.h",
        "CreateWeatherHdr.cpp",
        "DateRangeValues.h",
        "DateRangeValues.cpp",
        "EGG.H",
        "EGG.CPP",
        "EPAData.h",
        "EPAData.cpp",
        "GlobalOptions.h",
        "GlobalOptions.cpp",
        "IEDItem.h",
        "IEDItem.cpp",
        "LARVA.H",
        "LARVA.CPP",
        "Matrix.h",
        "Matrix.cpp",
        "Mite.h",
        "Mite.cpp",
        "MiteTreatmentItem.h",
        "MiteTreatmentItem.cpp",
        "MiteTreatments.h",
        "MiteTreatments.cpp",
        "NutrientContaminationTable.h",
        "NutrientContaminationTable.cpp",
        "Queen.h",
        "Queen.cpp",
        "Spores.h",
        "Spores.cpp",
        "VarroaPopSession.h",
        "VarroaPopSession.cpp",
        "WeatherEvents.h",
        "WeatherEvents.cpp",
        "WeatherGridData.h",
        "WeatherGridData.cpp"
    ]

def get_port_code_headers():
    headers = []
    dir_it = os.scandir(get_port_code_root()) 
    for dir_item in dir_it:
        if dir_item.is_file and dir_item.name.endswith('.h'):
            headers.append(dir_item.name)
    return headers


def fix_include_directives(source, target):
    with open(source, 'r') as code_file:
        code_file_content = code_file.read()
        result = re.findall(r'#include\s+\"(.*)\"', code_file_content)
    set_file_as_writable(target)
    if len(result):
        with open(target, 'w') as code_file:
            port_code_headers = get_port_code_headers()
            for include in result:
                if any(file.lower() == include.lower() for file in get_data_model_files()) or any(file == include.lower() for file in port_code_headers):
                    code_file_content = code_file_content.replace(include, include.lower())
            code_file.write(code_file_content)
    else:
        shutil.copy(source, target)
    set_file_as_readonly(target)


def copy_data_model():
    # check existance of data model file and if they changed
    data_model_dir = get_project_root()
    # data_model_target_dir will be the location of the working header files
    data_model_target_dir = get_data_model_root()
    # data_model_target_original_dir will be the location of the original copied files used for comparison 
    data_model_target_original_dir = os.path.join(data_model_target_dir, ".hidden")
    if os.path.exists(data_model_target_original_dir) == False:
        os.mkdir(data_model_target_original_dir)
    for data_model_file in get_data_model_files():
        data_model_path = os.path.join(data_model_dir, data_model_file)
        data_model_target_path = os.path.join(data_model_target_dir, data_model_file.lower())
        data_model_target_path_original = os.path.join(data_model_target_original_dir, data_model_file.lower())
        if os.path.exists(data_model_dir):
            target_file_exists = os.path.exists(data_model_target_path_original)
            if  target_file_exists == False or filecmp.cmp(data_model_path, data_model_target_path_original) == False:
                set_file_as_writable(data_model_target_path_original)
                shutil.copy(data_model_path, data_model_target_path_original)
                set_file_as_readonly(data_model_target_path_original)
                fix_include_directives(data_model_target_path_original, data_model_target_path)
                print("Copying file {} to {}".format(data_model_path, data_model_target_path))
        else:
            print("Cannot copy file {} to {} because it is missing".format(data_model_path, data_model_target_path))


def make_data_model_cmakelists():
    cmake_filename = "CMakeLists.txt"
    cmake_template_filename = "CMakeLists.txt.template"
    # extract source files from the files to be added to datamodel
    source_files = [file for file in get_data_model_files() if is_source_file(file)]
    source_files = [s.lower() for s in source_files] # set all source file name lowercase
    # load the template CMakeLists.txt.template
    cmake_template = os.path.join(get_data_model_root(), cmake_template_filename)
    with open(cmake_template) as cmake_template_file:
        cmake_template_content = Template(cmake_template_file.read())
        cmake_template_content = cmake_template_content.substitute(VarroaPopDataModelSourceFiles=str(' '.join(source_files)))
    # keep track weither or not we should replace the content of the cmake file
    write_cmake_target_file = True
    cmake_target = os.path.join(get_data_model_root(), cmake_filename)
    if os.path.exists(cmake_target):
        with open(cmake_target) as cmake_target_file: # check content
            cmake_target_content = cmake_target_file.read()
            if cmake_target_content == cmake_template_content:
                write_cmake_target_file = False
    if write_cmake_target_file == True: # write CMakeLists.txt
        with open(cmake_target, 'w') as cmake_target_file:
            cmake_target_file.write(cmake_template_content)
            print("Writing file {} to {}".format(cmake_filename, get_data_model_root()))
    else:
        print("{} is up-to-date".format(cmake_filename))


if __name__ == "__main__":
    copy_data_model()
    make_data_model_cmakelists()

