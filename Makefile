#---------------------------------------------------------------------------------
# Install snesdtc before using this Makefile
# https://github.com/nArnoSNES/rgc2016
#---------------------------------------------------------------------------------

include snes_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
SOURCES		:=	.

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	+=	$(INCLUDE) 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	./lib
#LIBOBJS +:=	
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))
 
export AUDIOFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.it)))

ifneq ($(AUDIOFILES),)
ifeq ($(wildcard soundbank.asm),)
	SFILES		:=	soundbank.asm $(SFILES)
endif
endif

#---------------------------------------------------------------------------------
export OFILES	:=	$(BINFILES:.bin=.obj) $(CFILES:.c=.obj) $(SFILES:.asm=.obj)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

SMCONVFLAGS	:= -l -s -o soundbank -v -b 7
					
GTITLE 		:= -ht"$(TARGET)"
 
.PHONY: bitmaps all
 
#---------------------------------------------------------------------------------
all	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc

clean:
	@echo clean ...
	@rm -f $(OFILES) $(TARGET).sfc $(TARGET).sym *.pic *.pal *.map *.bnk *.brr soundbank.asm soundbank.h *.clm

sfc	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc
	@rm -f $(OFILES) $(TARGET).sym *.pic *.pal *.map *.bnk *.brr soundbank.asm soundbank.h *.clm

#---------------------------------------------------------------------------------
soundbank.asm : $(AUDIOFILES)
	@echo Compiling Soundbank ...
	@$(SMCONV) $(SMCONVFLAGS) $^	

#--------------------------------------------------------------------------------
snesfont.pic: snesfont.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs8 -po4 -pc16 -pe1 -mR! -m! -p! $<
	
bg1.pic: bg1.bmp
	@echo convert without rearranging color order ... $(notdir $@)
	$(GFXCONV) -pc16 -n -gs8 -pe0 -fbmp -m $<
	
coll1.clm: coll1.bmp
	@echo convert collision map ... $(notdir $@)
	$(GFXCONV) -gb -pc4 -n -gs8 -mc $<

sprites.pic: sprites.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs32 -pc16 -po16 -n $<
	
bitmaps : snesfont.pic bg1.pic coll1.clm sprites.pic

#---------------------------------------------------------------------------------
$(OUTPUT).sfc	: $(OFILES)
