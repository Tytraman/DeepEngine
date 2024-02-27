Presque toutes les classes que j'ai pu analyser dans Godot héritent de `Object`. Et même d'après la documentation : https://docs.godotengine.org/en/stable/contributing/development/core_and_modules/object_class.html c'est à la base de pratiquement toutes les classes.

Cette classe permet d'ajouter la réflexion :

<iframe 
		border=0
		frameborder=0
		height=560
		width=1280
		src="https://fr.wikipedia.org/wiki/R%C3%A9flexion_(informatique)">
		</iframe>

## Propriétés
Cette classe permet de définir des propriétés sur les objets, qui sont utilisées pour :
- La sérialisation / désérialisation de l'objet.
- Créer une liste de valeurs éditables pour les classes dérivés.

## Signaux
Il est possible d'attacher des signaux aux objets, ou comme on peut les appeler, des fonctions callback ou encore des `delegate` *(en C#)*.

## Compteur de références
La classe [[Compteur de références#RefCounted]] hérite d'[[Object]] et permet de compter le nombre de références qu'il y a sur l'objet afin de libérer la mémoire automatiquement.
Plus de détails disponibles sur la page associée.

