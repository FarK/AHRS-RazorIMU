#Configuración dependiente del micro
TARGET = IMU
PORT = /dev/ttyUSB0
BAUD_P = 57600
BAUD_T = 9600
PROGRAMMER = arduino
MCU = atmega328p
F_CPU = 8000000L

#Programas y compiladores
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
SERIAL_MON = serialMon

#Lista de los archivos de código
CC_HDRS = twi.h
CC_SRCS = $(CC_HDRS:.h=.c)
CC_OBJ = $(CC_HDRS:.h=.o)

CXX_HDRS = Wire.h \
	   gyroscope.h \
	   accelerometer.h \
	   magnetometer.h \
	   USART.h \
	   quaternion.h \
	   vector.h \
	   algorithm.h \
	   mathUtils.h \
	   timer8b.h

CXX_SRCS = $(CXX_HDRS:.h=.cpp)
CXX_OBJ = $(CXX_HDRS:.h=.o)

AUX_HDRS = declarations.h

#Flags de compilación
FLAGS_WARN = -Wall
#FLAGS_TUNING = -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
FLAGS_OPT = -Os

#Unimos los objetos y las banderas
ALL_OBJ = $(addprefix build/, $(notdir $(CC_OBJ))) $(addprefix build/, $(notdir $(CXX_OBJ)))
CC_FLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(FLAGS_WARN) $(FLAGS_TUNNIG) $(FLAGS_OPT)
CXX_FLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(FLAGS_WARN) $(FLAGS_TUNNIG) $(FLAGS_OPT)
END_FLAGS = -lm -lc -lm

#Buscamos los objetos en build
VPATH = build
.PHONY: upload serialmon clean clear

all : build $(TARGET).hex

$(TARGET).hex : $(TARGET).out
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET).out $(TARGET).hex

$(TARGET).out : $(CC_OBJ) $(CXX_OBJ) $(CC_HDRS) $(CXX_HDRS) $(AUX_HDRS) main.cpp
	$(CXX) $(CXX_FLAGS) main.cpp $(ALL_OBJ) -o $(TARGET).out $(END_FLAGS)

build:
	mkdir build

upload : $(TARGET).hex
	$(AVRDUDE) -c$(PROGRAMMER) -p$(MCU) -b$(BAUD_P) -P$(PORT) -U flash:w:$(TARGET).hex

serialmon :
	$(SERIAL_MON) -b$(BAUD_T) $(PORT)

clean clear:
	@rm -v build/*.o $(TARGET).hex $(TARGET).out 2> /dev/null

.SUFFIXES: .c .cpp .o

# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(CXX_FLAGS) $< -o $(addprefix build/, $(notdir $@))

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(CC_FLAGS) $< -o $(addprefix build/, $(notdir $@))
