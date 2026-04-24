#!/bin/bash

#Generate on patient time series
patient_sequence_bin_file=$1
patient_pysiological_parameters_file=$2
time_resolution=$3

mkdir new_patient_time_series_$3_resolution
cp $patient_sequence_bin_file new_patient_time_series_$3_resolution/
cp $patient_pysiological_parameters_file new_patient_time_series_$3_resolution/
./complete_anesthesia $patient_sequence_bin_file $patient_pysiological_parameters_file ../anesthesia_database/events_impact_drug/ $time_resolution new_patient 1
mv *.txt new_patient_time_series_$3_resolution/
