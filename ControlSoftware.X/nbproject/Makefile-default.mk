#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ControlSoftware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ControlSoftware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Register.cpp SPIDevice.cpp SPIBus.cpp Altimeter.cpp LCD.cpp IMU.cpp PWMC.cpp Gyroscope.cpp Accelerometer.cpp Magnetometer.cpp Quaternion.cpp usb/src/usb.c usb/src/usb_cdc.c usb/src/usb_winusb.c usb_callbacks.c Object.cpp Main.cpp System.cpp usb_descriptors.c _Time.cpp Timer.cpp

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Register.o ${OBJECTDIR}/SPIDevice.o ${OBJECTDIR}/SPIBus.o ${OBJECTDIR}/Altimeter.o ${OBJECTDIR}/LCD.o ${OBJECTDIR}/IMU.o ${OBJECTDIR}/PWMC.o ${OBJECTDIR}/Gyroscope.o ${OBJECTDIR}/Accelerometer.o ${OBJECTDIR}/Magnetometer.o ${OBJECTDIR}/Quaternion.o ${OBJECTDIR}/usb/src/usb.o ${OBJECTDIR}/usb/src/usb_cdc.o ${OBJECTDIR}/usb/src/usb_winusb.o ${OBJECTDIR}/usb_callbacks.o ${OBJECTDIR}/Object.o ${OBJECTDIR}/Main.o ${OBJECTDIR}/System.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/_Time.o ${OBJECTDIR}/Timer.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Register.o.d ${OBJECTDIR}/SPIDevice.o.d ${OBJECTDIR}/SPIBus.o.d ${OBJECTDIR}/Altimeter.o.d ${OBJECTDIR}/LCD.o.d ${OBJECTDIR}/IMU.o.d ${OBJECTDIR}/PWMC.o.d ${OBJECTDIR}/Gyroscope.o.d ${OBJECTDIR}/Accelerometer.o.d ${OBJECTDIR}/Magnetometer.o.d ${OBJECTDIR}/Quaternion.o.d ${OBJECTDIR}/usb/src/usb.o.d ${OBJECTDIR}/usb/src/usb_cdc.o.d ${OBJECTDIR}/usb/src/usb_winusb.o.d ${OBJECTDIR}/usb_callbacks.o.d ${OBJECTDIR}/Object.o.d ${OBJECTDIR}/Main.o.d ${OBJECTDIR}/System.o.d ${OBJECTDIR}/usb_descriptors.o.d ${OBJECTDIR}/_Time.o.d ${OBJECTDIR}/Timer.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Register.o ${OBJECTDIR}/SPIDevice.o ${OBJECTDIR}/SPIBus.o ${OBJECTDIR}/Altimeter.o ${OBJECTDIR}/LCD.o ${OBJECTDIR}/IMU.o ${OBJECTDIR}/PWMC.o ${OBJECTDIR}/Gyroscope.o ${OBJECTDIR}/Accelerometer.o ${OBJECTDIR}/Magnetometer.o ${OBJECTDIR}/Quaternion.o ${OBJECTDIR}/usb/src/usb.o ${OBJECTDIR}/usb/src/usb_cdc.o ${OBJECTDIR}/usb/src/usb_winusb.o ${OBJECTDIR}/usb_callbacks.o ${OBJECTDIR}/Object.o ${OBJECTDIR}/Main.o ${OBJECTDIR}/System.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/_Time.o ${OBJECTDIR}/Timer.o

# Source Files
SOURCEFILES=Register.cpp SPIDevice.cpp SPIBus.cpp Altimeter.cpp LCD.cpp IMU.cpp PWMC.cpp Gyroscope.cpp Accelerometer.cpp Magnetometer.cpp Quaternion.cpp usb/src/usb.c usb/src/usb_cdc.c usb/src/usb_winusb.c usb_callbacks.c Object.cpp Main.cpp System.cpp usb_descriptors.c _Time.cpp Timer.cpp


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/ControlSoftware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX270F256D
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/usb/src/usb.o: usb/src/usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb/src/usb.o.d" -o ${OBJECTDIR}/usb/src/usb.o usb/src/usb.c   
	
