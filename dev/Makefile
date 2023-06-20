EXE=sandbox
CC_LINUX=cc
LIBS_LINUX=-lm
FLAGS_LINUX=-I ./engine/include/ -I ./game/include/ -I /usr/include/GLFW/ -I /usr/include/AL/
CC_WINDOWS=x86_64-w64-mingw32-gcc
LIBS_WINDOWS=-lm -lgdi32 -lmingwex -lwinmm
FLAGS_WINDOWS=-I ./engine/include/ -I ./game/include/ -I /usr/include/GLFW/ -I /usr/include/AL/ -I /usr/x86_64-w64-mingw32/include -m64 -mwindows
CENGINE=$(wildcard engine/src/*.c )
CGAME=$(wildcard game/src/*.c )
OBJS_ENGINE_LINUX=$(patsubst engine/src/%.c, bin/linux/%.o, $(CENGINE)) 
OBJS_ENGINE_WINDOWS=$(patsubst engine/src/%.c, bin/windows/%.o, $(CENGINE))
DEPS_ENGINE_LINUX=$(patsubst engine/src/%.c, bin/linux/%.d, $(CENGINE))
DEPS_ENGINE_WINDOWS=$(patsubst engine/src/%.c, bin/windows/%.d, $(CENGINE))
OBJS_GAME_LINUX=$(patsubst game/src/%.c, bin/linux/%.o, $(CGAME)) 
OBJS_GAME_WINDOWS=$(patsubst game/src/%.c, bin/windows/%.o, $(CGAME))
DEPS_GAME_LINUX=$(patsubst game/src/%.c, bin/linux/%.d, $(CGAME))
DEPS_GAME_WINDOWS=$(patsubst game/src/%.c, bin/windows/%.d, $(CGAME))
MODE=DEVELOPMENT

ifeq ($(MODE), DEVELOPMENT)
	FLAGS_LINUX += -Wall -Wextra -Werror -g
	FLAGS_WINDOWS += -Wall -Wextra -Werror -g
else ifeq ($(MODE), RELEASE)
	FLAGS_LINUX += -Ofast
	FLAGS_WINDOWS += -Ofast
else
$(error unknown mode $(RELEASE).)
endif

.PHONY: all clean res

all: bin/linux/$(EXE) bin/windows/$(EXE).exe res

bin/linux/$(EXE): $(OBJS_ENGINE_LINUX) $(OBJS_GAME_LINUX)
	@echo linking object files into elf...
	@$(CC_LINUX) $(FLAGS_LINUX) -o bin/linux/$(EXE) $^ $(LIBS_LINUX)
	@cp bin/linux/$(EXE) ../linux/
	@cp ./libs/linux/* ../linux/libs/
	@echo $(EXE) was succesfully builded for linux!

bin/windows/$(EXE).exe: $(OBJS_ENGINE_WINDOWS) $(OBJS_GAME_WINDOWS)
	@echo linking object files into exe...
	@$(CC_WINDOWS) $(FLAGS_WINDOWS) -o bin/windows/$(EXE) $^ $(LIBS_WINDOWS)
	@cp bin/windows/$(EXE).exe ../windows/
	@cp ./libs/windows/* ../windows/libs/
	@echo $(EXE) was succesfully builded for windows!

-include $(DEPS_ENGINE_WINDOWS)
-include $(DEPS_GAME_WINDOWS)
-include $(DEPS_ENGINE_LINUX)
-include $(DEPS_GAME_LINUX)

bin/linux/%.o: engine/src/%.c
	@echo compiling $< with gcc...
	@$(CC_LINUX) $(FLAGS_LINUX) -MMD -c -DLINUX $< -o $@

bin/linux/%.o: game/src/%.c
	@echo compiling $< with gcc...
	@$(CC_LINUX) $(FLAGS_LINUX) -MMD -c -DLINUX $< -o $@

bin/windows/%.o: engine/src/%.c
	@echo compiling $< with mingw...
	@$(CC_WINDOWS) $(FLAGS_WINDOWS) -MMD -c -DWINDOWS $< -o $@

bin/windows/%.o: game/src/%.c
	@echo compiling $< with mingw...
	@$(CC_WINDOWS) $(FLAGS_WINDOWS) -MMD -c -DWINDOWS $< -o $@

run:
	../linux/$(EXE)

clean:
	@rm bin/linux/*
	@rm bin/windows/*
	@echo cleaned up binaries!
	@rm ../windows/$(EXE).exe ../windows/libs/* ../linux/$(EXE) ../linux/libs/*
	@echo cleaned up build!

