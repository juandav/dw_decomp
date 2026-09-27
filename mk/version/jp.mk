EXE_NAME := SLPS_017.97

MWCC_OPT_LEVEL := 0

MAIN_SBSS := \
	$(GEN_DIR)/unk_0x8013DF1C.sbss.s \
	$(GEN_DIR)/unk_0x8013DF94.sbss.s

MAIN_BSS := \
	$(GEN_DIR)/unk_0x8013E6A8.bss.s \
	$(GEN_DIR)/unk_0x80140CF4.bss.s \
	$(GEN_DIR)/unk_0x80168920.bss.s

MAIN_GEN_SRC := $(MAIN_BSS) $(MAIN_SBSS)

MAIN_C_SRC := \
	src/main/_psstart.c \
	src/main/aabb.c \
	src/main/bubble.c \
	src/main/butterfly.c \
	src/main/efe.c \
	src/main/efe_table.c \
	src/main/evl.c \
	src/main/fade.c \
	src/main/fish.c \
	src/main/kar.c \
	src/main/math.c \
	src/main/overworld_card_text.c \
	src/main/overworld_evochart_text.c \
	src/main/overworld_medal_text.c \
	src/main/overworld_moves_text.c \
	src/main/overworld_playerinfo_text.c \
	src/main/toilet_data.c \
	src/main/world_object.c

$(eval $(call unit,MAIN,main))

BTL_C_SRC := \
	src/btl/command_shout.c

$(eval $(call overlay,BTL,btl))
$(eval $(call overlay,DGET,dget))
$(eval $(call overlay,DOO2,doo2))
$(eval $(call overlay,DOOA,dooa))
$(eval $(call overlay,EAB,eab))
$(eval $(call overlay,ENDI,endi))
$(eval $(call overlay,EVL,evl))
FISH_C_SRC := \
	src/fish/fish_model.c

$(eval $(call overlay,FISH,fish))
$(eval $(call overlay,KAR,kar))
$(eval $(call overlay,MOV,mov))
$(eval $(call overlay,MURD,murd))
$(eval $(call overlay,SHOP,shop))
$(eval $(call overlay,STD,std))
TRN2_C_SRC := \
	src/trn2/trn2_mp.c \
	src/trn2/trn2_bss.c

$(eval $(call overlay,TRN2,trn2))
TRN_C_SRC := \
	src/trn/trn_speed.c \
	src/trn/trn_bss.c

$(eval $(call overlay,TRN,trn))
$(eval $(call overlay,VS,vs))

UNDEFINED_SYMS := $(foreach u,main $(shell echo $(OVERLAY) | tr A-Z a-z), \
	$(GEN_DIR)/undefined_funcs_auto_$(u).ld \
	$(GEN_DIR)/undefined_syms_auto_$(u).ld)
