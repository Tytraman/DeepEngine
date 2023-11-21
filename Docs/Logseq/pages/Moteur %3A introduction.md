# Description
	- Cette page regroupe toutes les choses fondamentales à savoir pour développer un moteur 3D simple.
	- Pour aller plus loin et faire des choses plus complexes, lire la page [[Moteur : notions avancées]].
- # Références
	- SDL : https://www.libsdl.org/
	- OpenGL : https://fr.wikipedia.org/wiki/OpenGL
	- Learn OpenGL : https://learnopengl.com/
- # Initialisation
	- Le moteur doit en premier lieu initialiser et vérifier plusieurs choses :
		- Vérifier s'il y a une autre **instance** du programme en cours d'exécution.
		  logseq.order-list-type:: number
		- Vérifier si le disque dur en cours d'utilisation a suffisamment de **place** pour **stocker** les données du jeu.
		  logseq.order-list-type:: number
		- Vérifier si l'ordinateur possède suffisamment de **mémoire** pour supporter le jeu.
		  logseq.order-list-type:: number
		- Initialiser la **SDL**.
		  logseq.order-list-type:: number
	- Certains paramètres du moteur seront ensuite chargés selon le mode de **build** entre [[Mode debug]] et [[Mode release]].
- # La fenêtre
	- Avant de pouvoir afficher des pixels à l'écran, il faut d'abord créer une *fenêtre* sur laquelle plus tard nous dessineront des pixels.
	- Le mot *fenêtre* englobe plusieurs choses :
		- Le **conteneur visuel** qui contient les pixels.
		- Le système qui gère les **entrées utilisateurs** (clavier, souris, manette etc...).
		- Les **évènements** liés à la *fenêtre* (déplacement, redimmension, fermeture etc...).
	- C'est grâce au système de *fenêtre* que nous pouvons récupérer les appuis des touches, le déplacement de la souris, le redimmensionnement de la *fenêtre* et toutes autres choses en lien avec une *fenêtre*.
	- Plusieurs moyens existent pour créer une fenêtre, il est totalement possible de le faire en utilisant l'**API Windows** mais dans le cas du [[🕹️DeepEngine]], la [SDL](https://www.libsdl.org/) a été utilisé.
- # L'API graphique
	- Il existe **2 façons** d'afficher des pixels sur une fenêtre :
		- Rendu logiciel : **CPU**
		- Accélération matérielle : **GPU**
	- Quand on parle d'**API graphique**, on fait référence au système qui va nous permettre d'afficher des choses à l'écran. La **SDL** possède tout un tas de fonctions qui permettent d'afficher par exemple des lignes ou des rectangles, c'est ce qu'on appelle une **API graphique**.
	- Dans le cas du [[🕹️DeepEngine]], le rendu se fait grâce à l'accélération matérielle et utilise [OpenGL](https://fr.wikipedia.org/wiki/OpenGL) comme **API graphique**.
		- **OpenGL** étant une **spécification** et non une bibliothèque directement utilisable, il est nécessaire de trouver un moyen pour appeler les fonctions, le [[🕹️DeepEngine]] utilise [glad](https://glad.dav1d.de/).
- # Afficher des choses
	- ## Post-processing
	-