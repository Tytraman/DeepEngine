Cette page regroupe toutes les difficultés rencontrées pendant le développement du moteur.

# Mémoire

## Objet qui dépend d'un objet qui est détruit automatiquement

En C++, lorsque l'on sort d'un scope, le destructeur des objets est appelé.
Un problème majeur de cette fonctionnalité est lors de l'utilisation de variables globales, qui sont donc détruites à l'arrêt du programme.

Si le destructeur d'un objet dépend d'un objet qui a été détruit avant, le résultat est indéfini.

### Objet statique

Le problème que je viens de citer juste au-dessus se produit surtout lorsque des objets statiques sont utilisés.