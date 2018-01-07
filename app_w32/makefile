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
       CPPFLAGS += -DUNICODE -D_UNICODE
       CFLAGS   += -Wall
       CFLAGS   += -g
#      CFLAGS   += -mno-ms-bitfields
       LDFLAGS  += -L${OUT_DIR}
#      LDFLAGS  += -mwindows
       LDFLAGS  += -mconsole
#      LDFLAGS  += -Wl,-subsystem,windows
       LDFLAGS  += -g
#      LDFLAGS  += -s
       LD_LIBS  += -lcomctl32
#      LD_LIBS  += -lcomdlg32 -lgdi32

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
