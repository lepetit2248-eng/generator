import json
import csv
import os
import sys

# Paramètres globaux
nb_patients = sys.argv[1] 
resolution = sys.argv[2]


def get_event_mapping(mapping_file):
    """Charge le dictionnaire global des événements {Nom: Index}."""
    mapping = {}
    if not os.path.exists(mapping_file):
        print(f"Erreur : {mapping_file} introuvable.")
        return mapping
    with open(mapping_file, 'r', encoding='utf-8') as f:
        for line in f:
            if ':' in line:
                idx, name = line.split(':', 1)
                mapping[name.strip()] = int(idx.strip())
    return mapping

def process_all_patients():
    target_dir = f"../{nb_patients}_patients_{resolution}_resolution_set"
    mapping_file = "event_list.txt"
    output_file = f"db_patient_{nb_patients}.json"
    
    event_mapping = get_event_mapping(mapping_file)
    global_data = [] # Liste qui contiendra tous les objets patients

    # Lister et trier les fichiers pour un traitement ordonné
    all_files = os.listdir(target_dir)
    series_files = sorted([f for f in all_files if f.endswith("_series_1.txt")])

    print(f"Début du traitement de {len(series_files)} patients...")

    for s_file in series_files:
        # Extraction de l'ID par split sur le premier '_'
        p_id_raw = s_file.split('_')[0]
        if not p_id_raw.isdigit():
            continue
        
        patient_label = f"patient_{p_id_raw.zfill(4)}"
        
        # Chemins des fichiers
        events_file = s_file.replace("_series_1.txt", "_events_1.txt")
        s_path = os.path.join(target_dir, s_file)
        e_path = os.path.join(target_dir, events_file)
        
        if not os.path.exists(e_path):
            continue

        # 1. Charger les événements du patient
        patient_events = {}
        with open(e_path, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                try:
                    t = int(row['Time'])
                    event_name = row['Event'].strip()
                    patient_events[t] = event_mapping.get(event_name, 0)
                except: continue

        # 2. Traiter les séries (Constantes sans virgule)
        data_rows = []
        max_time = 0
        
        with open(s_path, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                try:
                    t = int(float(row['Time']))
                    if t < 0: continue
                    
                    # On utilise int(round(float(...))) pour supprimer les virgules proprement
                    line = [
                        t,
                        int(round(float(row['FC']))),
                        int(round(float(row['PAS']))),
                        int(round(float(row['PAM']))),
                        int(round(float(row['PAD']))),
                        round(float(row['temp']),1),
                        int(round(float(row['SpO2']))),
                        int(round(float(row['FR']))), 
                        patient_events.get(t, 0)
                    ]
                    data_rows.append(line)
                    if t > max_time: max_time = t
                except: continue

        # 3. Ajouter l'objet patient à la liste globale
        global_data.append({
            "patient_id": patient_label,
            "metadata": { "duree_totale_min": round(max_time / 60, 2) },
            "colonnes": ["temps_sec", "FC", "PAS", "PAM", "PAD", "temp", "SpO2", "FR", "event_code"],
            "donnees": data_rows
        })

    # 4. Écriture du fichier JSON
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(global_data, f, indent=2, ensure_ascii=False)
    
    print(f"Terminé ! Fichier généré : {output_file}")

if __name__ == "__main__":
    process_all_patients()