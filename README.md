# DeepEngine

Moteur de jeux en cours de développement.

## Structuration

Le moteur est constitué de 2 projets :
- **DeepEngineCore** : *la bibliothèque de code.*
- **DeepEngine** : *le logiciel permettant la création de jeux en utilisant le DeepEngineCore.*

Même si l'utilisation du **DeepEngineCore** à lui seul permet de créer des jeux, il est recommandé d'utiliser le **DeepEngine** car il fourni des outils spécialements conçus pour s'intégrer facilement avec le moteur.

## Compilation

Afin de simplifier le processus de compilation, CMake est utilisé.<br>
Une documentation plus détaillée sera écrite plus tard mais pour le moment il suffit d'ouvrir avec **Visual Studio**
le dossier racine et toutes les configurations devraient s'exécuter automatiquement.

La compilation de l'entièreté du projet, dépendances incluses, est totalement automatisée.
