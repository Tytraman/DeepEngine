

# Problèmes possibles du parallélisme

## Race condition
Ou *situation de compétition* en français.

### Description
C'est une situation où le résultat **diffère** selon l'ordre d'exécution.
Dans le cas du **parallélisme**, cela arrive quand **plusieurs** threads manipulent la **même** donnée. Nous ne pouvons pas prédire le résultat des opérations car elles dépendent de l'ordre des threads qui les exécutent.

<iframe 
		border=0
		frameborder=0
		height=560
		width=1280
		src="https://fr.wikipedia.org/wiki/Situation_de_comp%C3%A9tition">
		</iframe>

### Ressources externes

<iframe width="1120" height="630" src="https://www.youtube.com/embed/MqnpIwN7dz0?si=9UoX9js1f10ltYaz" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>

# Thread-safety

Il existe plusieurs moyens qui cumulés permettent de rendre l'exécution du code *thread-safe*, c'est-à-dire que le **résultat** d'une procédure n'a pas d'effet imprévu si on l'exécute en suivant un paradigme de **parallélisme**.
## Atomicité

<iframe 
		border=0
		frameborder=0
		height=560
		width=1280
		src="https://fr.wikipedia.org/wiki/Atomicit%C3%A9_%28informatique%29">
		</iframe>
### Explication du problème
Lorsque **plusieurs** threads manipulent la **même** variable, il y a 
