library(data.table)
library(dplyr)
library(tidyverse)
library(lubridate)
library(ggplot2)
library(chillR)
library(geosphere)
library(suncalc)

options(digits=9)
options(digit=9)

read_binary <- function(file_path, hist, no_vars){
  ######    The modeled historical is in /data/hydro/jennylabcommon2/metdata/maca_v2_vic_binary/
  ######    modeled historical is equivalent to having 4 variables, and years 1950-2005
  ######
  ######    The observed historical is in 
  ######    /data/hydro/jennylabcommon2/metdata/historical/UI_historical/VIC_Binary_CONUS_to_2016
  ######    observed historical is equivalent to having 8 variables, and years 1979-2016
  ######
  if (hist) {
    if (no_vars==4){
      start_year <- 1950
      end_year <- 2005
    } else {
      start_year <- 1979
      end_year <- 2015
    }
  } else{
    start_year <- 2006
    end_year <- 2099
  }
  ymd_file <- create_ymdvalues(start_year, end_year)
  data <- read_binary_addmdy(file_path, ymd_file, no_vars)
  return(data)
}

read_binary_addmdy <- function(filename, ymd, no_vars){
  if (no_vars==4){
    return(read_binary_addmdy_4var(filename, ymd))
  } else {return(read_binary_addmdy_8var(filename, ymd))}
}

read_binary_addmdy_8var <- function(filename, ymd){
  Nofvariables <- 8 # number of variables or column in the forcing data file
  Nrecords <- nrow(ymd)
  ind <- seq(1, Nrecords * Nofvariables, Nofvariables)
  fileCon  <-  file(filename, "rb")
  temp <- readBin(fileCon, integer(), size = 2, n = Nrecords * Nofvariables,
                  endian = "little")
  dataM <- matrix(0, Nrecords, 8)
  k <- 1
  dataM[1:Nrecords, 1] <- temp[ind] / 40.00         # precip data
  dataM[1:Nrecords, 2] <- temp[ind + 1] / 100.00    # Max temperature data
  dataM[1:Nrecords, 3] <- temp[ind + 2] / 100.00    # Min temperature data
  dataM[1:Nrecords, 4] <- temp[ind + 3] / 100.00    # Wind speed data
  dataM[1:Nrecords, 5] <- temp[ind + 4] / 10000.00  # SPH
  dataM[1:Nrecords, 6] <- temp[ind + 5] / 40.00     # SRAD
  dataM[1:Nrecords, 7] <- temp[ind + 6] / 100.00    # Rmax
  dataM[1:Nrecords, 8] <- temp[ind + 7] / 100.00    # RMin
  AllData <- cbind(ymd, dataM)
  # calculate daily GDD  ...what? There doesn't appear to be any GDD work?
  colnames(AllData) <- c("year", "month", "day", "precip", "tmax", "tmin",
                         "windspeed", "SPH", "SRAD", "Rmax", "Rmin")
  close(fileCon)
  return(AllData)
}

read_binary_addmdy_4var <- function(filename, ymd) {
  Nofvariables <- 4 # number of variables or column in the forcing data file
  Nrecords <- nrow(ymd)
  ind <- seq(1, Nrecords * Nofvariables, Nofvariables)
  fileCon <-  file(filename, "rb")
  temp <- readBin(fileCon, integer(), size = 2, n = Nrecords * Nofvariables,
                  endian="little")
  dataM <- matrix(0, Nrecords, 4)
  k <- 1
  dataM[1:Nrecords, 1] <- temp[ind] / 40.00       # precip data
  dataM[1:Nrecords, 2] <- temp[ind + 1] / 100.00  # Max temperature data
  dataM[1:Nrecords, 3] <- temp[ind + 2] / 100.00  # Min temperature data
  dataM[1:Nrecords, 4] <- temp[ind + 3] / 100.00  # Wind speed data
  
  AllData <- cbind(ymd, dataM)
  # calculate daily GDD  ...what? There doesn't appear to be any GDD work?
  colnames(AllData) <- c("year", "month", "day", "precip", "tmax", "tmin",
                         "windspeed")
  close(fileCon)
  return(AllData)
}

