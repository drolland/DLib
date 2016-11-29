#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/d_error.o \
	${OBJECTDIR}/d_img.o \
	${OBJECTDIR}/d_list.o \
	${OBJECTDIR}/d_logger.o \
	${OBJECTDIR}/d_memory.o \
	${OBJECTDIR}/d_ml.o \
	${OBJECTDIR}/d_socket.o \
	${OBJECTDIR}/d_string.o \
	${OBJECTDIR}/d_time.o \
	${OBJECTDIR}/d_tools.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dlib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dlib: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dlib ${OBJECTFILES} ${LDLIBSOPTIONS} -lm

${OBJECTDIR}/d_error.o: d_error.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_error.o d_error.c

${OBJECTDIR}/d_img.o: d_img.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_img.o d_img.c

${OBJECTDIR}/d_list.o: d_list.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_list.o d_list.c

${OBJECTDIR}/d_logger.o: d_logger.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_logger.o d_logger.c

${OBJECTDIR}/d_memory.o: d_memory.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_memory.o d_memory.c

${OBJECTDIR}/d_ml.o: d_ml.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_ml.o d_ml.c

${OBJECTDIR}/d_socket.o: d_socket.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_socket.o d_socket.c

${OBJECTDIR}/d_string.o: d_string.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_string.o d_string.c

${OBJECTDIR}/d_time.o: d_time.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_time.o d_time.c

${OBJECTDIR}/d_tools.o: d_tools.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/d_tools.o d_tools.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D_POSIX_C_SOURCE\ =\ 199309L -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dlib

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
