######  GENERAL  #####
CC 		  = gcc

#	0 = debug, 1 = release
PROFILE	  = 0

SRC_DIR   = src
BUILD_DIR = build
RES_DIR   = res
OUTPUT    = app

GLAD_INCLUDE = -I"libs/glad/includes"

INCLUDES  = -I"/usr/include" $(GLAD_INCLUDE)
LIBS      = -lglfw -lGL -ldl

ifeq ($(PROFILE), 0)
ARGS      = -std=c99 -g -O0 -D _DEBUG $(LIBS) $(INCLUDES)
else
ARGS	  = -std=c99 -O3 -D _RELEASE $(LIBS) $(INCLUDES)
endif

_: all

#####  MODULES  #####
#	UTILIS
utilis: file.o functions.o math.o mem.o option.o panic.o vector.o;
UTILIS_SRC_DIR   = $(SRC_DIR)/utilis
UTILIS_BUILD_DIR = $(BUILD_DIR)/utilis
UTILIS_OBJ       = $(UTILIS_BUILD_DIR)/file.o $(UTILIS_BUILD_DIR)/functions.o $(UTILIS_BUILD_DIR)/math.o $(UTILIS_BUILD_DIR)/mem.o $(UTILIS_BUILD_DIR)/option.o $(UTILIS_BUILD_DIR)/panic.o $(UTILIS_BUILD_DIR)/vector.o

#	OS
os:		window.o;
OS_SRC_DIR       = $(SRC_DIR)/os
OS_BUILD_DIR     = $(BUILD_DIR)/os
OS_OBJ			 = $(OS_BUILD_DIR)/window.o

#	GFX
gfx:	shader.o;
GFX_SRC_DIR		 = $(SRC_DIR)/gfx
GFX_BUILD_DIR    = $(BUILD_DIR)/gfx
GFX_OBJ			 = $(GFX_BUILD_DIR)/base/shader.o

#	GLAD
glad:	glad.o;
GLAD_SRC_DIR	 = libs/glad/src
GLAD_BUILD_DIR   = $(BUILD_DIR)/libs
GLAD_OBJ		 = $(GLAD_BUILD_DIR)/glad.o

#	MAIN
main: 	main.o;
MAIN_SRC_DIR 	 = $(SRC_DIR)
MAIN_BUILD_DIR 	 = $(BUILD_DIR)
MAIN_OBJ         = $(MAIN_BUILD_DIR)/main.o

#####  SINGLE FILES
main.o:
	$(CC) -c $(MAIN_SRC_DIR)/main.c 		-o $(MAIN_BUILD_DIR)/main.o  		$(ARGS)
file.o:
	$(CC) -c $(UTILIS_SRC_DIR)/file.c 		-o $(UTILIS_BUILD_DIR)/file.o 		$(ARGS)
functions.o:
	$(CC) -c $(UTILIS_SRC_DIR)/functions.c 	-o $(UTILIS_BUILD_DIR)/functions.o 	$(ARGS)
math.o:
	$(CC) -c $(UTILIS_SRC_DIR)/math.c 		-o $(UTILIS_BUILD_DIR)/math.o 		$(ARGS)
mem.o:
	$(CC) -c $(UTILIS_SRC_DIR)/mem.c 		-o $(UTILIS_BUILD_DIR)/mem.o 		$(ARGS)
option.o:
	$(CC) -c $(UTILIS_SRC_DIR)/option.c 	-o $(UTILIS_BUILD_DIR)/option.o 	$(ARGS)
panic.o:
	$(CC) -c $(UTILIS_SRC_DIR)/panic.c 		-o $(UTILIS_BUILD_DIR)/panic.o 		$(ARGS)
vector.o:
	$(CC) -c $(UTILIS_SRC_DIR)/vector.c 	-o $(UTILIS_BUILD_DIR)/vector.o 	$(ARGS)
window.o:
	$(CC) -c $(OS_SRC_DIR)/window.c 		-o $(OS_BUILD_DIR)/window.o			$(ARGS)
shader.o:
	$(CC) -c $(GFX_SRC_DIR)/base/shader.c	-o $(GFX_BUILD_DIR)/base/shader.o	$(ARGS)
glad.o:
	$(CC) -c $(GLAD_SRC_DIR)/glad.c			-o $(GLAD_BUILD_DIR)/glad.o			$(ARGS)

#####  TASKS  #####
OBJS = $(GLAD_OBJ) $(UTILIS_OBJ) $(OS_OBJ) $(GFX_OBJ) $(MAIN_OBJ)

all: clean prepare build;

clean:
	find build -name "*.o" -type f -delete
	rm -f build/app

prepare:
	rm -rf $(BUILD_DIR)/$(RES_DIR)
	cp -r $(RES_DIR) $(BUILD_DIR)/$(RES_DIR)

build: glad utilis os gfx main
	$(CC) $(OBJS) -o $(BUILD_DIR)/$(OUTPUT) $(ARGS)

run: all
	./$(BUILD_DIR)/$(OUTPUT)
