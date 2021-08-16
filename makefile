######  GENERAL  #####
CC 		  = gcc
CPPCHECK  = cppcheck
#	valgrind note: still reachable data should not be not bad...
VALGRIND  = valdgrind

#	0 = debug, 1 = release
PROFILE	  = 0

SRC_DIR   = src
BUILD_DIR = build
RES_DIR   = res
OUTPUT    = app

GLAD_INCLUDE = -I"libs/glad/includes"
STB_INCLUDE  = -I"libs/stb/includes"
FLOAT16_INCLUDE = -I"libs/float16/includes"
HMM_INCLUDE	 = -I"libs/handmade_math/includes"

INCLUDES  = -I"/usr/include" $(GLAD_INCLUDE) $(STB_INCLUDE) $(FLOAT16_INCLUDE) $(HMM_INCLUDE)
LIBS      = -lglfw -lGL -ldl -lm

ifeq ($(PROFILE), 0)
ARGS      = -std=c99 -g -O0 -D _DEBUG $(LIBS) $(INCLUDES)
else
ARGS	  = -std=c99 -O3 -D _RELEASE $(LIBS) $(INCLUDES)
endif

_: all

#####  MODULES  #####
#	UTILIS
utilis: file.o functions.o math.o mem.o option.o panic.o vector.o tostring.o;
UTILIS_SRC_DIR   = $(SRC_DIR)/utilis
UTILIS_BUILD_DIR = $(BUILD_DIR)/utilis
UTILIS_OBJ       = $(UTILIS_BUILD_DIR)/file.o $(UTILIS_BUILD_DIR)/functions.o $(UTILIS_BUILD_DIR)/math.o $(UTILIS_BUILD_DIR)/mem.o $(UTILIS_BUILD_DIR)/option.o $(UTILIS_BUILD_DIR)/panic.o $(UTILIS_BUILD_DIR)/vector.o $(UTILIS_BUILD_DIR)/tostring.o

#	OS
os:		window.o;
OS_SRC_DIR       = $(SRC_DIR)/os
OS_BUILD_DIR     = $(BUILD_DIR)/os
OS_OBJ			 = $(OS_BUILD_DIR)/window.o

#	GFX
gfx:	base.o gl_error.o shader.o program.o buffer.o layout.o texture.o sprogram.o;
GFX_SRC_DIR		 = $(SRC_DIR)/gfx
GFX_BUILD_DIR    = $(BUILD_DIR)/gfx
GFX_OBJ			 = $(GFX_BUILD_DIR)/base/base.o $(GFX_BUILD_DIR)/base/gl_error.o $(GFX_BUILD_DIR)/base/shader.o $(GFX_BUILD_DIR)/base/program.o $(GFX_BUILD_DIR)/base/buffer.o $(GFX_BUILD_DIR)/base/layout.o $(GFX_BUILD_DIR)/base/texture.o $(GFX_BUILD_DIR)/base/sprogram.o

#	GLAD
glad:	glad.o;
GLAD_SRC_DIR	 = libs/glad/src
GLAD_BUILD_DIR   = $(BUILD_DIR)/libs
GLAD_OBJ		 = $(GLAD_BUILD_DIR)/glad.o

#	STB
stb:	stb_impl.o;
STB_SRC_DIR		 = libs/stb/src
STB_BUILD_DIR	 = $(BUILD_DIR)/libs
STB_OBJ			 = $(STB_BUILD_DIR)/stb_impl.o

#	FLOAT16
float16: float16.o;
FLOAT16_SRC_DIR	 = libs/float16/src
FLOAT16_BUILD_DIR	= $(BUILD_DIR)/libs
FLOAT16_OBJ		 = $(FLOAT16_BUILD_DIR)/float16.o

