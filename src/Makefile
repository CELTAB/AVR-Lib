CC = avr-gcc
AR = avr-ar

MCU   = atmega328p
F_CPU = 8000000
BAUD = 19200

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -DBAUD=$(BAUD) -Os -isystem ../include
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,--relax
CFLAGS += -std=gnu99
## CFLAGS += -Wl,-u,vfprintf -lprintf_flt -lm  ## for floating-point printf
## CFLAGS += -Wl,-u,vfprintf -lprintf_min      ## for smaller printf

ARFLAGS = rcsv

SRC := $(shell ls *.c)

## List of all header files
HEADERS = $(SRC:.c=.h)

OBJ = $(SRC:.c=.o)

libavrutil.a: $(OBJ)
	mkdir -p ../lib
	$(AR) $(ARFLAGS) ../lib/$@ $^

clean:
	rm -f *.o *.a
