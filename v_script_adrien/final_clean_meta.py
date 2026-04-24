import os
import sys
import csv
import glob
import json

def build_event_dict(target_dir):
    print("1. Création du dictionnaire d'événements...")
    events = set()
    event_files = glob.glob(os.path.join(target_dir, "*_events_1.txt"))
    for e_file in event_files:
        with open(e_file, 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            next(reader, None)  
            for row in reader:
                if len(row) >= 2: events.add(row[1].strip())
    mapping = {name: idx + 1 for idx, name in enumerate(sorted(events))}
    return mapping

def process_and_clean_data(target_dir, event_mapping, output_json="db_patient_propre.json"):
    print(f"2. Nettoyage et incorporation des métadonnées...")
    series_files = sorted(glob.glob(os.path.join(target_dir, "*_series_1.txt")))
    global_data = []

    for s_file in series_files:
        basename = os.path.basename(s_file)
        
        # On découpe le nom du fichier selon notre séparateur '__'
        # Format attendu : ID__CLASSE__CHIRURGIE__parameters..._series_1.txt
        parts = basename.split('__')
        if len(parts) < 3:
            continue # Sécurité si le nom de fichier ne correspond pas
            
        p_id_raw = parts[0]
        p_class = parts[1]
        p_surgery = parts[2]
        
        patient_label = f"patient_{int(p_id_raw):04d}"
        e_file = s_file.replace('_series_1.txt', '_events_1.txt')
        
        # --- A. Lecture des événements ---
        patient_events = []
        if os.path.exists(e_file):
            print(f"   -> Traitement des événements pour {patient_label}...")
            with open(e_file, 'r', encoding='utf-8') as f:
                reader = csv.reader(f); next(reader)
                for row in reader:
                    if len(row) >= 2:
                        patient_events.append((int(float(row[0])), event_mapping.get(row[1].strip(), 0)))
        patient_events.sort(key=lambda x: x[0])

        # --- B. Traitement des constantes ---
        data_rows = []
        max_time = 0
        last_t = -1
        with open(s_file, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                t = int(float(row['Time']))
                if t < 0: continue
                
                # Calculs et arrondis
                pas, pad = int(round(float(row['PAS']))), int(round(float(row['PAD'])))
                pam = int(round((pas + 2 * pad) / 3.0))
                spo2 = min(100, int(round(float(row['SpO2']))))
                temp = round(float(row['temp']), 1)
                
                # Matching événements
                event_code = 0
                for (t_ev, code) in patient_events:
                    if last_t < t_ev <= t: event_code = code
                
                data_rows.append([t, int(round(float(row['FC']))), pas, pam, pad, temp, spo2, int(round(float(row['FR']))), event_code])
                last_t, max_time = t, max(max_time, t)

        # --- C. Filtrage (64 min) et Sauvegarde avec Métadonnées complètes ---
        duree_min = max_time / 60.0
        if duree_min >= 64:
            global_data.append({
                "patient_id": patient_label,
                "metadata": {
                    "patient_class": p_class,
                    "surgery_type": p_surgery, # <--- Nouvelle métadonnée
                    "duree_totale_min": round(duree_min, 2)
                },
                "colonnes": ["temps_sec", "FC", "PAS", "PAM", "PAD", "temp", "SpO2", "FR", "event_code"],
                "donnees": data_rows
            })

    with open(output_json, 'w', encoding='utf-8') as f:
        json.dump(global_data, f, indent=2, ensure_ascii=False)
    print(f"✅ JSON généré avec {len(global_data)} patients et leurs métadonnées.")

if __name__ == "__main__":
    if len(sys.argv) < 2: sys.exit("Usage: python final_clean.py <dossier>")
    dir_path = sys.argv[1]
    mapping = build_event_dict(dir_path)
    process_and_clean_data(dir_path, mapping)