#!/bin/bash
patient_number=$1
resolution=$2

output_dir="${patient_number}_patients_${resolution}_resolution_set"
mkdir -p "$output_dir"

# Chemins vers vos dossiers
patients=(patients/*.txt)
surgeries=(surgery/*.json)

for patient in $(seq 1 $patient_number)
do
  # Sélection aléatoire
  random_surgery_path="${surgeries[$RANDOM % ${#surgeries[@]}]}"
  random_patient_path="${patients[$RANDOM % ${#patients[@]}]}"
  
  # Extraction des noms (sans extension)
  patient_class=$(basename "$random_patient_path" .txt)
  surgery_type=$(basename "$random_surgery_path" .json)

  echo "Génération $patient : Patient [$patient_class] | Chirurgie [$surgery_type]"

  # 1. Génération de la séquence
  ./sequence_generator/sequence_generator "$random_surgery_path" "$random_patient_path" event_db/impact_drug event_db/impact_gesture
  
  mv *.txt "$output_dir/${patient}_sequence.txt"
  mv *.bin "$output_dir/${patient}_sequence.bin"

  # 2. Génération des séries (On utilise __ comme séparateur propre)
  # Le préfixe sera : ID__CLASSE__CHIRURGIE__
  prefix="${patient}__${patient_class}__${surgery_type}__"
  
  ./time_series_generator/complete_anesthesia "$output_dir/${patient}_sequence.bin" "$output_dir/${patient}_sequence.txt" "$random_patient_path" event_db/impact_drug/ $resolution "$prefix" 1
  
  # 3. Déplacement des fichiers
  mv *"${prefix}"* "$output_dir"/
  sleep 0.1
done