# Számítógépi grafika féléves és gyakorlati feladatok

**Név:** Lengyel Gábor

**Neptun-kód:** GKIU70

## A repository szerkezete

A **gyakorlati feladatok** megoldásai a **gyakorlatok/** jegyzékben találhatók. Minden gyakorlat, és azon belül minden feladat külön jegyzékbe került.

A **féléves beadandó feladat** megoldása a **feladat/** jegyzékben található.

A make parancs kiadásával egyszerűen fordíthatók a programok.

## Féléves feladat specifikáció

A feladat célja egy háromdimenziós, platformer, ügyességi játék prototípusának implementálása. A játék alapvetően egy lávatóban játszódik, amiben kisebb-nagyobb szikladarabok lebegnek. Ezeken kell átugrálnia a játékosnak.

A játék belsőnézetű, a játékos karaktere maga a kamera. A játékos tud előre és hátra, valamint oldalra mozogni a WASD billentyűkkel. A C billentyűvel guggolni, a bal Shift-tel futni, a Space-szel pedig ugrani lehet. Az ugrás implementálásához szükséges a gravitáció megvalósítása is.

Egy statikus fényforrás mellett elérhető egy zseblámpa is, ami egy spotlight és a kamerából indul. A fény követi a kamera mozgását. A lámpa az F billentyűvel kapcsolgatható, fényereje pedig a Q és E billentyűkkel módosítható.

Az ESC billentyű megnyomásával megnyitható egy menü, ahol megtalálható a program rövid leírása és az irányítás ismertetése. Található még 2 gomb a menüben, a Respawn, ami visszaállítja a kezdőhelyre a kamerát, valamint egy Quit, amivel ki lehet lépni a programból.

A program tartalmaz egy HUD-ot, ami folyamatosan megjelenít információkat a képernyőn felhasználó számára, például életerő vagy a zseblámpa állapota (világít vagy sem).

Az objektumok adatait egy .csv fájlból olvassa be a program. Ez alapján tölti be a megfelelő objektumokat, adott textúrával, koordinátákkal és rotációval, valamint mérettel. 

Minden objektumhoz meghatározásra került két bounding box. Az egyikkel akkor ellenőrzi a kamera ütközését a program, ha guggol a játékos, a másikkal pedig akkor, ha nem. A bounding box az objektumok három tengelyen való minimum és maximum koordinátái segítségével meghatározott téglatestek. A kamera pontként van kezelve, ezért a bounding box mérete meg van növelve annyival, hogy megfelelően működjön a kamerával való ütközés detektálása.

Különböző hangeffektek (például a láva hangja, ugrás hangeffekt) lejátszására is képes a program az SDL_mixer könyvtár segítségével. (Ideiglenesen eltávolítva)

## A féléves feladathoz tartozó assetek

https://www.dropbox.com/scl/fi/lfevhknkekb0czv2pe3u1/assets.zip?rlkey=s7roqg8mfuwqyh0tkmfztlmyi&e=2&st=jr01wgvj&dl=0

