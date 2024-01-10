
# Technique
- [ ] Se renseigner sur tout ce qui est thread-safe.

# Gestion des ressources
- [ ] Utiliser une classe **RID** pour identifier chaque ressource.

# Structuration du code
- [ ] **Séparer** chaque concept et chaque notion dans des classes biens spécifiques.
	- Arrêter de vouloir tout mettre dans une seule et unique classe.
- [ ] Faire une classe `image` dans laquelle se trouve un tableau de pixels et les informations concernant le format.

# Mémoire
- [ ] Gestion des données par `queue`.
- [x] Gestion des données par `stack`. ✅ 2024-01-01

## OpenGL
- [x] Stocker les emplacements d'uniformes pour éviter de les chercher à chaque utilisation. ✅ 2024-01-01

## Triage
- [ ] Pouvoir trier des listes de types primitifs.
- [ ] Pouvoir trier des listes de `string`.

# Visuel
- [ ] Ajouter les animations d'images.
- [x] Ajouter le post-processing. ✅ 2024-01-01
	- [x] Changer la taille du framebuffer quand la résolution change. ✅ 2024-01-01
- [x] Faire en sorte de pouvoir mettre plusieurs images sur un même objet. ✅ 2024-01-01
- [ ] Ajouter des effets de lumières.
- [ ] Ajouter des effets d'ombres.
- [ ] Implémenter le `outlining`.
- [ ] Ajouter le fait de pouvoir recharger un `shader` dynamiquement.
	- Si la compilation du shader échoue, garder l'ancien `shader` actif.
## Rendu
- [ ] Ajouter le rendu de textes.
- [ ] Ajouter le rendu de segments allant d'un point **A** à un point **B**.

## Images
- [ ] Fusionner plusieurs images en une seule.
- [ ] Mixer plusieurs images.
- [ ] Tourner une image.
- [ ] Pouvoir retourner verticalement et horizontalement une image.

# Modélisation
- [ ] Faire un système permettant de gérer une collection de triangles.
- [ ] Ajouter le support de formats de modèles 3D.

# Gestion d'erreurs
- [ ] Implémenter le même système de **logs** qu'à *FB Technology*.
	- Afficher l'**horodatage** du log.
	- Afficher le nom du `thread`.
	- Afficher le **type** de log.
	- Afficher le **message** du log.
- [ ] Implémenter la même gestion de crash qu'à *FB Technology*.
	- En ayant un `launcher` qui lance le processus du moteur, qui s'échangent ensuite des **messages** les `pipes` pour s'assurer que le moteur n'a pas planté.

# Stockage
- [x] Créer un format pour stocker des données **hiérarchiques**. ✅ 2024-01-01
- [ ] Créer un format pour stocker les ressources.
- [ ] Utiliser `file_object` pour stocker des mondes.
	- [ ] Se renseigner sur les différentes manières de sauvegarder / charger un monde.
- [x] Sauvegarder une `texture` en tant qu'`image`. ✅ 2024-01-01
- [x] Pouvoir faire des captures d'écran. ✅ 2024-01-01

# API
## OpenGL
- [ ] Définir des **uniformes** prédéfinis par le moteur pour *GLSL*.
	- Par exemple `deTime`, `dePos`, `deColor`...

# Modding
- [ ] Gestion des **mods** en **Lua**.

# Debug
- [ ] Afficher la liste des sons.
- [ ] Pouvoir gérer les entités présentes dans une scène.
	- [x] Afficher les entités présentes dans une scène. ✅ 2024-01-01
	- [x] Pouvoir supprimer des entités. ✅ 2024-01-01
	- [x] Pouvoir ajouter des entités. ✅ 2024-01-10
	- [ ] Afficher les informations concernant l'entité sélectionnée.
	- [ ] **Outliner** l'entité sélectionnée.

