
APP_NAME := PasswordWand
# ARDUINO_LIBS := AUnit SSD1306Ascii SPI EEPROM_SPI_WE RotaryEncoder SimpleCLI Crypto EEPROM Keyboard HID
ARDUINO_LIBS := AUnit
DEPS := $(wildcard src/*.cpp)
EXTRA_CXXFLAGS += -g
EXTRA_CFLAGS += -g
EXTRA_CPPFLAGS += -g

APP_SRCS_CPP := main.t.cpp $(wildcard src/*.t.cpp)

include ../libraries/EpoxyDuino/EpoxyDuino.mk

.PHONY: clean_app
clean_app:
	rm $(wildcard *.t.o) $(wildcard src/*.t.o)

t: clean_app $(APP_NAME).out run
