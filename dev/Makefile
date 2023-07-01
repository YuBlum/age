EXE=age
CC_LINUX=cc
LIBS_LINUX=-L ./libs/linux/ -lm -lglfw
FLAGS_LINUX=-I ./engine/include/ -I /usr/include/GLFW/ -I /usr/include/AL/ -I ./vendor/include/ -fPIC
CC_WINDOWS=x86_64-w64-mingw32-gcc
LIBS_WINDOWS=-L ./libs/windows/ -lm -lgdi32 -lmingwex -lwinmm -lglfw3
FLAGS_WINDOWS=-I ./engine/include/ -I /usr/include/GLFW/ -I /usr/include/AL/ -I ./vendor/include/ -I /usr/x86_64-w64-mingw32/include -m64 -mwindows -fPIC
CENGINE=$(wildcard engine/src/*.c )
CVENDOR=$(wildcard vendor/src/*.c )
OBJS_ENGINE_LINUX=$(patsubst engine/src/%.c, bin/linux/engine/%.o, $(CENGINE)) 
OBJS_ENGINE_WINDOWS=$(patsubst engine/src/%.c, bin/windows/engine/%.o, $(CENGINE))
DEPS_ENGINE_LINUX=$(patsubst engine/src/%.c, bin/linux/engine/%.d, $(CENGINE))
DEPS_ENGINE_WINDOWS=$(patsubst engine/src/%.c, bin/windows/engine/%.d, $(CENGINE))
OBJS_VENDOR_LINUX=$(patsubst vendor/src/%.c, bin/linux/vendor/%.o, $(CVENDOR)) 
OBJS_VENDOR_WINDOWS=$(patsubst vendor/src/%.c, bin/windows/vendor/%.o, $(CVENDOR))
DEPS_VENDOR_LINUX=$(patsubst vendor/src/%.c, bin/linux/vendor/%.d, $(CVENDOR))
DEPS_VENDOR_WINDOWS=$(patsubst vendor/src/%.c, bin/windows/vendor/%.d, $(CVENDOR))
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

bin/linux/$(EXE): $(OBJS_ENGINE_LINUX) $(OBJS_VENDOR_LINUX)
	@echo linking object files into elf...
	@$(CC_LINUX) $(FLAGS_LINUX) -shared -o bin/linux/lib$(EXE).so $^ $(LIBS_LINUX)
	@cp bin/linux/lib$(EXE).so ../linux/
	@cp bin/linux/lib$(EXE).so /usr/lib/
	@mkdir /usr/include/age/
	@cp	./engine/include/* /usr/include/age/
	@cp ./libs/linux/* ../linux/
	@echo $(EXE) was succesfully builded for linux!

bin/windows/$(EXE).exe: $(OBJS_ENGINE_WINDOWS) $(OBJS_VENDOR_WINDOWS)
	@echo linking object files into exe...
	@$(CC_WINDOWS) $(FLAGS_WINDOWS) -shared -o bin/windows/$(EXE).dll $^ $(LIBS_WINDOWS)
	@cp bin/windows/$(EXE).dll ../windows/
	@cp ./libs/windows/* ../windows/
	@echo $(EXE) was succesfully builded for windows!

-include $(DEPS_ENGINE_WINDOWS)
-include $(DEPS_ENGINE_LINUX)
-include $(DEPS_VENDOR_WINDOWS)
-include $(DEPS_VENDOR_LINUX)

bin/linux/engine/%.o: engine/src/%.c
	@echo compiling $< with gcc...
	@$(CC_LINUX) $(FLAGS_LINUX) -MMD -c -DLINUX $< -o $@

bin/windows/engine/%.o: engine/src/%.c
	@echo compiling $< with mingw...
	@$(CC_WINDOWS) $(FLAGS_WINDOWS) -MMD -c -DWINDOWS $< -o $@

bin/linux/vendor/%.o: vendor/src/%.c
	@echo compiling $< with gcc...
	@$(CC_LINUX) $(FLAGS_LINUX) -MMD -c -DLINUX $< -o $@

bin/windows/vendor/%.o: vendor/src/%.c
	@echo compiling $< with mingw...
	@$(CC_WINDOWS) $(FLAGS_WINDOWS) -MMD -c -DWINDOWS $< -o $@

run:
	../linux/$(EXE)

clean:
	@rm ./bin/linux/engine/*
	@rm ./bin/linux/vendor/*
	@rm ./bin/linux/lib$(EXE).so
	@rm ./bin/windows/engine/*
	@rm ./bin/windows/vendor/*
	@rm ./bin/windows/$(EXE).dll
	@echo cleaned up binaries!
	@rm ../windows/* ../linux/*
	@echo cleaned up build!

