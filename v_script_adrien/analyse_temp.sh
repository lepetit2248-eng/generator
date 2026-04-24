#!/bin/bash

# Dossier cible
TARGET_DIR="../300_patients_30_resolution_set"

# Vérifier si le dossier existe
if [ ! -d "$TARGET_DIR" ]; then
    echo "Erreur : Le dossier $TARGET_DIR est introuvable."
    exit 1
fi

# Récupération des arguments
# On donne des noms clairs : $1 est le nom de la colonne, $2 est le seuil
target_col=$1 
threshold=$2

if [ -z "$target_col" ] || [ -z "$threshold" ]; then
    echo "Usage: $0 <nom_colonne> <seuil>"
    echo "Exemple: $0 temp 32"
    exit 1
fi

# Initialisation des compteurs
total_files=0
alert_files=0

# On utilise une sous-coquille (subshell) pour ne pas changer le dossier du terminal actuel
(
    cd "$TARGET_DIR" || exit 1

    for file in *_series_1.txt; do
        # Vérifier si des fichiers correspondent au pattern
        [ -e "$file" ] || continue
        
        ((total_files++))

        # Correction AWK : 
        # 1. On passe le NOM de la colonne (ex: "temp") via -v c_name
        # 2. On passe la VALEUR du seuil via -v t_val
        is_below=$(awk -F'[ \t,]+' -v c_name="$target_col" -v t_val="$threshold" '
            NR==1 {
                # On cherche l index de la colonne qui correspond exactement au nom demandé
                for(i=1; i<=NF; i++) {
                    if($i == c_name) { col_idx=i; break }
                }
            }
            # Si on a trouvé la colonne et que la valeur est inférieure au seuil
            NR>1 && col_idx && $col_idx != "" && $col_idx < t_val { 
                found=1; exit 
            }
            END { print (found ? 1 : 0) }
        ' "$file")

        if [ "$is_below" -eq 1 ]; then
            ((alert_files++))
        fi
    done

    # Affichage des résultats à l'intérieur de la parenthèse pour accéder aux compteurs
    echo "--- Rapport d'analyse ---"
    echo "Colonne analysée      : $target_col"
    echo "Seuil de détection    : < $threshold"
    echo "Fichiers analysés     : $total_files"
    echo "Fichiers en alerte    : $alert_files"
)