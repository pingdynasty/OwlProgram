BUILDROOT ?= .

ifndef CONFIG
ifdef TEST
  CONFIG=Debug
else
  CONFIG=Release
endif
endif

ifndef PLATFORM
  PLATFORM=OWL
endif

DEPS = .FORCE
TARGET ?= patch

ifdef FAUST
# options for FAUST compilation
PATCHNAME   ?= $(FAUST)
PATCHCLASS  ?= FaustPatch
PATCHFILE   ?= FaustPatch.hpp
DEPS        += faust
else ifdef HEAVY
# options for Heavy PD compilation
PATCHNAME   ?= $(HEAVY)
PATCHCLASS  ?= HeavyPatch
PATCHFILE   ?= HeavyPatch.hpp
DEPS        += heavy
else ifdef GEN
# options for Max/MSP Gen compilation
PATCHNAME   ?= $(GEN)
PATCHCLASS  ?= GenPatch
PATCHFILE   ?= GenPatch.hpp
DEPS        += gen
else ifdef MAXIMILIAN
# options for Maximilian compilation
PATCHNAME   ?= $(MAXIMILIAN)
PATCHCLASS  ?= MaximilianPatch
PATCHFILE   ?= MaximilianPatch.hpp
DEPS        += maximilian
else ifdef SOUL
# options for SOUL patch compilation
PATCHNAME   ?= $(SOUL)
PATCHCLASS  ?= SoulPatch
PATCHFILE   ?= SoulPatch.hpp
SOULCLASS   ?= $(SOUL)
SOULFILE    ?= $(SOUL).soulpatch
SOULHPP     ?= $(SOUL).hpp
DEPS        += soul
else ifdef TEST
PATCHNAME   = $(TEST)
PATCHCLASS  = $(PATCHNAME)Patch
PATCHFILE   = $(PATCHNAME)Patch.hpp
else
# options for C++ compilation
PATCHNAME   ?= "Template"
PATCHCLASS  ?= $(PATCHNAME)Patch
PATCHFILE   ?= $(PATCHNAME)Patch.hpp
endif

PATCHIN     ?= 2
PATCHOUT    ?= 2
SLOT        ?= 0
OWLDEVICE   ?= OWL-*
BUILD       ?= $(BUILDROOT)/Build
PATCHSOURCE ?= $(BUILDROOT)/PatchSource
FIRMWARESENDER ?= Tools/FirmwareSender

# set up tests
TEST_FILES = $(notdir $(wildcard $(BUILDROOT)/TestPatches/*TestPatch.hpp))
TESTS = $(filter-out ShortFastFourierTest, $(TEST_FILES:%Patch.hpp=%))

export BUILD BUILDROOT TARGET
export PATCHNAME PATCHCLASS PATCHSOURCE 
export PATCHFILE PATCHIN PATCHOUT
export HEAVYTOKEN HEAVYSERVICETOKEN  HEAVY
export SOUL SOULCLASS SOULFILE SOULHPP
export CONFIG PLATFORM

DEPS += $(BUILD)/registerpatch.cpp $(BUILD)/registerpatch.h $(BUILD)/Source/startup.s 

all: libs patch web

.PHONY: .FORCE patch libs faust gen heavy soul maximilian web minify map as native test run check tables resource size clean realclean sysex load store docs help

.FORCE:
	@mkdir -p $(BUILD)/Source
	@mkdir -p $(BUILD)/web
	@mkdir -p $(BUILD)/Test

$(BUILD)/registerpatch.cpp: .FORCE
	@echo "REGISTER_PATCH($(PATCHCLASS), \"$(PATCHNAME)\", $(PATCHIN), $(PATCHOUT));" > $@

$(BUILD)/registerpatch.h: .FORCE
	@echo Building patch $(PATCHNAME)
	@echo "#include \"$(PATCHFILE)\"" > $@

$(BUILD)/Source/startup.s: .FORCE
	@echo '.string "'$(PATCHNAME)'"' > $(BUILD)/Source/progname.s

$(BUILD)/%.syx: $(BUILD)/%.bin
	@$(FIRMWARESENDER) -q -in $< -save $@

patch: $(DEPS) ## build patch binary
	@$(MAKE) -s -f compile.mk compile

libs: .FORCE ## build patch libraries
	@$(MAKE) -s -f compile.mk libs
	@$(MAKE) -s -f web.mk libs

web: $(DEPS) ## build Javascript patch
	@$(MAKE) -s -f web.mk web
	@echo Built javascript $(PATCHNAME) in $(BUILD)/web/$(TARGET).js

minify: $(DEPS)
	@$(MAKE) -s -f web.mk minify

faust: .FORCE
	@$(MAKE) -s -f faust.mk faust

heavy: .FORCE
	@$(MAKE) -s -f heavy.mk heavy

gen: .FORCE
	@$(MAKE) -s -f gen.mk gen

maximilian: .FORCE
	@$(MAKE) -s -f maximilian.mk maximilian

soul: .FORCE
	@$(MAKE) -s -f soul.mk soul

sysex: patch $(BUILD)/$(TARGET).syx ## package patch binary as MIDI sysex
	@echo Built sysex $(PATCHNAME) in $(BUILD)/$(TARGET).syx

load: patch ## upload patch to attached OWL via MIDI
	@echo Sending patch $(PATCHNAME) to $(OWLDEVICE) to load
	@$(FIRMWARESENDER) -q -in $(BUILD)/$(TARGET).bin -out "$(OWLDEVICE)" -run

store: patch ## upload and save patch to attached OWL
	@echo Sending patch $(PATCHNAME) to $(OWLDEVICE) to store in slot $(SLOT)
	@$(FIRMWARESENDER) -q -in $(BUILD)/$(TARGET).bin -out "$(OWLDEVICE)" -store $(SLOT)

resource: $(RESOURCE) ## upload and save resource to attached OWL
	@echo Sending resource $(RESOURCE) to $(OWLDEVICE) to store in slot $(SLOT)
	@$(FIRMWARESENDER) -q -in $(RESOURCE) -out "$(OWLDEVICE)" -store $(SLOT)

docs: ## generate HTML documentation
	@doxygen Doxyfile

tables: ## compile tools and generate lookup tables
	@$(MAKE) -s -f tables.mk tables

clean: ## remove generated patch files
	@rm -rf $(BUILD)/*

realclean: clean ## remove all library object files
	@find Libraries/ -name '*.[a|o]' -delete

size: patch ## show binary size metrics and large object summary
	@$(MAKE) -s -f compile.mk size

map: patch ## build map file (Build/patch.map)
	@$(MAKE) -s -f compile.mk map
	@echo Built $(PATCHNAME) map in $(BUILD)/$(TARGET).map

as: patch ## build assembly file (Build/patch.s)
	@$(MAKE) -s -f compile.mk as
	@echo Built $(PATCHNAME) assembly in $(BUILD)/$(TARGET).s

native: $(DEPS) ## build native executable of patch
	@$(MAKE) -s -f native.mk native

test: $(DEPS) ## test patch natively
	@$(MAKE) -s -f native.mk test

run: $(DEPS) ## run patch natively
	@$(MAKE) -s -f native.mk run

grind: $(DEPS) ## run valgrind on patch natively
	@$(MAKE) -s -f native.mk grind

check: ## run test patches and unit tests natively
	@for nm in $(TESTS) ; do \
		$(MAKE) -s TEST=$$nm test || exit;\
	done

help: ## show this help
	@echo 'Usage: make [target] ...'
	@echo 'Targets:'
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e  's/^\(.*\): .*##\(.*\)/\1:#\2/' | column -t -c 2 -s '#'
