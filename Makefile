extension = cpp
math = si
threads = si

# Descomentar si se quiere ver como se invoca al compilador
verbose = si

# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ para tratamiento de errores y warnings.
CFLAGS += -Wall -Werror -pedantic -pedantic-errors

# -O3: optimiza el binario, posiblemente revelando bugs sutiles
# -O0: no optimiza el binario pero lo hace debuggeable
ifdef optimize
CFLAGS += -O3
else
CFLAGS += -O0
endif

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c17

# Estandar de C++ a usar
CXXSTD = c++17

# Estandar POSIX que extiende C/C++. En teoria los grandes
# sistemas operativos incluyendo Windows son POSIX compliant
CFLAGS += -D _POSIX_C_SOURCE=200809L

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si

# Si se quiere simular pérdidas, definir la variable wrapsocks por linea
# de comandos: 'wrapsocks=si make'  o descomentar la siguiente linea
#wrapsocks = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################


# Linkea con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkea con threads de ser necesario. Permite el uso de pthread en C y C++. Permite el uso de built-in threads en C++.
ifdef threads
LDFLAGS += -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Agrega simulación de pérdidas de bytes en las funciones de sockets
ifdef wrapsocks
CFLAGS += -Dwrapsocks=1
LDFLAGS += -Wl,--wrap=send -Wl,--wrap=recv
endif

# Se reutilizan los flags de C para C++ también
CXXFLAGS += $(CFLAGS)

# Se usa enlazador de C++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
COMPILER = $(CC)
COMPILERFLAGS = $(CFLAGS)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
COMPILER = $(CXX)
COMPILERFLAGS = $(CXXFLAGS)
endif

directorios = $(shell find . -type d -regex '.*\w+')

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif

LDFLAGS-TSAN = $(LDFLAGS) -fsanitize=thread
COMPILERFLAGS-TSAN = $(COMPILERFLAGS) -fsanitize=thread




#============= src/out For source files of includes.
ori ?=utils/
out ?=./built/
srctrg ?=scripts/

# ============= SE busca compular para un target file

# default is frep?
trg ?=fmod

# DESDE una o mas carpetas src para files ..
utils_src ?= $(wildcard $(ori)*.$(extension))


h_src ?=./
HEADERS_SRC = -I$(h_src)

# REGLAS
#########

.PHONY: all clean


o_utils = $(patsubst $(ori)%.$(extension),$(ori)%.o, $(utils_src)) $(srctrg)$(trg).o

all: prep $(o_utils)
	# ====== BUILD FINAL FOR $(out)$(trg)
	$(LD) $(patsubst %.o, $(out)%.o, $(o_utils)) -o $(trg) $(LDFLAGS)
prep:
	#==== Create directories for outputs.
	mkdir -p $(out)
	mkdir -p $(out)$(ori)
	mkdir -p $(out)$(srctrg)

%.o: %.$(extension)
	#================================= compiling general : $(out)$@ from $< 
	$(COMPILER) $(COMPILERFLAGS) $(HEADERS_SRC) -o $(out)$@ -c $<
	echo
clean:
	$(RM) -f $(patsubst %.o, $(out)%.o, $(o_utils))
	$(RM) -f $(trg)

