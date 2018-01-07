# Variables
       PROJECT  := dhcpd

       CUR_DIR  := $(shell cd)
       SUB_DIRS := ${CUR_DIR}\app_w32
export OUT_DIR  ?= ${CUR_DIR}\_output

       TARGETS  := $(patsubst ${CUR_DIR}\\%,${OUT_DIR}/%.txt,${SUB_DIRS})

# Commands
ECHO  = echo
MKDIR = mkdir
RMDIR = rmdir

# Rules
all : ${OUT_DIR} ${TARGETS}

clean : ${OUT_DIR}
	@${ECHO} RM: $(notdir $<)
	@${RMDIR} /s /q $<

${OUT_DIR} :
	@${ECHO} MK: $(notdir $@)
	@${MKDIR} $@

${OUT_DIR}/%.txt : ${CUR_DIR}/%
	@${MAKE} -C $< ${MAKECMDGOALS}
	@${ECHO} DONE > $@
