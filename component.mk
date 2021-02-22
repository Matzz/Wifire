## Application Component configuration
## Parameters configured here will override default and ENV values
## Uncomment and change examples:

## Add your source directories here separated by space
COMPONENT_SRCDIRS := \
app \
app/Actions \
app/Model \
app/Model/Networking \
app/Model/Users \
app/Services \
app/Services/Networking \
app/Utils
# COMPONENT_SRCFILES :=
# COMPONENT_INCDIRS := include

## If you require any Arduino Libraries list them here
ARDUINO_LIBRARIES := ArduinoJson6
ARDUINO_LIBRARIES += MultipartParser

## List the names of any additional Components required for this project
COMPONENT_DEPENDS := malloc_count

## Set paths for any GIT submodules your application uses
# COMPONENT_SUBMODULES :=

## Append any targets to be built as dependencies of the project, such as generation of additional binary files
# CUSTOM_TARGETS += 

## Additional object files to be included with the application library
# EXTRA_OBJ :=

## Additional libraries to be linked into the project
# EXTRA_LIBS :=

## Provide any additional compiler flags
# COMPONENT_CFLAGS :=
# COMPONENT_CXXFLAGS :=

## Configure flash parameters (for ESP12-E and other new boards):
# SPI_MODE := dio

## SPIFFS options
## Configure flash parameters (for ESP12-E and other new boards):
SPI_MODE = dio
## SPIFFS options
SPIFF_FILES = web/build
RBOOT_ENABLED ?= 1
RBOOT_BIG_FLASH ?= 1
SPI_SIZE        ?= 4M
ENABLE_SSL=0
DEBUG_VERBOSE_LEVEL=INFO

## Use standard hardware config with two ROM slots and two SPIFFS partitions
ifeq ($(SMING_ARCH),Esp8266)
HWCONFIG := basic_rboot
else
HWCONFIG := spiffs
# Emulate UART 0
ENABLE_HOST_UARTID := 0
endif

web-pack:
	$(Q) npx gulp
	$(Q) date +'%a, %d %b %Y %H:%M:%S GMT' -u > web/build/.lastModified

web-upload: web-pack spiffs-image-update