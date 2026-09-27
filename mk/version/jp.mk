EXE_NAME := SLPS_017.97

MWCC_OPT_LEVEL := 0

MAIN_SBSS := \
	$(GEN_DIR)/unk_0x8013DF1C.sbss.s \
	$(GEN_DIR)/unk_0x8013DF94.sbss.s

MAIN_BSS := \
	$(GEN_DIR)/unk_0x8013E6A8.bss.s \
	$(GEN_DIR)/unk_0x80168920.bss.s

MAIN_GEN_SRC := $(MAIN_BSS) $(MAIN_SBSS)

MAIN_C_SRC := \
	src/main/aabb.c \
	src/main/butterfly.c \
	src/main/fade.c

$(eval $(call unit,MAIN,main))

$(eval $(call overlay,BTL,btl))
$(eval $(call overlay,DGET,dget))
$(eval $(call overlay,DOO2,doo2))
$(eval $(call overlay,DOOA,dooa))
$(eval $(call overlay,EAB,eab))
$(eval $(call overlay,ENDI,endi))
$(eval $(call overlay,EVL,evl))
$(eval $(call overlay,FISH,fish))
$(eval $(call overlay,KAR,kar))
$(eval $(call overlay,MOV,mov))
$(eval $(call overlay,MURD,murd))
$(eval $(call overlay,SHOP,shop))
$(eval $(call overlay,STD,std))
$(eval $(call overlay,TRN2,trn2))
$(eval $(call overlay,TRN,trn))
$(eval $(call overlay,VS,vs))

UNDEFINED_SYMS := $(foreach u,main $(shell echo $(OVERLAY) | tr A-Z a-z), \
	$(GEN_DIR)/undefined_funcs_auto_$(u).ld \
	$(GEN_DIR)/undefined_syms_auto_$(u).ld)
