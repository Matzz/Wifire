## Application Component configuration
## Parameters configured here will override default and ENV values
## Uncomment and change examples:

## Add your source directories here separated by space
COMPONENT_SRCDIRS := \
app \
app/Actions \
app/Configuration \
app/Configuration/Networking \
app/Configuration/Users \
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
SPIFF_FILES = files
RBOOT_ENABLED ?= 1
RBOOT_BIG_FLASH ?= 1
SPI_SIZE        ?= 4M
SPIFF_SIZE      ?= 524288
DISABLE_SPIFFS = 0
ENABLE_SSL=1