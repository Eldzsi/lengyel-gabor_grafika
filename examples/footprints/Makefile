all:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/path.c src/scene.c src/utils.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lm -o footprints.exe -Wall -Wextra -Wpedantic

linux:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/path.c src/scene.c src/utils.c -lSDL2 -lSDL2_image -lGL -lm -o footprints -Wall -Wextra -Wpedantic
