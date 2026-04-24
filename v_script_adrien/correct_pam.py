import json
import sys

nb_patients = sys.argv[1] 

def correct_pam(input_file, output_file):
    # Chargement des données JSON
    with open(input_file, 'r', encoding='utf-8') as f:
        data = json.load(f)

    # Indices des colonnes basés sur la structure du fichier
    # 2: PAS, 3: PAM (à corriger), 4: PAD
    pas_idx = 2
    pam_idx = 3
    pad_idx = 4

    for patient in data:
        for entry in patient['donnees']:
            pas = entry[pas_idx]
            pad = entry[pad_idx]
            
            # Application de la formule médicale standard
            # (PAS + 2*PAD) / 3
            corrected_pam = (pas + 2 * pad) / 3
            
            # Mise à jour de la valeur arrondie à 1 décimale
            entry[pam_idx] = round(corrected_pam, 1)

    # Sauvegarde du nouveau fichier corrigé
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
    
    print(f"Correction terminée. Le fichier '{output_file}' a été généré.")

if __name__ == "__main__":
    correct_pam(f'db_patient_{nb_patients}.json', 'db_patient_corrected.json')