${OBJECTDIR}/usb/src/usb_cdc.o: usb/src/usb_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_cdc.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb/src/usb_cdc.o.d" -o ${OBJECTDIR}/usb/src/usb_cdc.o usb/src/usb_cdc.c   
	
${OBJECTDIR}/usb/src/usb_winusb.o: usb/src/usb_winusb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_winusb.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_winusb.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_winusb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb/src/usb_winusb.o.d" -o ${OBJECTDIR}/usb/src/usb_winusb.o usb/src/usb_winusb.c   
	
${OBJECTDIR}/usb_callbacks.o: usb_callbacks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_callbacks.o.d 
	@${RM} ${OBJECTDIR}/usb_callbacks.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_callbacks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb_callbacks.o.d" -o ${OBJECTDIR}/usb_callbacks.o usb_callbacks.c   
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c   
	
else
${OBJECTDIR}/usb/src/usb.o: usb/src/usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb/src/usb.o.d" -o ${OBJECTDIR}/usb/src/usb.o usb/src/usb.c   
	
${OBJECTDIR}/usb/src/usb_cdc.o: usb/src/usb_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_cdc.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb/src/usb_cdc.o.d" -o ${OBJECTDIR}/usb/src/usb_cdc.o usb/src/usb_cdc.c   
	
${OBJECTDIR}/usb/src/usb_winusb.o: usb/src/usb_winusb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_winusb.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_winusb.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_winusb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb/src/usb_winusb.o.d" -o ${OBJECTDIR}/usb/src/usb_winusb.o usb/src/usb_winusb.c   
	
${OBJECTDIR}/usb_callbacks.o: usb_callbacks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_callbacks.o.d 
	@${RM} ${OBJECTDIR}/usb_callbacks.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_callbacks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb_callbacks.o.d" -o ${OBJECTDIR}/usb_callbacks.o usb_callbacks.c   
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Register.o: Register.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Register.o.d 
	@${RM} ${OBJECTDIR}/Register.o 
	@${FIXDEPS} "${OBJECTDIR}/Register.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Register.o.d" -o ${OBJECTDIR}/Register.o Register.cpp  
	
${OBJECTDIR}/SPIDevice.o: SPIDevice.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SPIDevice.o.d 
	@${RM} ${OBJECTDIR}/SPIDevice.o 
	@${FIXDEPS} "${OBJECTDIR}/SPIDevice.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/SPIDevice.o.d" -o ${OBJECTDIR}/SPIDevice.o SPIDevice.cpp  
	
${OBJECTDIR}/SPIBus.o: SPIBus.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SPIBus.o.d 
	@${RM} ${OBJECTDIR}/SPIBus.o 
	@${FIXDEPS} "${OBJECTDIR}/SPIBus.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/SPIBus.o.d" -o ${OBJECTDIR}/SPIBus.o SPIBus.cpp  
	
${OBJECTDIR}/Altimeter.o: Altimeter.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Altimeter.o.d 
	@${RM} ${OBJECTDIR}/Altimeter.o 
	@${FIXDEPS} "${OBJECTDIR}/Altimeter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Altimeter.o.d" -o ${OBJECTDIR}/Altimeter.o Altimeter.cpp  
	
${OBJECTDIR}/LCD.o: LCD.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LCD.o.d 
	@${RM} ${OBJECTDIR}/LCD.o 
	@${FIXDEPS} "${OBJECTDIR}/LCD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/LCD.o.d" -o ${OBJECTDIR}/LCD.o LCD.cpp  
	
${OBJECTDIR}/IMU.o: IMU.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IMU.o.d 
	@${RM} ${OBJECTDIR}/IMU.o 
	@${FIXDEPS} "${OBJECTDIR}/IMU.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/IMU.o.d" -o ${OBJECTDIR}/IMU.o IMU.cpp  
	
${OBJECTDIR}/PWMC.o: PWMC.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PWMC.o.d 
	@${RM} ${OBJECTDIR}/PWMC.o 
	@${FIXDEPS} "${OBJECTDIR}/PWMC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/PWMC.o.d" -o ${OBJECTDIR}/PWMC.o PWMC.cpp  
	
