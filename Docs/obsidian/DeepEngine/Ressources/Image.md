Les images héritent de la classe `resource`.

Elles peuvent avoir un des modèles de couleurs suivant :
- **RGB**
- **BGR**
- **RGBA**
- **ARGB**
- **BGRA**
Il est possible de passer d'un modèle de couleur à l'autre mais il peut y avoir de la perte, par exemple, passer du modèle **RGBA** à **RGB** fera perdre le canal **Alpha** qui gère la transparence.
Passer de **RGB** à **RGBA** mettra toutes les valeurs **Alpha** à `255` par défaut.

Cette classe permet une uniformisation de tous les formats d'images supportés comme le **BMP** ou le **PNG**.

# Structuration des données

Les pixels sont ordonnées dans un tableau continu en partant du pixel **haut gauche** pour finir sur le pixel **bas droite**.
La classe `image` stock en interne la largeur et la hauteur de l'image ainsi que son modèle de couleur.

