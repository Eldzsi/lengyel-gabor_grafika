all:
	gcc -Iinclude/ src/bounding_box.c src/sound.c src/app.c src/input.c src/player.c src/main.c src/scene.c src/smoke.c src/texture.c src/utils.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lobj -lopengl32 -lm -o main.exe -Wall -Wextra -Wpedantic

linux:
	gcc -Iinclude/ src/bounding_box.c src/sound.c src/app.c src/input.c src/player.c src/main.c src/scene.c src/smoke.c src/texture.c src/utils.c -lobj -lSDL2 -lSDL2_image -lGL -lm -o main -Wall -Wextra -Wpedantic
