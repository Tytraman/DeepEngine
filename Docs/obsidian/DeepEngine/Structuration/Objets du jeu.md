Tous les objets présents dans le jeu sont stockés dans une `scene_graph`, c'est une structure de données qui permet de stocker plusieurs éléments qui ont un parent et des enfants. Chaque enfant a un parent et peut avoir des enfants.


# Scene Graph
Une *Scene Graph* permet de représenter une liste d'objets de manière hiérarchique.
![[scene_graph01.png]]![[scene_graph02.png]]


# scene_node
*Interface* qui décrit ce que doit être un *node*.

Elle contient une `scene_node_properties` qui contient les données brutes du node.

Elle contient les fonctions suivantes :
- `set_transform` : modifie la matrice de transformation.
- `on_update` : appelée à chaque update de l'état du jeu, appelle l'update de tous ses enfants.
- `pre_render` : appelé juste avant `render`.
- `render` : fait le rendu du node, appelle le render de tous ses enfants.
- `post_render` : appelé après `render`.
- `is_visible` : retourne si le node est visible.
- `set_visible` : spécifie si le node doit être visible.
- `get_properties` : retourne les propriétés du node.
- `add_child` : ajoute un enfant au node.
- `remove_child` : retire un enfant du node.


# scene_node_properties
*Classe* qui contient les propriétés d'un *node*.

Elle possède :
- L'identifiant unique du node.
- Le nom du node.
- La matrice de transformation.
- Le *material* associé.


# Nodes
Il existe un node pour chaque élément du jeu : *Mesh*, *texte*, *caméra*...

