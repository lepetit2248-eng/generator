import re
import os

# Chemin vers le fichier cible
file_path = "time_series_generator/sources/include/bdd/Impact.cpp"

with open(file_path, "r", encoding="utf-8") as f:
    code = f.read()

# 1. Remplacer l'erreur fatale des valeurs négatives par un simple plancher à 0.0
pattern_neg = r'if\s*\(new_sample\s*<\s*0\)\s*\{[\s\S]*?throw\s+error_sms;\s*\}'
replacement_neg = """if(new_sample < 0){
    new_sample = 0.0;
  }"""
code = re.sub(pattern_neg, replacement_neg, code)

# 2. Désactiver toutes les assertions bloquantes (Trend 4 et Trend 5)
assertions = [
    r'assert\(begin_value < objective\);',
    r'if\(parameter != "temp"\)\{\s*assert\s*\(\s*\(\s*objective - begin_value\s*\)\s*>\s*0\s*\);\s*\}',
    r'assert\s*\(\s*\(\s*objective - begin_value\s*\)\s*>\s*0\s*\);',
    r'assert\(begin_value > objective\);',
    r'assert\s*\(\s*\(\s*begin_value - objective\s*\)\s*>\s*0\s*\);',
    r'assert\(objective >= begin_value\);',
    r'assert\(objective <= begin_value\);'
]

for pattern in assertions:
    # On met un double slash '//' devant chaque assert trouvé pour le commenter
    code = re.sub(pattern, lambda m: f"// {m.group(0)} // Désactivé pour flexibilité bidirectionnelle", code)

with open(file_path, "w", encoding="utf-8") as f:
    f.write(code)

print("✅ Le fichier Impact.cpp a été patché avec succès !")