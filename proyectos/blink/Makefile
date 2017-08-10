HAL = ../../hal

FUENTES += main.c
FUENTES += $(HAL)/mkl25-startup.c

CABECERAS += $(HAL)

SALIDA = programa
DIR_SALIDA = salida
LINKER_SCRIPT = $(HAL)/mkl25z128.ld

OBJETOS = $(addprefix $(DIR_SALIDA)/, $(patsubst %.c,%.o,$(notdir $(FUENTES))))

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CPUFLAGS += -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft
CPUFLAGS += -specs=nano.specs -specs=nosys.specs

CFLAGS += -Wall -g -ffunction-sections -fdata-sections
#CFLAGS += -O2
CFLAGS += $(addprefix -I,$(CABECERAS))

LDFLAGS += -gc-sections -T$(LINKER_SCRIPT)

CLEAN += $(SALIDA).elf
CLEAN += $(SALIDA).srec

ifeq ($(V),1)
  TRACE_CC =
  TRACE_LD =
  TRACE_OBJCOPY =
  Q=
else
  TRACE_CC = @echo "  CC       " $<
  TRACE_LD = @echo "  LD       " $@
  TRACE_OBJCOPY = @echo "  OBJCOPY  " $@
  Q=@
endif

.SUFFIXES:

vpath %.c $(sort $(dir $(FUENTES)))

all: $(SALIDA).srec

$(DIR_SALIDA):
	mkdir $(DIR_SALIDA)

$(DIR_SALIDA)/%.o: %.c | $(DIR_SALIDA)
	$(TRACE_CC)
	$(Q)$(CC) $(CPUFLAGS) $(CFLAGS) -c $< -o $@

$(SALIDA).elf: $(OBJETOS)
	$(TRACE_LD)
	$(Q)$(LD) $(CPUFLAGS) $(LDFLAGS) $^ -o $@

$(SALIDA).srec: $(SALIDA).elf
	$(TRACE_OBJCOPY)
	$(Q)$(OBJCOPY) $< -O srec $@

.PHONY: clean
clean:
	rm -rf $(DIR_SALIDA)
	rm -rf $(CLEAN)
