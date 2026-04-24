import json
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict
import os

def generate_vertically_stacked_fan_chart(json_file, time_limit=None):
    print(f"Chargement des données depuis {json_file}...")
    
    if not os.path.exists(json_file):
        print(f"Erreur : Le fichier {json_file} est introuvable.")
        return

    with open(json_file, 'r', encoding='utf-8') as f:
        data = json.load(f)
        
    if not data:
        print("Erreur: Le fichier JSON est vide.")
        return

    # Récupération de toutes les colonnes
    colonnes = data[0]['colonnes']
    idx_time = colonnes.index("temps_sec")
    
    # On isole les variables à tracer (on exclut 'temps_sec')
    variables_a_tracer = [col for col in colonnes if col != "temps_sec"]
    
    # Création d'une figure avec une grille 8 lignes x 1 colonne
    # sharex=True aligne l'axe du temps pour tous les graphiques empilés
    fig, axes = plt.subplots(nrows=len(variables_a_tracer), ncols=1, figsize=(14, 28), sharex=True)
    
    # Titre global
    fig.suptitle("Analyse Temporelle de la Population Simulée", fontsize=22, fontweight='bold', y=0.99)

    base_color = '#1f77b4'
    line_color = '#0a3b5c'

    # Boucle sur chaque variable et son axe correspondant
    for idx_ax, variable_target in enumerate(variables_a_tracer):
        print(f"Traitement de la variable : {variable_target}...")
        ax = axes[idx_ax]
        
        idx_var = colonnes.index(variable_target)
        time_series_dict = defaultdict(list)
        
        # Agrégation des données
        for patient in data:
            for ligne in patient['donnees']:
                t = ligne[idx_time]
                val = ligne[idx_var]
                time_series_dict[t].append(val)

        times = sorted(time_series_dict.keys())
        
        if time_limit:
            times = [t for t in times if t <= time_limit * 60]

        p5, p10, p25, p50, p75, p90, p95 = [], [], [], [], [], [], []
        valid_times = []

        # Calcul des percentiles
        for t in times:
            valeurs_a_l_instant_t = time_series_dict[t]
            
            if len(valeurs_a_l_instant_t) > 5:
                valid_times.append(t / 60.0) # Conversion en minutes
                p5.append(np.percentile(valeurs_a_l_instant_t, 5))
                p10.append(np.percentile(valeurs_a_l_instant_t, 10))
                p25.append(np.percentile(valeurs_a_l_instant_t, 25))
                p50.append(np.median(valeurs_a_l_instant_t))
                p75.append(np.percentile(valeurs_a_l_instant_t, 75))
                p90.append(np.percentile(valeurs_a_l_instant_t, 90))
                p95.append(np.percentile(valeurs_a_l_instant_t, 95))

        # Tracé sur le sous-graphique spécifique
        ax.fill_between(valid_times, p5, p95, color=base_color, alpha=0.15, label='5e - 95e percentile')
        ax.fill_between(valid_times, p10, p90, color=base_color, alpha=0.25, label='10e - 90e percentile')
        ax.fill_between(valid_times, p25, p75, color=base_color, alpha=0.40, label='25e - 75e percentile')
        ax.plot(valid_times, p50, color=line_color, linewidth=2.5, label='Médiane')

        # Esthétique : Titre placé à gauche pour faire de la place
        ax.set_title(variable_target, fontsize=16, fontweight='bold', loc='left')
        
        # Unité de l'axe Y
        if variable_target == "temp":
            ax.set_ylabel("Temp (°C)", fontsize=11)
        elif variable_target == "SpO2":
            ax.set_ylabel("Sat (%)", fontsize=11)
        elif variable_target == "event_code":
            ax.set_ylabel("ID Événement", fontsize=11)
        else:
            ax.set_ylabel("Valeur", fontsize=11)
            
        ax.grid(axis='both', linestyle='--', alpha=0.7)
        
        # La légende n'apparaît que sur le tout premier graphique en haut
        if idx_ax == 0:
            ax.legend(loc='upper right', frameon=True)

    # Ajout du label de l'axe X (Temps) uniquement sur le tout dernier graphique en bas
    axes[-1].set_xlabel("Temps (Minutes)", fontsize=14, fontweight='bold')

    # Ajustement des marges pour éviter la superposition des textes
    plt.tight_layout(rect=[0, 0, 1, 0.98])

    # Sauvegarde
    filename = "dashboard_vertical_population.png"
    plt.savefig(filename, dpi=300)
    print(f"\n✅ Terminé ! Le graphique empilé a été sauvegardé sous : {filename}")
    
    plt.show()

if __name__ == "__main__":
    generate_vertically_stacked_fan_chart(json_file='db_final_meta.json', time_limit=120)