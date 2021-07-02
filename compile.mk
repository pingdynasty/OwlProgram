BUILDROOT   ?= .
BUILD       ?= $(BUILDROOT)/Build

ifeq ($(CONFIG),Debug)
CPPFLAGS    ?= -g -Wall -Wcpp -Wunused-function -DDEBUG -DUSE_FULL_ASSERT
ASFLAGS      = -g
endif

ifeq ($(CONFIG),Release)
CPPFLAGS    ?= -O3 -ffast-math -DNDEBUG
endif

# Code Paths
SOURCE       = $(BUILDROOT)/Source
LIBSOURCE    = $(BUILDROOT)/LibSource
BUILDSOURCE  = $(BUILD)/Source
TESTPATCHES  = $(BUILDROOT)/TestPatches
DAISYSP      = $(BUILDROOT)/Libraries/DaisySP/Source
CMSIS        = $(BUILDROOT)/Libraries/CMSIS/Include/
DSPLIB       = $(BUILDROOT)/Libraries/CMSIS/DSP_Lib/Source
LDSCRIPT    ?= $(BUILDROOT)/Source/flash.ld

# Tool path
# TOOLROOT ?= Tools/gcc-arm-none-eabi-9-2020-q2-update/bin/

# Tools
CC=$(TOOLROOT)arm-none-eabi-gcc
LD=$(TOOLROOT)arm-none-eabi-gcc
AR=$(TOOLROOT)arm-none-eabi-gcc-ar
AS=$(TOOLROOT)arm-none-eabi-as
NM=$(TOOLROOT)arm-none-eabi-nm
CXX=$(TOOLROOT)arm-none-eabi-g++
GDB=$(TOOLROOT)arm-none-eabi-gdb
SIZE=$(TOOLROOT)arm-none-eabi-size
RANLIB=$(TOOLROOT)arm-none-eabi-ranlib
OBJCOPY=$(TOOLROOT)arm-none-eabi-objcopy
OBJDUMP=$(TOOLROOT)arm-none-eabi-objdump

# Compilation Flags
LDLIBS   = -lm
LDFLAGS  = -Wl,--gc-sections
LDFLAGS += -fpic
LDFLAGS += -fpie
LDFLAGS += -flto
CXXFLAGS = -fno-rtti
CXXFLAGS += -fno-exceptions
ifdef HEAVY
CPPFLAGS    += -D__unix__ -DHV_SIMD_NONE
endif
ifdef FAUST
ifneq (,$(findstring soundfile,$(FAUSTOPTS)))
CPPFLAGS    += -DSOUNDFILE
endif
endif
ARCH_FLAGS = -fsingle-precision-constant -mthumb
ARCH_FLAGS += -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
# ARCH_FLAGS += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16
# ARCH_FLAGS += -mcpu=cortex-m0 -mfloat-abi=soft -msoft-float
DEF_FLAGS = -DSTM32F4XX -DARM_MATH_CM4 -D__FPU_PRESENT -D__FPU_USED=1 -DDSY_CORE_DSP -DDSY_CUSTOM_DSP
# DEF_FLAGS = -DSTM32F745xx -DARM_MATH_CM7 -D__FPU_PRESENT -D__FPU_USED=1
INC_FLAGS = -I$(BUILDROOT)/Libraries -I$(DEVICE) -I$(CMSIS) -I$(PERIPH_FILE)/inc -I$(SOURCE)
INC_FLAGS += -I$(DEVICE)/Include -I$(CMSIS)
INC_FLAGS += -I$(USB_DEVICE_FILE)/Core/inc -I$(USB_DEVICE_FILE)/Class/cdc/inc
INC_FLAGS += -I$(USB_OTG_FILE)/inc
CPPFLAGS += $(ARCH_FLAGS) $(INC_FLAGS) $(DEF_FLAGS)
CFLAGS   += -std=gnu11
CXXFLAGS += -std=gnu++17
LDFLAGS  += -T$(LDSCRIPT) $(ARCH_FLAGS)
CPPFLAGS += -I$(PATCHSOURCE)
CPPFLAGS += -I$(LIBSOURCE)
CPPFLAGS += -I$(BUILDSOURCE)
CPPFLAGS += -I$(TESTPATCHES)
CPPFLAGS += -I$(DAISYSP)
CPPFLAGS += -I$(DAISYSP)/Control
CPPFLAGS += -I$(DAISYSP)/Drums
CPPFLAGS += -I$(DAISYSP)/Dynamics
CPPFLAGS += -I$(DAISYSP)/Effects
CPPFLAGS += -I$(DAISYSP)/Filters
CPPFLAGS += -I$(DAISYSP)/Noise
CPPFLAGS += -I$(DAISYSP)/PhysicalModeling
CPPFLAGS += -I$(DAISYSP)/Synthesis
CPPFLAGS += -I$(DAISYSP)/Utility
CPPFLAGS += -DARM_CORTEX
CPPFLAGS += -DEXTERNAL_SRAM
CPPFLAGS += -nostdlib -nostartfiles -fno-builtin -ffreestanding
CPPFLAGS += -mtune=cortex-m4
# CPPFLAGS += -mtune=cortex-m7
CPPFLAGS += -fpic
CPPFLAGS += -fpie
CPPFLAGS += -fdata-sections
CPPFLAGS += -ffunction-sections
CPPFLAGS += -mno-unaligned-access
CPPFLAGS += -fno-omit-frame-pointer
CPPFLAGS += -flto
# CPPFLAGS += -munaligned-access
# CPPFLAGS += -mlong-calls
# CPPFLAGS += -mpic-data-is-text-relative

# source files
PATCH_S_SRC    = $(SOURCE)/startup.s
PATCH_C_SRC    = $(wildcard $(PATCHSOURCE)/*.c)
PATCH_CPP_SRC += $(wildcard $(PATCHSOURCE)/*.cpp)
PATCH_CPP_SRC += PatchProgram.cpp
PATCH_C_SRC   += $(wildcard $(BUILDSOURCE)/*.c)
PATCH_CPP_SRC += $(wildcard $(BUILDSOURCE)/*.cpp)
ifdef MAXIMILIAN
PATCH_CPP_SRC := $(filter-out $(PATCHSOURCE)/$(MAXIMILIAN).cpp, $(PATCH_CPP_SRC))
endif
PATCH_OBJS += $(addprefix $(BUILD)/, $(notdir $(PATCH_S_SRC:.s=.o)))
PATCH_OBJS += $(addprefix $(BUILD)/, $(notdir $(PATCH_C_SRC:.c=.o)))
PATCH_OBJS += $(addprefix $(BUILD)/, $(notdir $(PATCH_CPP_SRC:.cpp=.o)))
DAISYSP_CPP_SRC = $(wildcard $(DAISYSP)/*/*.cpp)
DAISYSP_OBJS = $(addprefix $(BUILD)/, $(notdir $(DAISYSP_CPP_SRC:.cpp=.o)))

# include common make files
include $(BUILDROOT)/libs.mk
include $(BUILDROOT)/sources.mk

C_SRC   += $(SOURCE)/heap_5.c
CPP_SRC += $(SOURCE)/Patch.cpp
CPP_SRC += $(SOURCE)/PatchProcessor.cpp
CPP_SRC += $(SOURCE)/main.cpp
CPP_SRC += $(SOURCE)/operators.cpp
CPP_SRC += $(SOURCE)/message.cpp
CPP_SRC += $(SOURCE)/system_tables.cpp

OBJS = $(addprefix $(BUILD)/,$(notdir $(C_SRC:.c=.o)))
OBJS += $(addprefix $(BUILD)/,$(notdir $(CPP_SRC:.cpp=.o)))
OBJS += $(addprefix $(BUILD)/,$(notdir $(S_SRC:.S=.o)))

# Set up search path
vpath %.S $(sort $(dir $(S_SRC)))
vpath %.s $(sort $(dir $(S_SRC) $(PATCH_S_SRC)))
vpath %.c $(sort $(dir $(C_SRC) $(PATCH_C_SRC)))
vpath %.cpp $(sort $(dir $(CPP_SRC) $(PATCH_CPP_SRC) $(DAISYSP_CPP_SRC)))

.PHONY: libs as map compile size

$(BUILD)/ShortPatchProgram.o: $(SOURCE)/ShortPatchProgram.cpp $(DEPS)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -I$(BUILD) $(SOURCE)/ShortPatchProgram.cpp -o $@
	$(CXX) -MM -MT"$@" $(CPPFLAGS) $(CXXFLAGS) -I$(BUILD) $(SOURCE)/ShortPatchProgram.cpp > $(@:.o=.d)

$(BUILD)/PatchProgram.o: $(SOURCE)/PatchProgram.cpp $(DEPS)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -I$(BUILD) $(SOURCE)/PatchProgram.cpp -o $@
	$(CXX) -MM -MT"$@" $(CPPFLAGS) $(CXXFLAGS) -I$(BUILD) $(SOURCE)/PatchProgram.cpp > $(@:.o=.d)

Libraries/libowlprg.a: $(OBJS)
	$(AR) rcs $@ $^

Libraries/libdaisysp.a: $(DAISYSP_OBJS)
	@$(AR) rcs $@ $^

$(BUILD)/$(TARGET).elf: $(PATCH_OBJS) $(LDSCRIPT)
	@$(LD) $(LDFLAGS) -o $@ $(PATCH_OBJS) $(LDLIBS) Libraries/libdaisysp.a Libraries/libowlprg.a

libs: Libraries/libowlprg.a Libraries/libdaisysp.a

as: $(BUILD)/$(TARGET).elf
	@$(OBJDUMP) -S $< > $(BUILD)/$(TARGET).s

map: $(PATCH_OBJS) $(LDSCRIPT)
	@$(LD) $(LDFLAGS) -Wl,-Map=$(BUILD)/$(TARGET).map $(PATCH_OBJS) $(LDLIBS) Libraries/libdaisysp.a Libraries/libowlprg.a

compile: $(BUILD)/$(TARGET).bin

size:
	$(NM) --print-size --size-sort $(BUILD)/$(TARGET).elf | tail -n 20
	$(SIZE) $(BUILD)/$(TARGET).elf
	ls -s $(BUILD)/$(TARGET).bin

# compile and generate dependency info
$(BUILD)/%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@
	$(CC) -MM -MT"$@" $(CPPFLAGS) $(CFLAGS) $< > $(@:.o=.d)

$(BUILD)/%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
	$(CXX) -MM -MT"$@" $(CPPFLAGS) $(CXXFLAGS) $< > $(@:.o=.d)

$(BUILD)/%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
	$(CXX) -MM -MT"$@" $(CPPFLAGS) $(CXXFLAGS) $< > $(@:.o=.d)

$(BUILD)/%.o: %.s
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.S
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(BUILD)/%.s: %.c
	$(CC) -S $(CPPFLAGS) $(CFLAGS) $< -o $@

$(BUILD)/%.s: %.cpp
	@$(CXX) -S $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(BUILD)/%.s: %.cc
	@$(CXX) -S $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(BUILD)/%.bin: $(BUILD)/%.elf
	@$(OBJCOPY) -O binary $< $@

$(BUILD)/%.s: $(BUILD)/%.elf
	@$(OBJDUMP) -S $< > $@

$(BUILD)/%.map: $(BUILD)/%.elf
	@$(OBJDUMP) -S $< > $@

# pull in dependencies
-include $(OBJS:.o=.d) $(SOLO_OBJS:.o=.d) $(MULTI_OBJS:.o=.d)
