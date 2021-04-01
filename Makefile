include /usr/include/n64/make/PRdefs

PROJECT = bird

NUSYSINCDIR  = /usr/include/n64/nusys
NUSTDINCDIR  = /usr/include/n64/nustd
NUSYSLIBDIR  = /usr/lib/n64/nusys

LIB = $(ROOT)/usr/lib
LPR = $(LIB)/PR
INC = $(ROOT)/usr/include

NUAUDIOLIB = -lnualstl_n -ln_mus -ln_audio_sc

OPTIMIZER       = -O1
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom
CFLAGS := $(CFLAGS) -DNDEBUG -D_FINALROM -DF3DEX_GBI_2 -G 0 -MMD -MP -I. -Iinclude -I$(NUSYSINCDIR) -I$(NUSTDINCDIR) -I$(ROOT)/usr/include/PR -Wa,-Iinclude
CXXFLAGS := $(CXXFLAGS) -DNDEBUG -D_FINALROM -DF3DEX_GBI_2 -MMD -MP -G 0 -std=c++17 -fno-builtin -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Iinclude -I$(NUSYSINCDIR) -I$(NUSTDINCDIR) -I$(ROOT)/usr/include/PR

ELF		= ./build/$(PROJECT).elf
TARGETS	= ./build/$(PROJECT).z64
MAP		= ./build/$(PROJECT).map
LD_SCRIPT	= $(PROJECT).ld
CP_LD_SCRIPT	= ./build/$(PROJECT)_cp.ld

HFILES  := $(wildcard include/*.h) $(wildcard models/*.h) $(wildcard models/*/*.h) $(wildcard models/objects/*/*.h)

ASMFILES   := $(wildcard src/*.s)
CODEFILES   := $(wildcard src/*.c)
CXXFILES    := $(wildcard src/*.cpp)
DATAFILES   := $(wildcard data/*.c)
SCENEFILES  := $(wildcard scene/*.c)
MODELFILES  := $(wildcard models/static/*/*.c) $(wildcard models/static/objects/*/*.c)
MODELOVLFILES := $(wildcard models/ovl/*/*.c)
RELCFILES	:= $(wildcard rel/*.c)
RELCXXFILES	:= $(wildcard rel/*.cpp)
LIBFILES    := $(wildcard lib/*.o)

OBJPATH		= 	./build/obj
DEPPATH		=	./build/dep
NRELPATH	=	./build/nrel

RELOBJECTS =	$(RELCFILES:.c=.nrel) $(RELCXXFILES:.cpp=.nrel)
RELOBJNAME =   $(notdir $(RELOBJECTS))
RELOBJPATH =   $(addprefix $(NRELPATH)/,$(RELOBJNAME))

CODEOBJECTS =	$(CODEFILES:.c=.o) $(CXXFILES:.cpp=.o) $(SCENEFILES:.c=.o) $(MODELFILES:.c=.o)
CODEOBJNAME =   $(notdir $(CODEOBJECTS))
CODEOBJPATH =   $(addprefix $(OBJPATH)/,$(CODEOBJNAME))

ASMOBJECTS =	$(ASMFILES:.s=.o)
ASMOBJNAME =   $(notdir $(ASMOBJECTS))
ASMOBJPATH =   $(addprefix $(OBJPATH)/,$(ASMOBJNAME))

DATAOBJECTS =	$(DATAFILES:.c=.o) $(MODELOVLFILES:.c=.o)
DATAOBJNAME =   $(notdir $(DATAOBJECTS))
DATAOBJPATH =   $(addprefix $(OBJPATH)/,$(DATAOBJNAME))

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= ./build/obj/boot.6102.o

DEPFILES	=	$(addprefix $(DEPPATH)/,$(CODEOBJNAME:%.o=%.d))

CODESEGMENT =	./build/codesegment.o

OBJECTS =	$(ASMOBJPATH) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJPATH) $(RELOBJPATH)

LCINCS =	-I. -I$(NUSYSINCDIR) -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0
LDIRT  =	$(APP) $(TARGETS)

LDFLAGS =	-L$(LIB) -L$(NUSYSLIBDIR) $(NUAUDIOLIB) -lnusys -lultra_rom -L$(N64_LIBGCCDIR) -lgcc  -lnustd

.SUFFIXES: .nrel

default: $(TARGETS)

clean:
	@echo "\e[31mCleaning output...\e[0m"
	@rm -rf ./build

-include $(DEPFILES)

build/nrel/%.nrel: */%.c | makeDirs
	@echo "\e[35mCompiling module $<...\e[0m"
	@$(CC) -MF $(DEPPATH)/$*.d -o $(NRELPATH)/$*.o $(CFLAGS) $<
	@./tools/makereloc $(NRELPATH)/$*.o $@

build/nrel/%.nrel: */%.cpp | makeDirs
	@echo "\e[35mCompiling module $<...\e[0m"
	@$(CC) -MF $(DEPPATH)/$*.d -o $(NRELPATH)/$*.o $(CXXFLAGS) $<
	@./tools/makereloc $(NRELPATH)/$*.o $@

build/obj/%.o: */%.s | makeDirs
	@echo "\e[35mCompiling $<...\e[0m"
	@$(CC) -MF $(DEPPATH)/$*.d -o $@ $(CFLAGS) $<

build/obj/%.o: */%.c | makeDirs
	@echo "\e[35mCompiling $<...\e[0m"
	@$(CC) -MF $(DEPPATH)/$*.d -o $@ $(CFLAGS) $<
	
build/obj/%.o: */%.cpp | makeDirs
	@echo "\e[35mCompiling $<...\e[0m"
	@$(CC) -MF $(DEPPATH)/$*.d -o $@ $(CXXFLAGS) $<

build/obj/%.o: */*/*/%.c
	@echo "\e[35mCompiling data $<...\e[0m"
	@$(CC) -MF $(DEPPATH)/$*.d -o $@ $(CFLAGS) $<

build/obj/%.o: */*/*/*/%.c
	@echo "\e[35mCompiling data $<...\e[0m"
	@$(CC) -MF $(DEPPATH)/$*.d -o $@ $(CFLAGS) $<	

$(CODESEGMENT):	$(CODEOBJPATH)
	@echo "\e[31mLinking codesegment...\e[0m"
	@$(LD) -o $(CODESEGMENT) -r $(CODEOBJPATH) $(LIBFILES) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT) | makeDirs
	@echo "\e[35mLinking $<...\e[0m"
	@$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT) | makeDirs
	@echo "\e[35mPreprocessing linkerscript $<...\e[0m"
	@cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) -o $@ $<

$(TARGETS) $(APP): $(CP_LD_SCRIPT) $(OBJECTS)
#	$(MAKEROM) spec -I$(NUSYSINCDIR) -r $(TARGETS) -m -s 10 -e $(APP)
#	@mv $(PROJECT).map codesegment.o a.out $(APP) $(TARGETS) "./build"
	@echo "\e[35mLinking ROM $<...\e[0m"
	@$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	@echo "\e[35mConverting ROM $<...\e[0m"
	@$(OBJCOPY) --pad-to=0x2000000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	@makemask $(TARGETS)
	@echo "\e[32mDone! ./build/$(PROJECT).z64\e[0m"

makeDirs:
	@echo "\e[33mCreating build directories... $<\e[0m"
	@mkdir -p "build/obj"
	@mkdir -p "build/dep"
	@mkdir -p "build/nrel"

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules