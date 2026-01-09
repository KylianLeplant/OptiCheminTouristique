# Optimisation Discrète - Chemin Touristique

Pacôme RENIMEL--LAMIRE
Kylian LEPLANT

## Informations importantes

Nous avons modifié la plupart du code qui avait été fourni au départ afin de le nettoyer, de le commenter proprement et d'en améliorer le confort d'utilisation.

Un effort tout particulier a été employé dans le but de reproduire finalement la fonction qui vérifie la validité d'une solution, et des tests intensifs ont été mis en place pour vérifier que son fonctionnement est en tout point similaire à celle fournie originalement. Elles sont donc parfaitement interchangeables en termes de fonctionnalités.

## Compiler et lancer

Avant de compiler le programme, assurez-vous que les données correspondantes aux instances sont accessibles et que vous avez rentré les chemins correspondants dans le fichier `main.cpp`.

### Première option (recommandée) : avec `xmake`

`xmake` est un remplaçant moderne à `make` et `cmake` qui peut être installé sur macOS avec la commande `brew install xmake`.
Utiliser `xmake` permet de générer un fichier `compile_commands.json` qui permettra à votre IDE de trouver les fichiers headers.

Il suffit ensuite d'exécuter la commande `xmake run` pour compiler et lancer en mode debug (à éviter), ou bien `xmake benchmark` pour lancer en mode release, c'est-à-dire avec les optimisations du compilateur.

Note : les optimisations du compilateur font partie du projet et de notre travail de recherche, merci de les laisser activées lors de l'évaluation.

### Deuxième option : avec `make`

Si vous n'avez pas `xmake` et que vous ne souhaitez pas l'installer, vous pouvez compiler le programme à l'aide du Makefile fourni.

Le programme peut être compilé (avec les optimisations) et lancé avec la commande `make benchmark`.
