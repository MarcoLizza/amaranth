ifeq ($(PLATFORM),windows)
	ifeq ($(VARIANT),x64)
		TARGET=tofu_x64.exe
	else
		TARGET=tofu_x32.exe
	endif
else ifeq ($(PLATFORM),raspberry)
	TARGET=tofu-rpi_x32
else
	TARGET=tofu
endif

# Use software renderer to use VALGRIND
#   export LIBGL_ALWAYS_SOFTWARE=1
#   valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./tofu ./demos/mode7/

ANALYZER=luacheck
AFLAGS=--no-self --std lua53 -q

# In case we want to embed pre-compiled script, we need to disable the `LUA_32BITS` compile flag!
#	@luac5.3 -o - $< | $(DUMPER) $(DFLAGS) > $@
DUMPER=hexdump
DFLAGS=-v -e '1/1 "0x%02X,"'

ifeq ($(PLATFORM),windows)
	ifeq ($(VARIANT),x64)
		COMPILER=x86_64-w64-mingw32-gcc
	else
		COMPILER=i686-w64-mingw32-gcc
	endif
else
	COMPILER=gcc
endif
CWARNINGS=-std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -Wpedantic -Wstrict-prototypes -Wunreachable-code -Wlogical-op
#CWARNINGS=-std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -Wpedantic -Wstrict-prototypes -Wshadow -Wunreachable-code -Wlogical-op -Wfloat-equal
CFLAGS=-D_DEFAULT_SOURCE -DLUA_32BITS -DLUA_FLOORN2I=1 -DSTBI_ONLY_PNG -DSTBI_NO_STDIO -Isrc -Iexternal
ifeq ($(BUILD),release)
	COPTS=-O3 -DRELEASE
else
#	COPTS=-Og -g -DDEBUG
	COPTS=-O0 -g -DDEBUG
endif
# -Ofast => -O3 -ffast-math
# -Os => -O2, favouring size

ifeq ($(PLATFORM),windows)
	ifeq ($(VARIANT),x64)
		LINKER=x86_64-w64-mingw32-gcc
		LFLAGS=-Lexternal/GLFW/windows/x64 -lglfw3 -lgdi32
	else
		LINKER=i686-w64-mingw32-gcc
		LFLAGS=-Lexternal/GLFW/windows/x32 -lglfw3 -lgdi32
	endif
else ifeq ($(PLATFORM),raspberry)
	LINKER=gcc
	LFLAGS=-Lexternal/GLFW/rpi/x32 -lglfw3 -lm -lpthread -lX11 -ldl
else
	LINKER=gcc
	LFLAGS=-Lexternal/GLFW/linux/x64 -lglfw3 -lm -lpthread -lX11 -ldl
endif
LWARNINGS=-Wall -Wextra -Werror

SOURCES:= $(wildcard src/*.c src/core/*.c src/core/io/*.c src/core/io/display/*.c src/core/vm/*.c src/core/vm/modules/*.c src/core/vm/modules/resources/*.c src/libs/*.c src/libs/fs/*.c src/libs/gl/*.c external/glad/*.c external/GLFW/*.c external/lua/*.c external/miniaudio/*.c external/spleen/*.c external/stb/*.c)
INCLUDES:= $(wildcard src/*.h src/core/*.h src/core/io/*.h src/core/io/display/*.h src/core/vm/*.h src/core/vm/modules/*.h src/core/vm/modules/resources/*.h src/libs/*.h src/libs/fs/*.h src/libs/gl/*.h external/glad/*.h external/GLFW/*.h external/lua/*.h external/miniaudio/*.h external/spleen/*.h external/stb/*.h)
OBJECTS:= $(SOURCES:%.c=%.o)
SCRIPTS:= $(wildcard src/core/vm/*.lua src/core/vm/modules/*.lua)
BLOBS:= $(SCRIPTS:%.lua=%.inc)
RM=rm -f

default: $(TARGET)
all: default

$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LWARNINGS) $(LFLAGS) -o $@
	@echo "Linking complete!"

# The dependency upon `Makefile` is redundant, since scripts are bound to it.
$(OBJECTS): %.o : %.c $(BLOBS) $(INCLUDES) Makefile
	@$(COMPILER) $(CWARNINGS) $(CFLAGS) $(COPTS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

# Define a rule to automatically convert `.lua` script into an embeddable-ready `.inc` file.
# `.inc` files also depend upon `Makefile` to be rebuild in case of tweakings.
$(BLOBS): %.inc: %.lua Makefile
	@$(ANALYZER) $(AFLAGS) $<
	@$(DUMPER) $(DFLAGS) $< > $@
	@echo "Generated "$@" from "$<" successfully!"

primitives: $(TARGET)
	@echo "Launching *primitives* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/primitives
	./$(TARGET) ./demos/primitives

bunnymark: $(TARGET)
	@echo "Launching *bunnymark* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/bunnymark
	@./$(TARGET) ./demos/bunnymark

fire: $(TARGET)
	@echo "Launching *fire* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/fire
	@./$(TARGET) ./demos/fire

tiled-map: $(TARGET)
	@echo "Launching *tiled-map* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/tiled-map
	@./$(TARGET) ./demos/tiled-map

timers: $(TARGET)
	@echo "Launching *timers* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/timers
	@./$(TARGET) ./demos/timers

postfx: $(TARGET)
	@echo "Launching *postfx* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/postfx
	@./$(TARGET) ./demos/postfx

spritestack: $(TARGET)
	@echo "Launching *spritestack* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/spritestack
	@./$(TARGET) ./demos/spritestack

palette: $(TARGET)
	@echo "Launching *palette* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/palette
	@./$(TARGET) ./demos/palette

mode7: $(TARGET)
	@echo "Launching *mode7* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/mode7
	@./$(TARGET) ./demos/mode7

snake: $(TARGET)
	@echo "Launching *snake* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/snake
	@./$(TARGET) ./demos/snake

shades: $(TARGET)
	@echo "Launching *shades* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/shades
	@./$(TARGET) ./demos/shades

gamepad: $(TARGET)
	@echo "Launching *gamepad* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/gamepad
	@./$(TARGET) ./demos/gamepad

gamepad-pak: $(TARGET)
	@echo "Launching *gamepad (PAK)* application!"
	@$(ANALYZER) $(AFLAGS) ./demos/gamepad
	@lua ./extras/pakgen.lua ./demos/gamepad ./demos/gamepad.pak
	@./$(TARGET) ./demos/gamepad.pak

valgrind: $(TARGET)
	@echo "Valgrind *$(DEMO)* application!"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes env LIBGL_ALWAYS_SOFTWARE=1 ./$(TARGET) ./demos/$(DEMO)

.PHONY: clean
clean:
	@$(RM) $(OBJECTS)
	@$(RM) $(BLOBS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(RM) $(TARGET)
	@echo "Executable removed!"