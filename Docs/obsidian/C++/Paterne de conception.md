Il existe plusieurs manières de programmer en C++, chacune a ses avantages et ses inconvénients, mais aussi leur niveau de complexité à mettre en place et à maintenir.

# Dependency injection

# Sources

Le livre *Dependency Injection in .NET* par Mark Seemann.

## Explication du paterne

<iframe width="560" height="315" src="https://www.youtube.com/embed/yunF2PgJlHU?si=BvGUlcixWJyH3u_y" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>

# Intérêts du paterne

Le principal intérêt à utiliser ce paterne de conception est d'écrire du code maintenable, extensible et facilement modifiable.

D'autres intérêts très intéressants à noter sont ceux-ci :
- Permet le *late binding* : permet de changer une partie du code sans avoir besoin de tout recompiler.
- Test unitaires.
- Permet de remplacer l'implémentation d'une interface sans que cela casse le client qui l'utilise.
	- Il est possible d'ajouter des fonctionnalités qui n'étaient pas de base pensées.
- 