create_ymdvalues <- function(data_start_year, data_end_year){
  Years <- seq(data_start_year, data_end_year)
  nYears <- length(Years)
  daycount_in_year <- 0
  moncount_in_year <- 0
  yearrep_in_year <- 0
  
  for (i in 1:nYears){
    ly <- leap_year(Years[i])
    if (ly == TRUE){
      days_in_mon <- c(31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
    }
    else{
      days_in_mon <- c(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
    }
    
    for (j in 1:12){
      daycount_in_year <- c(daycount_in_year, seq(1, days_in_mon[j]))
      moncount_in_year <- c(moncount_in_year, rep(j, days_in_mon[j]))
      yearrep_in_year <- c(yearrep_in_year, rep(Years[i], days_in_mon[j]))
    }
  }
  
  daycount_in_year <- daycount_in_year[-1] #delete the leading 0
  moncount_in_year <- moncount_in_year[-1]
  yearrep_in_year <- yearrep_in_year[-1]
  ymd <- cbind(yearrep_in_year, moncount_in_year, daycount_in_year)
  colnames(ymd) <- c("year", "month", "day")
  return(ymd)
}

filePathPrefix <- ''
file <- ''
location <- 'Richland'
hist <- TRUE
vars <- 8

if (location == 'Omak')
  file <- 'data_48.40625_-119.53125'
if (location == 'Wenatchee')
  file <- 'data_47.40625_-120.34375'
if (location == 'Richland')
  file <- 'data_46.28125_-119.34375'
if (location == 'WallaWalla')
  file <- 'data_46.03125_-118.34375'

if (hist) {
  filePathPrefix <- "D:/Coursework/PACCAR/VarroaPOP/Beepop/Simulations/ObservedHistoricalBinary/"
} else {
  filePathPrefix <- "D:/Coursework/PACCAR/VarroaPOP/Beepop/Simulations/Rcp85Binary/"
  vars <- 4
}
  
outputfilePath <- paste0('D:/Coursework/PACCAR/VarroaPOP/Beepop/Simulations/', file, '.csv')

met_data <- read_binary(file_path = paste0(filePathPrefix, file),
                          hist = hist, no_vars=vars)

# I make the assumption that lat always has same number of decimal points
lat <- as.numeric(substr(x = file, start = 6, stop = 13))
lon <- as.numeric(substr(x = file, start = 15, stop =24))

# data frame required
met_data <- as.data.frame(met_data)

# 3b. Clean it up
# rename needed columns

data.table::setnames(met_data, old=c("year","month", "day", "tmax", "tmin"), 
                               new=c("Year", "Month", "Day", "Tmax", "Tmin"))

met_data <- met_data %>%
            select(-c(precip, windspeed)) %>%
            data.frame()

# 3c. Get hourly interpolation
# generate hourly data
met_hourly <- stack_hourly_temps(weather = met_data,
                                 latitude = lat)
met_hourly <- met_hourly$hourtemps

met_data['HRSFLYtemp'] <- 0

for (row in 1:nrow(met_data)) {
  
  year <- met_data[row, "Year"]
  month <- met_data[row, "Month"]
  day <- met_data[row, "Day"]
  
  onThatDay <- filter(met_hourly, Year == year & Month == month & Day == day)
  
  sunrise = as.POSIXlt(getSunlightTimes(date = as.Date(strptime(paste(onThatDay$Year[1], onThatDay$Month[1], onThatDay$Day[1], sep = '-'), "%Y-%m-%d")), lat = lat, lon = lon, tz = 'America/Los_Angeles')$sunrise)
  sunset = as.POSIXlt(getSunlightTimes(date = as.Date(strptime(paste(onThatDay$Year[1], onThatDay$Month[1], onThatDay$Day[1], sep = '-'), "%Y-%m-%d")), lat = lat, lon = lon, tz = 'America/Los_Angeles')$sunset)
  
  onThatDay <- filter(onThatDay, Hour >= sunrise$hour & Hour <= sunset$hour)
  
  count <- sum(onThatDay$Temp > 10)
  rm(onThatDay)
  met_data$HRSFLYtemp[row] = count
  
}

met_data['HRSlight'] = daylength(lat, as.character(strptime(paste(met_data$Year, met_data$Month, met_data$Day, sep = '-'), "%Y-%m-%d")))

HRSsolstice = max(met_data$HRSlight)

#met_data['PROPHRSFLYtemp'] <- met_data['HRSFLYtemp'] / met_data['HRSlight']


#for (row in 1:nrow(met_data)) {
  
#  if(met_data$PROPHRSFLYtemp[row] > 1) {
#    met_data$PROPHRSFLYtemp[row] = 1
#  }
#   
#}

#met_data['HRSFLY'] = met_data['PROPHRSFLYtemp'] * 24

met_data['PROPFLIGHTDAY'] <- met_data['HRSFLYtemp'] / HRSsolstice

met_data['SUMFLIGHTDAY'] <- 0

met_data$SUMFLIGHTDAY[1] = met_data$PROPFLIGHTDAY[1]

for (row in 2:nrow(met_data)) {
  
  met_data$SUMFLIGHTDAY[row] = ifelse(met_data$PROPFLIGHTDAY[row] + met_data$SUMFLIGHTDAY[row - 1] > 14, 0, met_data$PROPFLIGHTDAY[row] + met_data$SUMFLIGHTDAY[row - 1])
  
}

met_data['CURPROPFLIGHTDAY'] <- 0

for (row in 1:nrow(met_data)) {
  
  met_data$CURPROPFLIGHTDAY[row] = ifelse(met_data$PROPFLIGHTDAY[row] > 0, 1, met_data$CURPROPFLIGHTDAY[row])
  
}

met_data['CURSUMFLIGHTDAY'] <- 0

met_data$CURSUMFLIGHTDAY[1] = met_data$CURPROPFLIGHTDAY[1]

for (row in 2:nrow(met_data)) {
  
  met_data$CURSUMFLIGHTDAY[row] = ifelse(met_data$CURPROPFLIGHTDAY[row] + met_data$CURSUMFLIGHTDAY[row - 1] > 14, 0, met_data$CURPROPFLIGHTDAY[row] + met_data$CURSUMFLIGHTDAY[row - 1])
  
}

write.csv(met_data,'D:/Coursework/PACCAR/VarroaPOP/Beepop/Simulations/Richland.csv', row.names = FALSE)

