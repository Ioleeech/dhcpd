# Variables
       PROJECT  := dhcpd

       CUR_DIR  := $(shell cd)
       INC_DIR  := ${CUR_DIR}
       SRC_DIR  := ${CUR_DIR}
export OUT_DIR  ?= ${CUR_DIR}\_output

       HEADERS  := $(wildcard ${INC_DIR}/*.h)
       SOURCES  := $(wildcard ${SRC_DIR}/*.c)
       OBJECTS  := $(patsubst ${SRC_DIR}/%.c,${OUT_DIR}/${PROJECT}_%.o,${SOURCES})
       BINARY   := ${OUT_DIR}/${PROJECT}.exe

       CPPFLAGS += $(patsubst %,-I%,${INC_DIR})
       CFLAGS   += -Wall
#      CFLAGS   += -mno-ms-bitfields
       LDFLAGS  += -L${OUT_DIR}
       LD_LIBS  += -lcomctl32

# Dependences from config
ifeq ($(CFG_UNICODE),y)
       CPPFLAGS += -DUNICODE -D_UNICODE
endif

ifeq ($(CFG_DEBUG),y)
       CPPFLAGS += -DDEBUG -D_DEBUG
       CFLAGS   += -g
       LDFLAGS  += -g
       LDFLAGS  += -mconsole
else
       LDFLAGS  += -s
       LDFLAGS  += -mwindows
#      LDFLAGS  += -Wl,-subsystem,windows
endif

# Commands
CC    = gcc
LD    = ld
ECHO  = echo
MKDIR = mkdir
RMDIR = rmdir

# Rules
all : ${OUT_DIR} ${BINARY}

clean : ${OUT_DIR}
	@${ECHO} RM: $(notdir $<)
	@${RMDIR} /s /q $<

${OUT_DIR} :
	@${ECHO} MK: $(notdir $@)
	@${MKDIR} $@

${BINARY} : ${OBJECTS}
	@${ECHO} LD: $(notdir $@)
	@${CC} -o $@ $^ ${LDFLAGS} ${LD_LIBS}

${OUT_DIR}/${PROJECT}_%.o : ${SRC_DIR}/%.c
	@${ECHO} CC: $(notdir $<)
	@${CC} -c -o $@ $< ${CPPFLAGS} ${CFLAGS}
