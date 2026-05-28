
APP_NAME := PasswordWand
# ARDUINO_LIBS := AUnit SSD1306Ascii SPI EEPROM_SPI_WE RotaryEncoder SimpleCLI Crypto EEPROM Keyboard HID
ARDUINO_LIBS := AUnit Crypto
DEPS := $(wildcard src/*.cpp)

APP_SRCS_CPP := main.t.cpp $(wildcard src/*.t.cpp)

include ../libraries/EpoxyDuino/EpoxyDuino.mk

.PHONY: clean_app
clean_app:
	rm $(wildcard *.t.o) $(wildcard src/*.t.o)

t: clean_app $(APP_NAME).out run
