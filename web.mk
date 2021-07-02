ifeq ($(CONFIG),Debug)
EMCCFLAGS   ?= -g
endif

ifeq ($(CONFIG),Release)
EMCCFLAGS   ?= -Oz # optimise for size
endif

LIBSOURCE    = $(BUILDROOT)/LibSource
SOURCE       = $(BUILDROOT)/Source
BUILDSOURCE    = $(BUILD)/Source
DAISYSP      = $(BUILDROOT)/Libraries/DaisySP/Source

# emscripten
EMCC      ?= emcc
EMAR      ?= emar
EMCCFLAGS += -fno-rtti -fno-exceptions
# EMCCFLAGS += -s ASSERTIONS=1 -Wall
EMCCFLAGS += -I$(SOURCE) -I$(PATCHSOURCE) -I$(LIBSOURCE) -I$(BUILDSOURCE) -I$(BUILD)
EMCCFLAGS += -I$(BUILD)/Source
EMCCFLAGS += -I$(DAISYSP)
EMCCFLAGS += -I$(DAISYSP)/Control
EMCCFLAGS += -I$(DAISYSP)/Drums
EMCCFLAGS += -I$(DAISYSP)/Dynamics
EMCCFLAGS += -I$(DAISYSP)/Effects
EMCCFLAGS += -I$(DAISYSP)/Filters
EMCCFLAGS += -I$(DAISYSP)/Noise
EMCCFLAGS += -I$(DAISYSP)/PhysicalModeling
EMCCFLAGS += -I$(DAISYSP)/Synthesis
EMCCFLAGS += -I$(DAISYSP)/Utility
EMCCFLAGS +=  -ILibraries -ILibraries/KissFFT -DHV_SIMD_NONE -DDSY_CORE_DSP -DDSY_CUSTOM_DSP
EMCCFLAGS += -Wno-warn-absolute-paths
EMCCFLAGS += -Wno-unknown-warning-option
EMCCFLAGS += --memory-init-file 0 # don't create separate memory init file .mem
EMCCFLAGS += -s EXPORTED_FUNCTIONS="['_WEB_setup','_WEB_setParameter','_WEB_getParameter','_WEB_processBlock','_WEB_getPatchName','_WEB_getParameterName','_WEB_getMessage','_WEB_getStatus','_WEB_getButtons','_WEB_setButton', '_WEB_processMidi', '_malloc']"
EMCCFLAGS += -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']"

include $(BUILDROOT)/sources.mk
CPP_SRC += Source/Patch.cpp
CPP_SRC += Source/PatchProcessor.cpp
CPP_SRC += Source/message.cpp
CPP_SRC += WebSource/web.cpp
C_SRC   += Libraries/KissFFT/kiss_fft.c

ifdef MAXIMILIAN
EMCC_SRC := $(filter-out $(PATCHSOURCE)/$(MAXIMILIAN).cpp, $(EMCC_SRC))
endif
WEBDIR   = $(BUILD)/web

EMCCFLAGS += -s WASM=0 # disables wasm output

CPPFLAGS =
CFLAGS   = $(EMCCFLAGS) -std=gnu11
CXXFLAGS = $(EMCCFLAGS) -std=gnu++17
LDFLAGS  = $(EMCCFLAGS)

EMCC_OBJS = $(addprefix $(WEBDIR)/, $(notdir $(CPP_SRC:.cpp=.o)))
EMCC_OBJS += $(addprefix $(WEBDIR)/, $(notdir $(C_SRC:.c=.o)))

EMDAISYSP_CPP_SRC = $(wildcard $(DAISYSP)/*/*.cpp)
EMDAISYSP_OBJS = $(addprefix $(WEBDIR)/, $(notdir $(EMDAISYSP_CPP_SRC:.cpp=.o)))

PATCH_C_SRC  = $(wildcard $(BUILDSOURCE)/*.c)
PATCH_C_SRC  += $(wildcard $(PATCHSOURCE)/*.c)
PATCH_CPP_SRC = $(SOURCE)/PatchProgram.cpp
PATCH_CPP_SRC += $(wildcard $(BUILDSOURCE)/*.cpp)
PATCH_CPP_SRC += $(wildcard $(PATCHSOURCE)/*.cpp)

PATCH_OBJS = $(addprefix $(WEBDIR)/, $(notdir $(PATCH_CPP_SRC:.cpp=.o)))
PATCH_OBJS += $(addprefix $(WEBDIR)/, $(notdir $(PATCH_C_SRC:.c=.o)))

OBJS  = $(addprefix $(BUILD)/,$(notdir $(C_SRC:.c=.o)))
OBJS += $(addprefix $(BUILD)/,$(notdir $(CPP_SRC:.cpp=.o)))
# OBJS += $(addprefix $(BUILD)/,$(notdir $(S_SRC:.S=.o)))

# Set up search path
vpath %.c $(sort $(dir $(C_SRC)))
vpath %.cpp $(sort $(dir $(CPP_SRC)))
vpath %.s $(sort $(dir $(S_SRC)))
vpath %.S $(sort $(dir $(S_SRC)))
vpath %.cpp $(sort $(dir $(EMDAISYSP_CPP_SRC)))

vpath %.cpp $(SOURCE)
vpath %.c $(SOURCE)
vpath %.s $(SOURCE)
vpath %.cpp $(LIBSOURCE)
vpath %.c $(LIBSOURCE)
vpath %.s $(LIBSOURCE)
vpath %.cpp $(PATCHSOURCE)
vpath %.c $(PATCHSOURCE)
vpath %.s $(PATCHSOURCE)
vpath %.cpp $(BUILDSOURCE)
vpath %.c $(BUILDSOURCE)
vpath %.s $(BUILDSOURCE)
vpath %.cpp WebSource
vpath %.c Libraries/KissFFT

ifeq ($(CONFIG),Debug)
EMCCFLAGS += -s ASSERTIONS=1
endif

PHONY: libs web minify

# JavaScript minifiers
#CLOSURE = java -jar Tools/node_modules/google-closure-compiler/compiler.jar --language_in=ECMASCRIPT5
UGLIFYJS = Tools/node_modules/uglifyjs/bin/uglifyjs

$(WEBDIR)/$(TARGET).js: $(PATCH_OBJS)
	$(EMCC) $(LDFLAGS) $(PATCH_OBJS) -o $(WEBDIR)/$(TARGET).js Libraries/libowlweb.a -Wl,--whole-archive Libraries/libdaisyspweb.a -Wl,--no-whole-archive
	@cp WebSource/*.js WebSource/*.html $(WEBDIR)

$(WEBDIR)/%.min.js: $(WEBDIR)/%.js
	@$(UGLIFYJS) -o $@ $<
#	@$(CLOSURE) --js_output_file=$@ $<

Libraries/libowlweb.a: $(EMCC_OBJS)
	$(EMAR) rcs $@ $^

Libraries/libdaisyspweb.a: $(EMDAISYSP_OBJS)
	$(EMAR) rcs $@ $^

libs: Libraries/libowlweb.a Libraries/libdaisyspweb.a

# compile and generate dependency info
$(WEBDIR)/%.o: %.c
	@$(EMCC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@
	@$(EMCC) -MM -MT"$@" $(CPPFLAGS) $(CFLAGS) $< > $(@:.o=.d)

$(WEBDIR)/%.o: %.cpp
	@$(EMCC) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
	@$(EMCC) -MM -MT"$@" $(CPPFLAGS) $(CXXFLAGS) $< > $(@:.o=.d)

web: $(WEBDIR)/$(TARGET).js ;

minify: $(WEBDIR)/$(TARGET).min.js ;