#	HANDMADEMATH (HMM)
hmm: hmm.o;
HMM_SRC_DIR		 = libs/handmade_math/src
HMM_BUILD_DIR	 = $(BUILD_DIR)/libs
HMM_OBJ		 	 = $(HMM_BUILD_DIR)/hmm.o

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
tostring.o:
	$(CC) -c $(UTILIS_SRC_DIR)/tostring.c 	-o $(UTILIS_BUILD_DIR)/tostring.o 	$(ARGS)
window.o:
	$(CC) -c $(OS_SRC_DIR)/window.c 		-o $(OS_BUILD_DIR)/window.o			$(ARGS)
base.o:
	$(CC) -c $(GFX_SRC_DIR)/base/base.c		-o $(GFX_BUILD_DIR)/base/base.o		$(ARGS)
gl_error.o:
	$(CC) -c $(GFX_SRC_DIR)/base/gl_error.c	-o $(GFX_BUILD_DIR)/base/gl_error.o	$(ARGS)
shader.o:
	$(CC) -c $(GFX_SRC_DIR)/base/shader.c	-o $(GFX_BUILD_DIR)/base/shader.o	$(ARGS)
program.o:
	$(CC) -c $(GFX_SRC_DIR)/base/program.c  -o $(GFX_BUILD_DIR)/base/program.o  $(ARGS)
buffer.o:
	$(CC) -c $(GFX_SRC_DIR)/base/buffer.c   -o $(GFX_BUILD_DIR)/base/buffer.o   $(ARGS)
layout.o:
	$(CC) -c $(GFX_SRC_DIR)/base/layout.c   -o $(GFX_BUILD_DIR)/base/layout.o   $(ARGS)
texture.o:
	$(CC) -c $(GFX_SRC_DIR)/base/texture.c  -o $(GFX_BUILD_DIR)/base/texture.o  $(ARGS)
sprogram.o:
	$(CC) -c $(GFX_SRC_DIR)/base/sprogram.c -o $(GFX_BUILD_DIR)/base/sprogram.o $(ARGS)
glad.o:
	$(CC) -c $(GLAD_SRC_DIR)/glad.c			-o $(GLAD_BUILD_DIR)/glad.o			$(ARGS)
stb_impl.o:
	$(CC) -c $(STB_SRC_DIR)/stb_impl.c		-o $(STB_BUILD_DIR)/stb_impl.o		$(ARGS)
float16.o:
	$(CC) -c $(FLOAT16_SRC_DIR)/float16.c	-o $(FLOAT16_BUILD_DIR)/float16.o	$(ARGS)
hmm.o:
	$(CC) -c $(HMM_SRC_DIR)/handmade_math.c	-o $(HMM_BUILD_DIR)/hmm.o			$(ARGS)

#####  TASKS  #####
OBJS = $(HMM_OBJ) $(FLOAT16_OBJ) $(STB_OBJ) $(GLAD_OBJ) $(UTILIS_OBJ) $(OS_OBJ) $(GFX_OBJ) $(MAIN_OBJ)

all: clean build_prepare build;

clean:
	find build -name "*.o" -type f -delete
	rm -f build/app

build_prepare:
	rm -rf $(BUILD_DIR)/$(RES_DIR)
	cp -r $(RES_DIR) $(BUILD_DIR)/$(RES_DIR)

build: hmm float16 stb glad utilis os gfx main
	$(CC) $(OBJS) -o $(BUILD_DIR)/$(OUTPUT) $(ARGS)

run: all
	./$(BUILD_DIR)/$(OUTPUT)

analyze: all
	(valgrind --leak-check=full --show-leak-kinds=all -s ./$(BUILD_DIR)/$(OUTPUT)) &> ./result.txt
	cat ./result.txt

static_analyze:
ifeq ($(PROFILE), 0)
	$(CPPCHECK) -D_DEBUG -v --bug-hunting --output-file=result.txt -q  .
else
	$(CPPCHECK) -D_RELEASE -v --bug-hunting --output-file=result.txt -q .
endif
	cat ./result.txt
