# DeepEngine

Moteur de jeux en cours de d�veloppement.

## Structuration

Le moteur est constitu� de 2 projets :
- **DeepEngineCore** : la biblioth�que de code.
- **DeepEngine** : le logiciel permettant la cr�ation de jeux en utilisant le DeepEngineCore.

M�me si l'utilisation du **DeepEngineCore** � lui seul permet de cr�er des jeux, il est recommand� d'utiliser le **DeepEngine** car il fourni des outils sp�cialements con�us pour s'int�grer facilement avec le moteur.

## D�pendances

**DeepEngineCore** d�pend de plusieurs librairies dont voici la liste :
- `SDL` : pour afficher / g�rer une fen�tre et g�rer les entr�es utilisateur.
- `zlib` : pour la compression /  d�compression de donn�es.
- `libpng` : pour le support des images PNG.
- `imgui` : pour afficher des interfaces interactives.
- `OpenAL` : pour le support de l'audio / audio spatial.
- `OpenGL` : pour le rendu graphique.

**DeepEngine** ne d�pend que de **DeepEngineCore**.

## Compilation

Afin de simplifier le processus de compilation, CMake est utilis�.<br>
Une documentation plus d�taill�e sera �crite plus tard mais pour le moment il suffit d'ouvrir avec **Visual Studio**
le dossier racine et toutes les configurations devraient s'ex�cuter automatiquement.

La compilation de l'enti�ret� du projet, d�pendances incluses, est totalement automatis�e.
