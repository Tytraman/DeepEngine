# Description
	- *File Object* est un format de fichiers inventé spécialement pour le moteur.
	- Inspiré par le côté "objet" du **JSON**.
	- Permet de stocker à peu près n'importe quelle donnée (sauvegardes, données utilisateur etc...)
	- L'extension de fichier est généralement `.fobj`.
- # Principe
	- Le format repose sur 3 concepts :
		- Les [éléments]((65579cc6-3934-4e8a-841d-6f37091c278c)).
		  logseq.order-list-type:: number
		- Les [conteneurs]((65579bc3-3757-4c79-bca2-8dde473125b1)).
		  logseq.order-list-type:: number
		- Les [listes]((65579ce7-21e0-4c3e-abbd-52d72873c406)).
		  logseq.order-list-type:: number
	- Chaque conteneur et chaque élément sont référençable par une [clé]((6559cc9e-7904-4529-a4e6-17c495c4090b)).
- # Éléments
  id:: 65579cc6-3934-4e8a-841d-6f37091c278c
	- Les *éléments* sont des valeurs identifiables grâce à un nom unique, appelé la clé, ce qui fait
	  `clé : valeur`.
	- Ils permettent de stocker différents types de données sous forme de chaînes de caractères.
	- ## Exemple
		- ```fobj
		  clé : valeur
		  username : tytraman
		  score : 151
		  ```
- # Conteneurs
  id:: 65579bc3-3757-4c79-bca2-8dde473125b1
	- Les *conteneurs* sont plus ou moins l'équivalence des objets dans le format **JSON**.
	- Ils permettent de regrouper plusieurs [éléments]((65579cc6-3934-4e8a-841d-6f37091c278c)) au sein du même bloc.
	- Utile pour organiser les différentes données.
	- Une clé qui a été utilisée dans un conteneur parent peut être réutilisé dans un conteneur enfant.
	- Les conteneurs sont identifiés par un nom unique.
	- Le caractère `{` est utilisé pour ouvrir un conteneur.
	- Le caractère `}` est utilisé pour fermer le conteneur.
	- Plusieurs conteneurs peuvent être imbriqués entre eux.
	- Le caractère `.` est utilisé pour séparé les identifiants de conteneur lors d'une recherche.
	- ## Exemple
		- ```fobj
		  nom_du_conteneur {
		    clé1 : valeur1
		    clé2 : valeur2
		  }
		  
		  conteneur1 {
		    conteneur2 {
		      clé1 : 100
		      conteneur3 {
		        clé1 : 200
		      }
		    }
		  }
		  ```
			- La valeur de `clé1` valant `200` peut être récupérée grâce à la combinaison des identifiants : `conteneur1.conteneur2.conteneur3.clé1`.
- # Listes
  id:: 65579ce7-21e0-4c3e-abbd-52d72873c406
	- ## Exemple
		- ```fobj
		  conteneur {
		  	nom_liste :
		      	- valeur1
		          - valeur2
		          - valeur3
		  }
		  ```
- # Clé
  id:: 6559cc9e-7904-4529-a4e6-17c495c4090b
	- ## Règles
		- Une clé ne peut posséder que des **lettres**, des **chiffres** et / ou des **underscores**.
		- Tout autre caractère est considéré comme une erreur.