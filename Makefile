# Project: ProjGraf

CPP  = g++
CC   = gcc
BIN  = ProjGraf

OBJ  = main.o plane.o controller.o mesh.o terrain.o HUD.o meteorspawner.o perlinnoise.o skybox.o texture.o finishline.o collectable.o
LINKOBJ  = main.o plane.o controller.o mesh.o terrain.o HUD.o meteorspawner.o perlinnoise.o skybox.o texture.o finishline.o collectable.o

# Library linking
OS := $(shell uname)
ifeq ($(OS),Darwin)
## caso Mac OS
$(info Mac OS detected)
FRMPATH=-F /Library/Frameworks
LIBS =  -framework OpenGL -framework SDL2 -framework SDL2_ttf -framework SDL2_image -lm
$(info SDL2 libraries must be in: $(FRMPATH))
else
ifeq ($(OS),MINGW32_NT-6.1)
## caso Windows MinGW
$(info Windows MinGW detected)
FRMPATH = -IC:\MinGW\include
LIBS = -LC:\MinGW\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lopengl32 -lglu32 -lm
else
##caso Linux
$(info Linux detected)
#framework presenti solo nel caso MAC OS
FRMPATH =
LIBS = -lGL -lGLU -lSDL2 -lSDL2_ttf -lSDL2_image -lm
endif
endif

FLAG = -Wno-deprecated
RM = rm -f

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(FRMPATH) $(LIBS)

main.o: main.cpp
	$(CPP) -c $(FRMPATH) main.cpp -o main.o

plane.o: plane.cpp
	$(CPP) -c $(FRMPATH) plane.cpp -o plane.o

controller.o: controller.cpp
	$(CPP) -c $(FRMPATH) controller.cpp -o controller.o

mesh.o: mesh.cpp
	$(CPP) -c $(FRMPATH) mesh.cpp -o mesh.o

terrain.o: terrain.cpp
	$(CPP) -c $(FRMPATH) terrain.cpp -o terrain.o

HUD.o: HUD.cpp
	$(CPP) -c $(FRMPATH) HUD.cpp -o HUD.o

meteorspawner.o: meteorspawner.cpp
	$(CPP) -c $(FRMPATH) meteorspawner.cpp -o meteorspawner.o

perlinnoise.o: perlinnoise.cpp
	$(CPP) -c $(FRMPATH) perlinnoise.cpp -o perlinnoise.o

skybox.o: skybox.cpp
	$(CPP) -c $(FRMPATH) skybox.cpp -o skybox.o

texture.o: texture.cpp
	$(CPP) -c $(FRMPATH) texture.cpp -o texture.o

finishline.o: finishline.cpp
	$(CPP) -c $(FRMPATH) finishline.cpp -o finishline.o

collectable.o: collectable.cpp
	$(CPP) -c $(FRMPATH) collectable.cpp -o collectable.o