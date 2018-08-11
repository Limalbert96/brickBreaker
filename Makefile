################################################################################
# These are variables for the GBA toolchain build
# You can add others if you wish to
# ***** Albert Lim *****
################################################################################

# The name of your desired GBA game
# This should be a just a name i.e MyFirstGBAGame
# No SPACES AFTER THE NAME.
PROGNAME = brickBreaker

# Here you must put a list of all of the object files
# that will be compiled into your program. For example
# if you have main.c and myLib.c then in the following
# line you would put
# OFILES = main.o myLib.o
# This is a space separated list of files with the .o extension.
OFILES = main.o mylib.o text.o font.o pic1.o win.o lose.o

# The header files you have created.
# This is necessary to determine when to recompile for files.
# This should be a space separated list of .h files
HFILES = DMA.h mylib.h pic1.h text.h

################################################################################
# These are various settings used to make the GBA toolchain work
# DO NOT EDIT BELOW.
################################################################################

.PHONY: all
all: CFLAGS += $(CRELEASE)
all: LDFLAGS += $(LDRELEASE)
all: $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

include /opt/cs2110-tools/GBAVariables.mak

debug : CFLAGS += $(CDEBUG)
debug : LDFLAGS += $(LDDEBUG)
debug : $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

$(PROGNAME).gba : $(PROGNAME).elf
	@echo "[LINK] Linking objects together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf : crt0.o $(GCCLIB)/crtbegin.o $(GCCLIB)/crtend.o $(GCCLIB)/crti.o $(GCCLIB)/crtn.o $(OFILES) libc_sbrk.o
	@$(CC) $(CFLAGS) -o $(PROGNAME).elf $^ $(LDFLAGS)

%.o : %.c
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.s                                                                        
	@echo "[ASSEMBLE] Assembling $<"                                               
	@$(AS) $(MODEL) $< -o $@

.PHONY : vba
vba : CFLAGS += $(CRELEASE)
vba : LDFLAGS += $(LDRELEASE)
vba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator VBA-M"
	@vbam $(VBAOPT) $(PROGNAME).gba

.PHONY : mgba
mgba : CFLAGS += $(CRELEASE)
mgba : LDFLAGS += $(LDRELEASE)
mgba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator VBA-M"
	@mgba $(PROGNAME).gba

.PHONY : med
med : CFLAGS += $(CRELEASE)
med : LDFLAGS += $(LDRELEASE)
med : $(PROGNAME).gba
	@echo "[EXECUTE] Running emulator Mednafen"
	@mednafen $(PROGNAME).gba

.PHONY : clean
clean :
	@echo "[CLEAN] Removing all compiled files"
	@rm -f *.o *.elf *.gba
