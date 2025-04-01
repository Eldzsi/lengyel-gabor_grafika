# Számítógépi grafika beadandó és gyakorlati feladatok

**Név:** Lengyel Gábor

**Neptun-kód:** GKIU70

## A repository szerkezete

A **gyakorlati feladatok** megoldásai a **gyakorlatok/** jegyzékben találhatók. Minden gyakorlat, és azon belül minden feladat külön jegyzékbe került.

A **féléves beadandó feladat** megoldása a **feladat/** jegyzékben található.

A make parancs kiadásával egyszerűen fordíthatók a programok.

## Beadandó feladat specifikáció

A feladat célja egy háromdimenziós, platformer, ügyességi játék prototípusának implementálása. A játék lényege, hogy a játékos eljusson egy A pontból egy B pontba, miközben különböző akadályokon jut át. (például: szakadékok átugrása, tüskék kikerülése, stb.)

A program elindításakor a főmenü fogadja a felhasználót, amelyen minimális design elemeken kívül található egy Play gomb és egy Kilépés gomb. A Play gombra kattintva elindul a játék. Játék közben vissza lehet térni a főmenübe az ESC megnyomásával és ki lehet lépni vagy az elejéről kezdeni a játékot.

Az objektumok adatai (modell, textúra, pozíció, stb.) egy fájlban kerülnek tárolásra, ahonnan a program be tudja tölteni őket.

Az F1 billentyű megnyomásával előhozható egy Segítség panel, ahol megtalálható a program felhasználói dokumentációja.

A játék belsőnézetű, a játékos karaktere maga a kamera. A játékos tud előre és hátra, valamint oldalra mozogni a WASD billentyűkkel. A C billentyűvel guggolni, a Space megnyomásával pedig ugrani lehet.

Az ugrás implementálásához szükséges a gravitáció megvalósítása is, ami folyamatosan "húzza" lefelé a játékost.

Elérhető egy zseblámpa is, ami egy spotlight és a kamerából indul. A fény követi a kamera mozgását. A lámpa az F billentyűvel kapcsolgatható, fényereje pedig a görgővel módosítható.

## A beadandó feladathoz tartozó assetek

https://www.dropbox.com/scl/fi/kvtkr0ie3kwcmu695079p/assets.zip?rlkey=v7qwgr0l61d0xjnvlxieiv1d4&st=xxsfd8rw&dl=0

## A beadandó feladathoz szükséges könyvtárak

https://www.dropbox.com/scl/fi/wtv9yfhq1la2ahw2csomj/dependencies.zip?rlkey=uevt3f8l14tofletpmeukxlg3&st=js81i4vm&dl=0