${OBJECTDIR}/Gyroscope.o: Gyroscope.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Gyroscope.o.d 
	@${RM} ${OBJECTDIR}/Gyroscope.o 
	@${FIXDEPS} "${OBJECTDIR}/Gyroscope.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Gyroscope.o.d" -o ${OBJECTDIR}/Gyroscope.o Gyroscope.cpp  
	
${OBJECTDIR}/Accelerometer.o: Accelerometer.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Accelerometer.o.d 
	@${RM} ${OBJECTDIR}/Accelerometer.o 
	@${FIXDEPS} "${OBJECTDIR}/Accelerometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Accelerometer.o.d" -o ${OBJECTDIR}/Accelerometer.o Accelerometer.cpp  
	
${OBJECTDIR}/Magnetometer.o: Magnetometer.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Magnetometer.o.d 
	@${RM} ${OBJECTDIR}/Magnetometer.o 
	@${FIXDEPS} "${OBJECTDIR}/Magnetometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Magnetometer.o.d" -o ${OBJECTDIR}/Magnetometer.o Magnetometer.cpp  
	
${OBJECTDIR}/Quaternion.o: Quaternion.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Quaternion.o.d 
	@${RM} ${OBJECTDIR}/Quaternion.o 
	@${FIXDEPS} "${OBJECTDIR}/Quaternion.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Quaternion.o.d" -o ${OBJECTDIR}/Quaternion.o Quaternion.cpp  
	
${OBJECTDIR}/Object.o: Object.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Object.o.d 
	@${RM} ${OBJECTDIR}/Object.o 
	@${FIXDEPS} "${OBJECTDIR}/Object.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Object.o.d" -o ${OBJECTDIR}/Object.o Object.cpp  
	
${OBJECTDIR}/Main.o: Main.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Main.o.d 
	@${RM} ${OBJECTDIR}/Main.o 
	@${FIXDEPS} "${OBJECTDIR}/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Main.o.d" -o ${OBJECTDIR}/Main.o Main.cpp  
	
${OBJECTDIR}/System.o: System.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/System.o.d 
	@${RM} ${OBJECTDIR}/System.o 
	@${FIXDEPS} "${OBJECTDIR}/System.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/System.o.d" -o ${OBJECTDIR}/System.o System.cpp  
	
${OBJECTDIR}/_Time.o: _Time.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/_Time.o.d 
	@${RM} ${OBJECTDIR}/_Time.o 
	@${FIXDEPS} "${OBJECTDIR}/_Time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/_Time.o.d" -o ${OBJECTDIR}/_Time.o _Time.cpp  
	
${OBJECTDIR}/Timer.o: Timer.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Timer.o.d 
	@${RM} ${OBJECTDIR}/Timer.o 
	@${FIXDEPS} "${OBJECTDIR}/Timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Timer.o.d" -o ${OBJECTDIR}/Timer.o Timer.cpp  
	
else
${OBJECTDIR}/Register.o: Register.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Register.o.d 
	@${RM} ${OBJECTDIR}/Register.o 
	@${FIXDEPS} "${OBJECTDIR}/Register.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Register.o.d" -o ${OBJECTDIR}/Register.o Register.cpp  
	
${OBJECTDIR}/SPIDevice.o: SPIDevice.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SPIDevice.o.d 
	@${RM} ${OBJECTDIR}/SPIDevice.o 
	@${FIXDEPS} "${OBJECTDIR}/SPIDevice.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/SPIDevice.o.d" -o ${OBJECTDIR}/SPIDevice.o SPIDevice.cpp  
	
${OBJECTDIR}/SPIBus.o: SPIBus.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SPIBus.o.d 
	@${RM} ${OBJECTDIR}/SPIBus.o 
	@${FIXDEPS} "${OBJECTDIR}/SPIBus.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/SPIBus.o.d" -o ${OBJECTDIR}/SPIBus.o SPIBus.cpp  
	
${OBJECTDIR}/Altimeter.o: Altimeter.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Altimeter.o.d 
	@${RM} ${OBJECTDIR}/Altimeter.o 
	@${FIXDEPS} "${OBJECTDIR}/Altimeter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Altimeter.o.d" -o ${OBJECTDIR}/Altimeter.o Altimeter.cpp  
	
