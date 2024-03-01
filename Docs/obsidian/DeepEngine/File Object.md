**File Object** est un format de fichier spécialement conçu pour le DeepEngine.
Il a une syntax ressemblant à celle du **JSON** avec quelques spécificités.

# Fonctionnement

Un **File Object** est un conteneur pouvant contenir d'autres conteneurs et des éléments.

## Conteneur

Un **conteneur** permet de stocker d'autres conteneurs et des éléments.
Ils ont la forme suivante :
```fobj
conteneur {

}
```

Ils sont constitués d'un nom, dans l'exemple au-dessus le nom est *conteneur*, suivi d'une accolade ouvrante et se terminant par une accolade fermante.
Tout ce qui se trouve à l'intérieur des accolades fait parti de ce conteneur.

Un conteneur peut avoir d'autres conteneurs, par exemple :
```fobj
top {
    second {
    }
    third {
    }
    other {
      last {
      }
    }
}
```

## Élément

Un élément est constitué d'une clé et d'une valeur dans la forme suivante :
```fobj
clé : valeur
```

