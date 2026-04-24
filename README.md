# BDLBS generator version 3  - 17 variables physiologiques

*Cette nouvelle version intégrer les fonctionalité suivantes*
* Gestion du minutage des événement uniquement par le générateur de séquences
* Compatibilité totale avec les chirurgies non coelioscopique
*
*

## Description du dépot
Ce dépot est composé de plusieurs répertoires dont la description est présente ci-dessous :

* *event_db* : contient la base de donnée des différents événements anesthésiques sous la forme de deux sous-répertoires : *impact_drug*, pour les médicaments et *impact_gesture*, pour les gestes médicaux
* *boost* : contient la librairie boost au format archivé et un tutoriel d'installation
* *patients* : contient le fichier de paramétrages du patient (Dossier médical)
* *sequences_generator* : contient le code source de la partie génération de séquences d'événements anesthésiques du générateur ainsi que les scripts associés. Après compilation, est présent dans ce répertoire l’exécutable de cette partie du générateur.
* *surgery* : contient les fichier de paramétrage propres aux chirurgies (grammaires)
* *time_serie_generator* : contient le code source de la partie génération de séries temporelles d'après une séquence d'événements anesthésiques du générateur ainsi que les scripts associés.Après compilation, est présent l’exécutable de cette partie du générateur.

## Générateur de profil anesthésiques

### Librairies

Pour fonctionner correctement, et être compilé, le générateur nécessite la librairie *sérialisation* de boost en version 1.72.

Cette version est fournie dans le dossier 'boost' et un script d’installation, *1_install_boost.sh*, est présent à la racine de ce dépôt pour une installation sur une distribution unix. Pour une installation sur Windows se référer à la documentation de boost ou du compilateur utilisé.

### Principe de fonctionnement

Le générateur est composé de deux parties distinctes :
- Un générateur de séquence d'événement anesthésiques
- Un générateur de séries temporelles correspondantes à une séquence d'événements anesthésiques

Chacune de ces parties correspond à un programme qui doit être compilé, à des fichiers d'entrée et des fichiers de sortie spécifiques.

### Mise en place

#### Etape 1 : Installation de la librairie *sérialisation* de boost

Deux approches sont possible :
- Soit en utilisant le script fournis *1_install_boost.sh* qui fera l'installation automatiquement (sur une distribution unix). Un accès *sudo* vous sera demandé.
- Soit en suivant le tutoriel présent dans le dossier *boost*

#### Etape 2 : Compilation

Pour compiler les deux programmes un script est présent *2_compile_all.sh*, une execution de ce script, avec les droits adaptés, va produire les deux programmes dans leurs dossier respectifs. A savoir :
- le programme *sequence_generator* dans le dossier sequence_generator : pour la partie génération d'une séquence d'événements anesthésiques.
- le programme *complete_anesthesia* dans le dossier time_serie_generator : pour la génération des séries temporelles à partir d'une séquence d'événements anesthésiques.

Pour une compilation spécifique d'un des deux programmes, un script de compilation se trouve dans leurs dossier respectifs.

#### Etape 3 : Génération d'un jeu

Pour générer un jeu de profils anesthésiques un script dédié est fourni, *3_generate_set.sh*. Ce script génére une séquence d'événements anesthésiques unique puis les série temporelles correspondantes, chaque profil anesthésiques est ainsi unique. Son exécution, avec les droits adaptés, se fait en utilisant la commande suivante :

`./3_generate_set.sh [Nombre de patients] [Résolution]`

où :
- `[Nombre de patients]` correspond au nombre de patient unique souhaité dans le jeu.
- `[Résolution]` correspond à la résolution souhaitée pour les séries temporelles. A choisir parmi [30;60;90;120] secondes entre chaque valeurs.  

Si vous souhaiter plus de détails sur le fonctionnement technique des deux parties du générateur consultez la section suivante.

### Instruction d’exécution détaillées pour chaque partie

Chaque partie du générateur a des fichiers d'entrée et des fichiers de sortie spécifiques. Une exécution sans arguments, ou avec des arguments incorrects, affichera un message d'erreur avec les arguments à saisir.   

#### Générateur de séquences d'événements anesthésiques

`./sequence_generator  [structure_chirurgie] [parametre_patient] [drug_db] [gesture_db]`

où :
- `[structure_chirurgie]` : chemin du fichier au format JSON de la structure de la chirurgie. C'est ce fichier qui est utilisé par le générateur de séries temporelles.
- `[parametre_patient]` : chemin du fichier des paramétres du patient (dossier médical).
- `[drug_db]` : chemin du dossier contenant les différents événements 'drug'.
- `[gesture_db]` : chemin du dossier contenant les différents événements 'gesture'.

L'execution du programme va générer deux fichier 'event' :
- Un fichier *.txt* qui contient les identifiants des événements présents dans la séquence et leurs horodatage. Le séparateur est le point-virgule.
- Un fichier *.bin* qui contient la séquence d'événements anesthésiques sérialisée.

#### Générateur de séries temporelles

`./complete_anesthesia [sequence_bin]  [patient_pysiological_parameters] [drug_db] [drug_db] [prefix] [iteration_number]`

