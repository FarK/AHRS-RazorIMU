TARGET = IMU
PORT = /dev/ttyUSB0
BAUD_P = 57600
BAUD_T = 9600
PROGRAMMER = arduino
MCU = atmega328p
F_CPU = 8000000L

CXX_SRCS = ADXL345.cpp \
	   AHRS.cpp \
	   HMC5843.cpp \
	   ITG3200.cpp \
	   USART.cpp

CXX_OBJ = $(CXX_SRCS:.cpp=.o)

CXX_HDRS = ADXL345.h \
	   AHRS.h \
	   declarations.h \
	   HMC5843.h \
	   ITG3200.h \
	   USART.h

CORE_DIR = libarduinocore

CORE_CXX_SRCS = $(CORE_DIR)/HardwareSerial.cpp \
		$(CORE_DIR)/Print.cpp \
		$(CORE_DIR)/Tone.cpp \
		$(CORE_DIR)/WMath.cpp \
		$(CORE_DIR)/WString.cpp

CORE_CXX_OBJ = $(CORE_CXX_SRCS:.cpp=.o)

CORE_CC_SRCS = $(CORE_DIR)/pins_arduino.c \
	       $(CORE_DIR)/WInterrupts.c \
	       $(CORE_DIR)/wiring_analog.c \
	       $(CORE_DIR)/wiring.c \
	       $(CORE_DIR)/wiring_digital.c \
	       $(CORE_DIR)/wiring_pulse.c \
	       $(CORE_DIR)/wiring_shift.c

CORE_CC_OBJ = $(CORE_CC_SRCS:.c=.o)

CORE_HDRS = $(CORE_DIR)/binary.h \
	    $(CORE_DIR)/HardwareSerial.h \
	    $(CORE_DIR)/pins_arduino.h \
	    $(CORE_DIR)/Print.h \
	    $(CORE_DIR)/Stream.h \
	    $(CORE_DIR)/WCharacter.h \
	    $(CORE_DIR)/WConstants.h \
	    $(CORE_DIR)/wiring.h \
	    $(CORE_DIR)/wiring_private.h \
	    $(CORE_DIR)/WProgram.h \
	    $(CORE_DIR)/WString.h

ARD_LIB_DIR = libraries

ARD_LIB_CXX_SRCS = $(ARD_LIB_DIR)/Wire/Wire.cpp
ARD_LIB_CC_SRCS = $(ARD_LIB_DIR)/Wire/utility/twi.c

ARD_LIB_CXX_OBJ = $(ARD_LIB_CXX_SRCS:.cpp=.o)
ARD_LIB_CC_OBJ = $(ARD_LIB_CC_SRCS:.c=.o)

CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AR  = avr-ar
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude

ARD_LIB_INC = -I$(ARD_LIB_DIR) -I$(ARD_LIB_DIR)/Wire -I$(ARD_LIB_DIR)/Wire/utility

#FLAGS_WARN = -Wall -Wstrict-prototypes
#FLAGS_TUNING = -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
FLAGS_OPT = -Os

ALL_INC = -I. $(ARD_LIB_INC)
OBJS = $(CXX_OBJ) $(ARD_LIB_CC_OBJ) $(ARD_LIB_CXX_OBJ) 
ALL_OBJS := $(addprefix build/, $(notdir $(OBJS)))
ALL_CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(ALL_INC) $(FLAGS_WARN) $(FLAGS_TUNNIG) $(FLAGS_OPT)
ALL_CXXFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(ALL_INC) $(FLAGS_TUNNIG) $(FLAGS_OPT) #-Wall
#ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)
END_FLAGS = -lc -lm -lc

all : $(TARGET).hex

$(TARGET).hex : $(TARGET).out
	avr-objcopy -O ihex -R .eeprom $(TARGET).out $(TARGET).hex

$(TARGET).out : $(OBJS)
	$(CXX) $(ALL_CXXFLAGS) main.cpp $(ALL_OBJS) -o $(TARGET).out $(END_FLAGS)

upload : $(TARGET).hex
	avrdude -c$(PROGRAMMER) -p$(MCU) -b$(BAUD_P) -P$(PORT) -U flash:w:$(TARGET).hex

serialmon :
	picocom -b$(BAUD_T) $(PORT)

.SUFFIXES: .elf .hex .eep .lss .sym .cpp .o .c .s .S

# Define all listing files.
#LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $(addprefix build/, $(notdir $@)) $(END_FLAGS)

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $(addprefix build/, $(notdir $@)) $(END_FLAGS)


# Compile: create assembler files from C source files.
#.c.s:
#	$(CC) -S $(ALL_CFLAGS) $< -o build/$@ -lm


# Assemble: create object files from assembler source files.
#.S.o:
#	$(CC) -c $(ALL_ASFLAGS) $< -o build/$@
