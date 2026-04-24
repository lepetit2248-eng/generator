#!/bin/bash
patient_number=$1
resolution=$2

# Création d'une variable pour le dossier de sortie pour plus de clarté
output_dir="${patient_number}_patients_${resolution}_resolution_set"
mkdir -p "$output_dir"

# Récupération de tous les fichiers de chirurgie et de patients dans des tableaux (arrays)
surgeries=(surgery/*.json)
patients=(patients/*.txt)

for patient in $(seq 1 $patient_number)
do
  # Sélection aléatoire d'une chirurgie et d'un patient pour cette itération
  random_surgery="${surgeries[$RANDOM % ${#surgeries[@]}]}"
  random_patient="${patients[$RANDOM % ${#patients[@]}]}"

  # Affichage optionnel pour savoir ce qui est généré
  echo "Génération patient $patient avec chirurgie : $random_surgery et profil : $random_patient"

  # Utilisation des variables aléatoires dans le générateur de séquence
  ./sequence_generator/sequence_generator "$random_surgery" "$random_patient" event_db/impact_drug event_db/impact_gesture
  
  mv *.txt "$output_dir/${patient}_sequence.txt"
  mv *.bin "$output_dir/${patient}_sequence.bin"

  # Utilisation de la même variable patient aléatoire dans le générateur de séries temporelles
  ./time_series_generator/complete_anesthesia "$output_dir/${patient}_sequence.bin" "$output_dir/${patient}_sequence.txt" "$random_patient" event_db/impact_drug/ $resolution "${patient}_" 1
  
  mv ${patient}*'events'* "$output_dir"/
  mv ${patient}*'series'* "$output_dir"/
  sleep 1
done