#!/bin/bash

# Paramètres
nb_patient=$1
resolution=$2

# Génération du dataset
cd ..
./3_generate_set.sh $nb_patient $resolution

# Analyse des données abérantes
cd v_script_adrien/
./analyse_temp.sh temp 31
./analyse_temp.sh FC 40
./analyse_temp.sh PAS 80
./analyse_temp.sh PAM 60
./analyse_temp.sh PAD 40
./analyse_temp.sh SpO2 90
./analyse_temp.sh FR 10

# Génération de la liste d'événements
./event_list.sh

# Génération du JSON
python -u process_patient.py $nb_patient $resolution

# Correction de la PAM
python -u correct_pam.py $nb_patient