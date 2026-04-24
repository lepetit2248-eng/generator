#!/bin/bash

TARGET_DIR="../300_patients_30_resolution_set"
OUTPUT_FILE="event_list.txt"

if [ ! -d "$TARGET_DIR" ]; then
    echo "Erreur : Le dossier $TARGET_DIR est introuvable."
    exit 1
fi

echo "Analyse des événements (Format CSV : virgule)..."

# -F',' définit la virgule comme séparateur de colonne
# On affiche la deuxième colonne ($2)
awk -F',' '
    NR == 1 { next } # On ignore l en-tête (Time,Event)
    $2 != "" { print $2 } # On affiche la colonne Event si elle n est pas vide
' "$TARGET_DIR"/*_events_1.txt | sort | uniq | nl -w1 -s': ' > "$OUTPUT_FILE"

if [ -s "$OUTPUT_FILE" ]; then
    echo "Succès ! Le dictionnaire CSV est prêt."
    echo "Lieu du fichier : $(realpath "$OUTPUT_FILE")"
    echo "--- Aperçu des événements ---"
    head -n 5 "$OUTPUT_FILE"
else
    echo "Erreur : Aucun événement trouvé. Vérifiez que la virgule est bien présente."
fi