# Optimisation Discrète - Chemin Touristique

Pacôme RENIMEL--LAMIRE
Kylian LEPLANT

## Informations importantes

Nous avons modifié la plupart du code qui avait été fourni au départ afin de le nettoyer, de le commenter proprement et d'en améliorer le confort d'utilisation.

Un effort tout particulier a été employé dans le but de reproduire finalement la fonction qui vérifie la validité d'une solution, et des tests intensifs ont été mis en place pour vérifier que son fonctionnement est en tout point similaire à celle fournie originalement. Elles sont donc parfaitement interchangeables en termes de fonctionnalités.

Pour ce qui est du temps de résolution : notre algorithme étant génétique et utilisant **toujours** les 60 secondes à notre disposition, le fichier de sortie affichera dans la colonne correspondant au temps de résolution le temps qu'il aura fallu pour parvenir à la solution finale pour cette instance. Par exemple, si la solution retournée par l'algorithme sur une instance est trouvée au bout de 32 secondes, c'est 32 qui sera affiché dans le fichier même si l'algorithme aura effectivement tourné pendant une minute complète.

Par simplicité, j'ai laissé les résultats de notre algorithme sur toutes les instances dans le fichier `sortie.txt`.

## Compiler et lancer

Avant de compiler le programme, assurez-vous que les données correspondantes aux instances sont accessibles et que vous avez rentré les chemins correspondants dans le fichier `main.cpp`.

### Première option (recommandée) : avec `xmake`

`xmake` est un remplaçant moderne à `make` et `cmake` qui peut être installé sur macOS avec la commande `brew install xmake`.
Utiliser `xmake` permet de générer un fichier `compile_commands.json` qui permettra à votre IDE de trouver les fichiers headers.

Il suffit ensuite d'exécuter la commande `xmake run` pour compiler et lancer en mode debug (à éviter), ou bien `xmake benchmark` pour lancer en mode release, c'est-à-dire avec les optimisations du compilateur.

Le fichier `xmake` a été testé et fonctionne correctement sous macOS, Windows et Linux.

Note : les optimisations du compilateur font partie du projet et de notre travail de recherche, merci de les laisser activées lors de l'évaluation.

### Deuxième option : avec `make`

Si vous n'avez pas `xmake` et que vous ne souhaitez pas l'installer, vous pouvez compiler le programme à l'aide du Makefile fourni.

Le programme peut être compilé (avec les optimisations) et lancé avec la commande `make benchmark`.

Attention ! Le Makefile n'a **pas été testé sur Windows**. Toute erreur liée à l'utilisation de Windows est honnêtement bien méritée.

### Troubleshooting

Si le projet ne fonctionne pas correctement, avant de nous enlever des points :

1. Vérifier que vous avez rentré les bons chemins de fichiers dans le fichier `main.cpp`
2. Vérifier que ces chemins sont cohérents avec ceux contenus dans `Data/data.txt`
3. Vérifier que vous n'essayez pas de compiler avec `make` et le Makefile depuis Windows
4. Vérifier que vous lancez l'exécutable depuis la racine du dossier, afin de ne pas casser les liens de fichier relatifs.
5. Si vous voyez des erreurs causées par l'inclusion des fichiers headers, vérifiez que vous avez compilé avec `xmake` et / ou que vous avez bien un fichier `compile_commands.json` qui permet à votre IDE de trouver les fichiers headers. Ce fichier n'est pas nécessaire pour la compilation et l'exécution.
