# DeepEngine

Moteur de jeux en cours de développement.

## Structuration

Le moteur est constitué de 2 projets :
- **DeepEngine** : *la bibliothèque de code.*
- **CreationUI** : *le logiciel permettant la création de jeux en utilisant le DeepEngine.*

Même si l'utilisation du **DeepEngine** à lui seul permet de créer des jeux, il est recommandé d'utiliser le **CreationUI** car il fourni des outils spécialements conçus pour s'intégrer facilement avec le moteur.

## Compilation

Afin de simplifier le processus de compilation, CMake est utilisé.<br>
Une documentation plus détaillée sera écrite plus tard mais pour le moment il suffit d'ouvrir avec **Visual Studio**
le dossier où se situe *CMakeLists.txt* et toutes les configurations devraient s'exécuter automatiquement.

Le **DeepEngine** et le **CreationUI** pourront être compilés en 1 seul clic.
