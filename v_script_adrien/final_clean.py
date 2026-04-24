import os
import sys
import csv
import glob
import json

def build_event_dict(target_dir, dict_output_file="dictionnaire_evenements.json"):
    print("1. Génération du dictionnaire d'événements...")
    events = set()
    
    event_files = glob.glob(os.path.join(target_dir, "*_events_1.txt"))
    
    for e_file in event_files:
        with open(e_file, 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            next(reader, None)  
            for row in reader:
                if len(row) >= 2 and row[1].strip():
                    events.add(row[1].strip())
                    
    event_mapping = {name: idx + 1 for idx, name in enumerate(sorted(events))}
    
    with open(dict_output_file, 'w', encoding='utf-8') as f:
        json.dump(event_mapping, f, indent=4, ensure_ascii=False)
    
    print(f"   -> {len(event_mapping)} événements uniques enregistrés.")
    return event_mapping

def process_and_clean_data(target_dir, event_mapping, output_json="db_patient_final.json", min_duration_min=64):
    print(f"\n2. Traitement des patients...")
    
    series_files = sorted(glob.glob(os.path.join(target_dir, "*_series_1.txt")))
    global_data = []
    
    patients_kept = 0
    patients_rejected = 0
    
    for s_file in series_files:
        basename = os.path.basename(s_file)
        p_id_raw = basename.split('_')[0]
        patient_label = f"patient_{int(p_id_raw):04d}"
        
        # ---> LA CORRECTION EST ICI <---
        # Au lieu de deviner le nom, on remplace la balise 'series' par 'events' depuis le nom exact du fichier en cours !
        e_file = s_file.replace('_series_1.txt', '_events_1.txt')
        
        # --- ETAPE A : Lecture des événements ---
        patient_events = [] 
        if os.path.exists(e_file):
            with open(e_file, 'r', encoding='utf-8') as f:
                reader = csv.reader(f)
                next(reader, None)
                for row in reader:
                    if len(row) >= 2:
                        try:
                            t_event = int(float(row[0]))
                            e_name = row[1].strip()
                            if e_name in event_mapping:
                                patient_events.append((t_event, event_mapping[e_name]))
                        except ValueError:
                            continue
        else:
            print(f"Attention: Impossible de trouver le fichier d'événements {e_file}")
                            
        patient_events.sort(key=lambda x: x[0])
        
        # --- ETAPE B : Lecture des constantes ---
        data_rows = []
        max_time = 0
        last_t = -1 
        
        with open(s_file, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                try:
                    t = int(float(row['Time']))
                    if t < 0: 
                        continue  
                    
                    fc = int(round(float(row['FC'])))
                    pas = int(round(float(row['PAS'])))
                    pad = int(round(float(row['PAD'])))
                    pam = int(round((pas + 2 * pad) / 3.0))
                    temp = round(float(row['temp']), 1)
                    
                    spo2 = int(round(float(row['SpO2'])))
                    if spo2 > 100: spo2 = 100
                        
                    fr = int(round(float(row['FR'])))
                    
                    event_code = 0
                    for (t_ev, code) in patient_events:
                        # Si l'événement a eu lieu entre l'enregistrement précédent et l'actuel :
                        if last_t < t_ev <= t:
                            event_code = code 
                            
                    line = [t, fc, pas, pam, pad, temp, spo2, fr, event_code]
                    data_rows.append(line)
                    
                    last_t = t 
                    if t > max_time: 
                        max_time = t
                except Exception:
                    continue  
                    
        # --- ETAPE C : Filtrage ---
        duree_totale_min = max_time / 60.0
        if duree_totale_min < min_duration_min:
            patients_rejected += 1
            continue  
            
        patients_kept += 1
        global_data.append({
            "patient_id": patient_label,
            "metadata": { "duree_totale_min": round(duree_totale_min, 2) },
            "colonnes": ["temps_sec", "FC", "PAS", "PAM", "PAD", "temp", "SpO2", "FR", "event_code"],
            "donnees": data_rows
        })
        
    print("\n3. Sauvegarde des données...")
    with open(output_json, 'w', encoding='utf-8') as f:
        json.dump(global_data, f, indent=2, ensure_ascii=False)
        
    print("\n" + "="*45)
    print("                 BILAN")
    print("="*45)
    print(f"Patients conservés : {patients_kept}")
    print(f"Patients rejetés (durée < {min_duration_min} min) : {patients_rejected}")
    print("="*45)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python final_clean.py <chemin_vers_dossier_patients>")
        sys.exit(1)
        
    target_directory = sys.argv[1]
    
    mapping = build_event_dict(target_directory)
    process_and_clean_data(target_directory, mapping, output_json="db_patient_propre.json", min_duration_min=64)