${OBJECTDIR}/LCD.o: LCD.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LCD.o.d 
	@${RM} ${OBJECTDIR}/LCD.o 
	@${FIXDEPS} "${OBJECTDIR}/LCD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/LCD.o.d" -o ${OBJECTDIR}/LCD.o LCD.cpp  
	
${OBJECTDIR}/IMU.o: IMU.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IMU.o.d 
	@${RM} ${OBJECTDIR}/IMU.o 
	@${FIXDEPS} "${OBJECTDIR}/IMU.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/IMU.o.d" -o ${OBJECTDIR}/IMU.o IMU.cpp  
	
${OBJECTDIR}/PWMC.o: PWMC.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PWMC.o.d 
	@${RM} ${OBJECTDIR}/PWMC.o 
	@${FIXDEPS} "${OBJECTDIR}/PWMC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/PWMC.o.d" -o ${OBJECTDIR}/PWMC.o PWMC.cpp  
	
${OBJECTDIR}/Gyroscope.o: Gyroscope.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Gyroscope.o.d 
	@${RM} ${OBJECTDIR}/Gyroscope.o 
	@${FIXDEPS} "${OBJECTDIR}/Gyroscope.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Gyroscope.o.d" -o ${OBJECTDIR}/Gyroscope.o Gyroscope.cpp  
	
${OBJECTDIR}/Accelerometer.o: Accelerometer.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Accelerometer.o.d 
	@${RM} ${OBJECTDIR}/Accelerometer.o 
	@${FIXDEPS} "${OBJECTDIR}/Accelerometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Accelerometer.o.d" -o ${OBJECTDIR}/Accelerometer.o Accelerometer.cpp  
	
${OBJECTDIR}/Magnetometer.o: Magnetometer.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Magnetometer.o.d 
	@${RM} ${OBJECTDIR}/Magnetometer.o 
	@${FIXDEPS} "${OBJECTDIR}/Magnetometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Magnetometer.o.d" -o ${OBJECTDIR}/Magnetometer.o Magnetometer.cpp  
	
${OBJECTDIR}/Quaternion.o: Quaternion.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Quaternion.o.d 
	@${RM} ${OBJECTDIR}/Quaternion.o 
	@${FIXDEPS} "${OBJECTDIR}/Quaternion.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Quaternion.o.d" -o ${OBJECTDIR}/Quaternion.o Quaternion.cpp  
	
${OBJECTDIR}/Object.o: Object.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Object.o.d 
	@${RM} ${OBJECTDIR}/Object.o 
	@${FIXDEPS} "${OBJECTDIR}/Object.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Object.o.d" -o ${OBJECTDIR}/Object.o Object.cpp  
	
${OBJECTDIR}/Main.o: Main.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Main.o.d 
	@${RM} ${OBJECTDIR}/Main.o 
	@${FIXDEPS} "${OBJECTDIR}/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Main.o.d" -o ${OBJECTDIR}/Main.o Main.cpp  
	
${OBJECTDIR}/System.o: System.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/System.o.d 
	@${RM} ${OBJECTDIR}/System.o 
	@${FIXDEPS} "${OBJECTDIR}/System.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/System.o.d" -o ${OBJECTDIR}/System.o System.cpp  
	
${OBJECTDIR}/_Time.o: _Time.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/_Time.o.d 
	@${RM} ${OBJECTDIR}/_Time.o 
	@${FIXDEPS} "${OBJECTDIR}/_Time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/_Time.o.d" -o ${OBJECTDIR}/_Time.o _Time.cpp  
	
${OBJECTDIR}/Timer.o: Timer.cpp  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Timer.o.d 
	@${RM} ${OBJECTDIR}/Timer.o 
	@${FIXDEPS} "${OBJECTDIR}/Timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -I"." -I"usb/include" -I"usb/src" -MMD -MF "${OBJECTDIR}/Timer.o.d" -o ${OBJECTDIR}/Timer.o Timer.cpp  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/ControlSoftware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CPPC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/ControlSoftware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}              -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--defsym=_min_heap_size=4096,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/ControlSoftware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CPPC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/ControlSoftware.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=4096,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/ControlSoftware.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