où :
- `[sequence_bin]` : séquence d'événements anesthésiques sérialisée du patient.
- `[sequence_time]` : horodatage de la séquence d'événements anesthésiques.
- `[parametre_patient]` : chemin du fichier des paramétres du patient (dossier médical).
- `[drug_db]` : chemin du dossier contenant les différents événements 'drug'. Ce chemin doit se terminer par un '/'.
- `[Résolution]` : correspond à la résolution souhaitée pour les séries temporelles. A choisir parmi [30;60;90;120] secondes entre chaque valeurs.
- `[prefix]` : Préfixe pour les fichier de sortie.
- `[iteration_number]` : Nombre de lot de séries temporelles à générer à partir de la même séquence d'événements anesthésiques.

L'execution du programme va générer 2 fichiers :
- un fichier `*_series_*`, qui contient les séries temporelles en colonne avec comme séparateur la virgule.
- un fichier `*_events_*`, qui contient la séquence d'événements.

## Paramétres du patient

Le fichier de paramètrage du patient suit un formatage précis.


### Formatage d'un paramètre

Chaque paramètre est composé de deux lignes. La première ligne identifie le paramètre, la ligne suivante indique la valeur du paramètre.

#### Identifiant 

L'identifiant d'un paramètre doit obligatoirement commencer par un `#`.

- `#age` est un identifiant de paramètre valide
- `poid` n'est pas un identifiant de paramètre valide (absence du `#`)

#### Valeur du paramètre

 Cette valeurs peut étre un entier, un réel (le séparateur décimal est le point), ou un vecteur (les valeurs doivent être séparées par un espace). 

#### Exemple

Voici un exemple de paramétre valide :

`#age`
`30`

### Paramétrage des variables physiologiques

Les variables physilogiques à simuler sont à indiquer dans le fichier de paramétrage du patient (dossier médical) sous la forme de deux paramètres correspondant aux valeurs de bases et aux valeurs d'initiatlisation de la variable physiologique.

#### Identifiant spécifiques des paramètes des variables physiologiques

Les variables physiologiques ont des identifiants spécifiques qui doivent êtres présent dans  l'identifiant du paramètre.

Le formatatage de l'identifiant d'une variable physiologiques est ainsi de la forme suivante :

`#[nom de la variable]_[identifiant spécifique]` 

Où : 
- `[nom du paramètre]` est le nom de la vaiable, à choisir dans la liste présentée ci-dessous.
- `[identifiant spécifique]` est l'identifiant spécifique des valeurs de base ou d'initialisation. 

Ces identifiant spécifiques sont les suivants :
- `base_values` pour les valeurs de bases
- `begin_value` pour les valeurs d'initialisation

#### Valeurs des paramètres

Pour chaque paramètre d'une variable physiologique il faut préciser les valeurs de base et d'initialisation. 

Si une seule valeur (réel ou entier) est donnée au paramètre alors cette valeur sera sytématiquement utilisées. Si un vecteur de deux valeurs (entier ou réel) est passée en paramètre alors une valeur aléatoire tirée entre la première et la seconde valeur sera utilisée.

#### Exemple

Voici les deux paramètres pour la fréquence cardiaque 

`#FC_base_values`
`60 90`
`#FC_begin_values`
`60 90 ` 
 
#### Liste des variables possibles

Pour chaque variable, l'unité et les valeurs usuelles sont indiquées.

- `FC` : Fréquence cardiaque, en battements par minute (b.min-1), valeurs usuelle [60;90]
- `PAM` : Pression artérielle moyenne, en millimètres de mercure (mmHg), valeurs usuelle [70;105]
- `PAD` : Pression artérielle diastolique, en millimètres de mercure (mmHg), valeurs usuelle [60;90]
- `PAS` : Pression artérielle systolique , en millimètres de mercure (mmHg), valeurs usuelle [89;139]
- `temp` : Température, en degré centigrade (°C), valeurs usuelle [36.5;37.5]
- `SpO2` : Saturation en O2 dans le sang, en pourcents (%), valeurs usuelle [90;100 ]
- `FiO2` : Fraction d'O2 dans l'air inspiré, en pourcents (%), valeurs usuelle [0;35]
- `FiCO2` : Fraction de CO2 dans l'air inspiré, en pourcents (%), valeurs usuelle [0;0.04]
- `FeCO2` : Fraction de CO2 dans l'air expiré, en pourcents (%), valeurs usuelle [35;45]
- `FR` : Fréquence respiratoire, en mouvements par minute (m.min-1), valeurs usuelle [12;20]
- `Vt` : Volume de ventilation, en litres (L), valeurs usuelle [6;10]
- `Vme` : Volume moyen, en litres par minutes (L.min-1), valeurs usuelle [6;10]
- `Pmoy` : Pression moyenne, en centimètre d'eau (cmH2O), valeurs usuelle [5;10]
- `Pmax` : Pression maximale, en centimètre d'eau (cmH2O), valeurs usuelle [8;25]
- `PEP` : Pression expiratoire positive, en centimètre d'eau (cmH2O), valeurs usuelle [5;10]
- `FiHAL` : Fraction de gaz halogéné dans l'air inspiré, en pourcents (%), valeurs usuelle [5;25]
- `FeHAL` : Fraction de gaz halogéné dans l'air expiré, en pourcents (%), valeurs usuelle [2;20]


## Contact

hugo.boisaubert@univ-nantes.fr
