Les ressources sont les fichiers externes utilisés dans un jeu, comme les **images**, les **sons**, les **vidéos** ou encore les **modèles 3D**.
Les images peuvent ensuite être utilisées pour texturer un modèle 3D, ou un son peut être utilisé comme musique de fond dans un niveau.

Quoi qu'il arrive, n'importe quel jeu a besoin de ressources pour avoir une identité.

Il est possible de hard-coder les ressources dans l'exécutable du jeu, cependant cela n'est pas souvent possible, notamment dû à la taille de ces ressources qui peuvent parfois peser plusieurs gigas.
De ce fait il est nécessaire de pouvoir gérer toutes ces ressources.

# Gestion des ressources

**DeepEngine** embarque un gestionnaire de ressources accessible avec le **singleton** `resource_manager`.
Il est possible avec cette classe d'indiquer un flux *(stream)* dans lequel toutes les ressources du jeu sont stockées, actuellement ce flux de données est au format **zip**.

Il est ensuite possible de récupérer une ressource grâce à la méthode suivante :
```c++
ref<resource> get_resource(const char *name);
```

Cette fonction retourne une référence vers une ressource. La fonction **vérifie** si la ressource est déjà dans le **cache**, si c'est le cas alors elle retourne simplement la référence déjà accessible, sinon, elle charge la ressource depuis le flux dans le cache et retourne la référence, ce qui implique un temps d'exécution plus lent.

⚠️ **Attention** : une ressource ne doit pas contenir "plusieurs" ressources. Par exemple j'avais pour idée de faire une ressource *shader* qui contiendrait à la fois le *vertex shader* **et** le *fragment shader*, mais c'est une mauvaise idée.
Chaque ressource doit être séparée. C'est au moment où les ressources sont nécessaires qu'elles peuvent être fusionnées.

## Nom des ressources

Chaque ressource possède un nom unique et doit respecter la syntax du format [[File Object]].

## Entrée dans le File Object

Pour qu'une ressource soit considérée dans le projet, il faut que son entrée figure dans le *File Object* nommé `file_listing.fobj`.
L'entrée doit indiquer son **type** ainsi que sa **localisation** dans le fichier zip.

Ces entrées permettent d'avoir des métadonnées sur les fichiers en plus de ceux déjà présents par le format zip.
Il est par exemple plus simple de savoir le format d'un fichier grâce à la clé **type**.

La syntax du fichier `file_listing.fobj` ressemble à ça :
```fobj
textures {

    texture1 {
	    type : png
	    location : textures/grass.png
    }

	texture2 {
		type : bmp
		location : textures/dirt.bmp
	}

}

shaders {

	default_vert {
		type : txt
		location : shaders/default.vert
	}

	default_frag {
		type : txt
		location : shaders/default.frag
	}

}
```

**textures** et **shaders** sont 2 conteneurs dans lesquels il y a 2 entrées.
Par exemple **texture1** est une ressource de type **PNG** qui se situe à l'emplacement "textures/grass.png".
Chaque entrée doit posséder une liste de propriétés, certaines sont obligatoires et d'autres sont optionnelles :

- **type** : défini le type de fichier, par exemple PNG, TXT ou OBJ.
- **location** : emplacement du fichier dans le fichier zip.

## Cache des ressources

Dès qu'une ressource est accédée, elle est automatiquement stockée dans le cache si ce n'est pas déjà le cas.
Cela permet plusieurs choses :
- Éviter la duplication de ressources : *si on veut utiliser la même image pour différentes textures, il est inutile de recharger à chaque fois l'image*.
- Gain de mémoire : *le fait d'éviter la duplication permet intrinsèquement de gagner de la mémoire*.
- Gain de performance : *si une ressource est déjà dans le cache alors la ressource peut y être accédée directement sans avoir à la charger*.

## Chargement des ressources

Lors de l'initialisation du `resource_manager`, les loaders pour les formats supportés par défaut par **DeepEngine** sont automatiquement créés.
En interne, une `hash_table` stock pour chaque type le loader approprié, par exemple pour le type `bmp`, c'est le loader `bmp_loader` qui est utilisé.

Chaque loader doit hériter de la classe `resource_loader`.

Il est possible d'ajouter un loader customisé ou de modifier un loader déjà existant grâce à la fonction `resource_manager::add_loader`.

## Format des ressources

Chaque ressource peut avoir un format différent, le moteur permet la gestion de plusieurs formats très connus comme le `bmp`, le `png` ou encore le `wav`.
De ce fait, toutes les classes comme `image` ou `sound` hérite de la classe `resource`.

## Liste des ressources

Cette liste sera mise à jour si j'en ressens le besoin.

- ### Image

De la classe `image`, décrite dans la page [[Image]], cette ressource représente comme son nom l'indique une image. Elle est utilisée dans tout le moteur pour créer une uniformisation du format.

- ### Texte

De la classe `text`, cette ressource contient en interne un `string` *(chaîne de caractères)*.

