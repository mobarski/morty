# Conditionals

Language without "else" is also acceptable!

```
distance 10 or-less [ collision-warning ] if

distance 10 or-less [ collision-warning ] then

distance 10 or-less then collision-warning end-then

distance 10 or-less then [ collision-warning ]

distance 10 or-less then [ collision-warning ] do
```

```
age 18 or-more [ show-content ] [ show-restriction ] if-else

age 18 or-more if-else [ show-content ] [ show-restriction ] 

age 18 or-more then [ show-content ]
               else [ show-restriction ]


```

# Loops

Definite:
```
5 times [ 1 add ]

5 times [ 1 add ] loop

```

Indefinite:
```
loop [ ... ]

do [ ... ] loop

do [ ... ] repeat

for [ ... ] loop

loop [ ... 10 ge then [ break ] ... ]

```


```
```
