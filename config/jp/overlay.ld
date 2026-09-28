SECTIONS
{
	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.BTL {
			btl_START = .;

			KEEP(BUILD_DIR/asm/btl/data/texture.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_0.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_1.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_2.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_3.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/btl/battle_setup.c.o(.text .text.*);
			KEEP(BUILD_DIR/asm/btl/5498.s.o(.text .text.*));
			BUILD_DIR/src/btl/command_shout.c.o(.text .text.*);
			BUILD_DIR/src/btl/command_menu.c.o(.text .text.*);
			BUILD_DIR/src/btl/battle_hud.c.o(.text .text.*);
			KEEP(BUILD_DIR/asm/btl/15DD0.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/btl/data/btl.data.s.o(.data .data.*));
			BUILD_DIR/src/btl/battle_setup.c.o(.rodata .rodata.*);
			BUILD_DIR/src/btl/command_shout.c.o(.rodata .rodata.*);
			BUILD_DIR/src/btl/command_menu.c.o(.rodata .rodata.*);
			BUILD_DIR/src/btl/battle_hud.c.o(.rodata .rodata.*);
			BUILD_DIR/src/btl/command_shout.c.o(.data .data.*);
			KEEP(BUILD_DIR/asm/btl/data/28B68.data.s.o(.data .data.*));
			BUILD_DIR/src/btl/battle_hud.c.o(.data .data.*);
			KEEP(BUILD_DIR/asm/btl/data/28C10.data.s.o(.data .data.*));
			BUILD_DIR/src/btl/btl_bss.c.o(.data .data.*);
			LONG(0);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS
	{
		.DGET {
			dget_START = .;

			KEEP(BUILD_DIR/asm/dget/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/dget/dget.c.o(.text .text.*);
			BUILD_DIR/src/dget/dget.c.o(.rodata .rodata.*);
			BUILD_DIR/src/dget/dget.c.o(.data .data.*);
			LONG(0);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS
	{
		.DOO2 {
			doo2_START = .;

			KEEP(BUILD_DIR/asm/doo2/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/doo2/doo2.c.o(.text .text.*);
			BUILD_DIR/src/doo2/doo2.c.o(.rodata .rodata.*);
			BUILD_DIR/src/doo2/doo2.c.o(.data .data.*);
			BUILD_DIR/src/doo2/doo2_bss.c.o(.data .data.*);
		}
	}

	OVERLAY 0x80080000 : NOCROSSREFS
	{
		.DOOA {
			dooa_START = .;

			KEEP(BUILD_DIR/asm/dooa/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/dooa/dooa.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/dooa/data/dooa.data.s.o(.data .data.*));
			BUILD_DIR/src/dooa/dooa_bss.c.o(.data .data.*);
			LONG(0);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EAB {
			eab_START = .;

			KEEP(BUILD_DIR/asm/eab/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/eab/eab.c.o(.text .text.*);
			BUILD_DIR/src/eab/eab.c.o(.rodata .rodata.*);
			BUILD_DIR/src/eab/eab.c.o(.data .data.*);
			BUILD_DIR/src/eab/eab_bss.c.o(.data .data.*);
			LONG(0);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.ENDI {
			endi_START = .;

			KEEP(BUILD_DIR/asm/endi/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/endi/endi.c.o(.text .text.*);
			BUILD_DIR/src/endi/endi.c.o(.rodata .rodata.*);
			BUILD_DIR/src/endi/endi.c.o(.data .data.*);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EVL {
			evl_START = .;

			KEEP(BUILD_DIR/asm/evl/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/evl/evl.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/evl/data/evl.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS
	{
		.FISH {
			fish_START = .;

			KEEP(BUILD_DIR/asm/fish/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/fish/fish.s.o(.text .text.*));
			BUILD_DIR/src/fish/fish_model.c.o(.text .text.*);
			BUILD_DIR/src/fish/fish_model.c.o(.rodata .rodata.*);
			KEEP(BUILD_DIR/asm/fish/data/fish.data.s.o(.data .data.*));
			BUILD_DIR/src/fish/fish_bss.c.o(.data .data.*);
			LONG(0);
		}
	}

	OVERLAY 0x80053800 : NOCROSSREFS
	{
		.KAR {
			kar_START = .;

			KEEP(BUILD_DIR/asm/kar/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/kar/kar.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/kar/data/kar.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80010000 : NOCROSSREFS
	{
		.MOV {
			mov_START = .;

			KEEP(BUILD_DIR/asm/mov/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/mov/mov.c.o(.text .text.*);
			BUILD_DIR/src/mov/mov.c.o(.rodata .rodata.*);
			BUILD_DIR/src/mov/mov.c.o(.data .data.*);
			KEEP(BUILD_DIR/asm/mov/data/C10.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x8007C000 : NOCROSSREFS
	{
		.MURD {
			murd_START = .;

			KEEP(BUILD_DIR/asm/murd/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/murd/murd.c.o(.text .text.*);
			BUILD_DIR/src/murd/murd.c.o(.rodata .rodata.*);
			BUILD_DIR/src/murd/murd.c.o(.data .data.*);
			BUILD_DIR/src/murd/murd_bss.c.o(.data .data.*);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS
	{
		.SHOP {
			KEEP(BUILD_DIR/asm/shop/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/shop/shop.s.o(.text .text.*));
			BUILD_DIR/src/main/script_menu.c.o(.text .text.*);
			BUILD_DIR/src/main/script_menu.c.o(.rodata .rodata.*);
			KEEP(BUILD_DIR/asm/shop/data/shop.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.STD {
			std_START = .;

			KEEP(BUILD_DIR/asm/std/data/texture.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/model_0.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/model_1.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/model_2.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/model_3.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/std/std_setup.c.o(.text .text.*);
			KEEP(BUILD_DIR/asm/std/5E80.s.o(.text .text.*));
			BUILD_DIR/src/std/std_hud.c.o(.text .text.*);
			KEEP(BUILD_DIR/asm/std/1F208.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/std/data/std.data.s.o(.data .data.*));
			BUILD_DIR/src/std/std_setup.c.o(.rodata .rodata.*);
			BUILD_DIR/src/std/std_setup.c.o(.data .data.*);
			KEEP(BUILD_DIR/asm/std/data/317C0.data.s.o(.data .data.*));
			BUILD_DIR/src/std/std_hud.c.o(.rodata .rodata.*);
			BUILD_DIR/src/std/std_hud.c.o(.data .data.*);
			KEEP(BUILD_DIR/asm/std/data/3264C.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN2 {
			trn2_START = .;

			KEEP(BUILD_DIR/asm/trn2/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/trn2/trn2_hp_map99.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_def_map99.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_hp_map107.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_mp.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_off.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_def_map108.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_reward.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_hud.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_slots.c.o(.text .text.*);
			BUILD_DIR/src/trn2/trn2_hp_map107.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_hp_map99.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_mp.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_off.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_reward.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_def_map108.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_def_map99.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_hud.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_slots.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn2/trn2_off.c.o(.data .data.*);
			BUILD_DIR/src/trn2/trn2_hud.c.o(.data .data.*);
			BUILD_DIR/src/trn2/trn2_slots.c.o(.data .data.*);
			BUILD_DIR/src/trn2/trn2_bss.c.o(.data .data.*);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN {
			trn_START = .;

			KEEP(BUILD_DIR/asm/trn/data/header.rodata.s.o(.rodata .rodata.*));
			BUILD_DIR/src/trn/trn_reward.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_hp.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_mp.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_off.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_def.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_speed.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_brain.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_hud.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_slots.c.o(.text .text.*);
			BUILD_DIR/src/trn/trn_hp.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_off.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_speed.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_def.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_mp.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_brain.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_hud.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_reward.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_slots.c.o(.rodata .rodata.*);
			BUILD_DIR/src/trn/trn_reward.c.o(.data .data.*);
			BUILD_DIR/src/trn/trn_hud.c.o(.data .data.*);
			BUILD_DIR/src/trn/trn_slots.c.o(.data .data.*);
			BUILD_DIR/src/trn/trn_bss.c.o(.data .data.*);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.VS {
			vs_START = .;

			KEEP(BUILD_DIR/asm/vs/data/texture.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/model_0.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/model_1.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/model_2.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/model_3.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/vs.s.o(.text .text.*));
			BUILD_DIR/src/vs/vs_hud.c.o(.text .text.*);
			KEEP(BUILD_DIR/asm/vs/1F8FC.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/vs/data/vs.data.s.o(.data .data.*));
			BUILD_DIR/src/vs/vs_hud.c.o(.rodata .rodata.*);
			KEEP(BUILD_DIR/asm/vs/data/305B4.data.s.o(.data .data.*));
			BUILD_DIR/src/vs/vs_hud.c.o(.data .data.*);
			KEEP(BUILD_DIR/asm/vs/data/319C0.data.s.o(.data .data.*));
			LONG(0);
		}
	}
}
