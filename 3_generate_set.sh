#!/bin/bash
patient_number=$1
resolution=$2
mkdir $patient_number'_patients_'$resolution'_resolution_set'
for patient in $(seq 1 $patient_number)
do
  ./sequence_generator/sequence_generator  surgery/LMMC002_inguinal_hernia_laparoscopic_prothesis.json patients/dossier_medical_patient_normal.txt event_db/impact_drug event_db/impact_gesture
  mv *.txt $patient_number'_patients_'$resolution'_resolution_set'/$patient'_sequence.txt'
  mv *.bin $patient_number'_patients_'$resolution'_resolution_set'/$patient'_sequence.bin'

  ./time_series_generator/complete_anesthesia $patient_number'_patients_'$resolution'_resolution_set'/$patient'_sequence.bin' $patient_number'_patients_'$resolution'_resolution_set'/$patient'_sequence.txt' patients/dossier_medical_patient_normal.txt event_db/impact_drug/ $resolution $patient'_' 1
  mv $patient*'events'* $patient_number'_patients_'$resolution'_resolution_set'/
  mv $patient*'series'* $patient_number'_patients_'$resolution'_resolution_set'/
  sleep 1
done
