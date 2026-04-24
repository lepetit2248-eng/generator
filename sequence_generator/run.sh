#!/bin/bash
mkdir new_patient_sequence
cp $1 new_patient_sequence/$1
cp $2 new_patient_sequence/$2
./sequence_generator $1 $2 ../event_db/impact_drug ../event_db/impact_gesture
mv event* new_patient_sequence/
