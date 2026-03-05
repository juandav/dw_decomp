/* See LICENSE file for copyright and license details. */

SECTIONS
{
	OVERLAY 0x80052AE0 : NOCROSSREFS SUBALIGN(4)
	{
		.BTL {
			build/asm/btl/data/texture.rodata.s.o(.rodata);
			build/asm/btl/data/model_0.rodata.s.o(.rodata);
			build/asm/btl/data/model_1.rodata.s.o(.rodata);
			build/asm/btl/data/model_2.rodata.s.o(.rodata);
			build/asm/btl/data/model_3.rodata.s.o(.rodata);

			btl_TEXT_START = .;

			build/asm/btl/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/btl/41C8.s.o(.text);
			. = ALIGN(., 4);

			build/asm/btl/41C8.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/btl/41C8.s.o(.data);
			build/asm/btl/data/20280.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/btl/41C8.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS SUBALIGN(4)
	{
		.DGET {
			dget_TEXT_START = .;

			build/asm/dget/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/dget/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/dget/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/dget/40.s.o(.data);
			build/asm/dget/data/14CC.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/dget/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS SUBALIGN(4)
	{
		.DOO2 {
			doo2_TEXT_START = .;

			build/asm/doo2/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/doo2/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/doo2/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/doo2/40.s.o(.data);
			build/asm/doo2/data/1A80.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/doo2/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80080000 : NOCROSSREFS SUBALIGN(4)
	{
		.DOOA {
			dooa_TEXT_START = .;

			build/asm/dooa/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/dooa/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/dooa/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/dooa/40.s.o(.data);
			build/asm/dooa/data/48D4.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/dooa/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS SUBALIGN(4)
	{
		.EAB {
			eab_TEXT_START = .;

			build/asm/eab/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/eab/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/eab/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/eab/40.s.o(.data);
			build/asm/eab/data/16DC.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/eab/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS SUBALIGN(4)
	{
		.ENDI {
			endi_TEXT_START = .;

			build/asm/endi/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/endi/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/endi/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/endi/40.s.o(.data);
			build/asm/endi/data/D98.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/endi/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS SUBALIGN(4)
	{
		.EVL {
			evl_TEXT_START = .;

			build/asm/evl/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/evl/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/evl/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/evl/40.s.o(.data);
			build/asm/evl/data/3EE8.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/evl/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS SUBALIGN(4)
	{
		.FISH {
			fish_TEXT_START = .;

			build/asm/fish/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/fish/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/fish/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/fish/40.s.o(.data);
			build/asm/fish/data/A160.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/fish/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80053800 : NOCROSSREFS SUBALIGN(4)
	{
		.KAR {
			kar_TEXT_START = .;

			build/asm/kar/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/kar/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/kar/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/kar/40.s.o(.data);
			build/asm/kar/data/7310.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/kar/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80010000 : NOCROSSREFS SUBALIGN(4)
	{
		.MOV {
			mov_TEXT_START = .;

			build/asm/mov/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/mov/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/mov/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/mov/40.s.o(.data);
			build/asm/mov/data/94C.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/mov/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x8007C000 : NOCROSSREFS SUBALIGN(4)
	{
		.MURD {
			murd_TEXT_START = .;

			build/asm/murd/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/murd/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/murd/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/murd/40.s.o(.data);
			build/asm/murd/data/1748.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/murd/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS SUBALIGN(4)
	{
		.SHOP {
			shop_TEXT_START = .;

			build/asm/shop/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/shop/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/shop/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/shop/40.s.o(.data);
			build/asm/shop/data/7860.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/shop/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS SUBALIGN(4)
	{
		.STD {
			build/asm/std/data/texture.rodata.s.o(.rodata);
			build/asm/std/data/model_0.rodata.s.o(.rodata);
			build/asm/std/data/model_1.rodata.s.o(.rodata);
			build/asm/std/data/model_2.rodata.s.o(.rodata);
			build/asm/std/data/model_3.rodata.s.o(.rodata);

			std_TEXT_START = .;

			build/asm/std/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/std/41C8.s.o(.text);
			. = ALIGN(., 4);

			build/asm/std/41C8.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/std/41C8.s.o(.data);
			build/asm/std/data/26E0C.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/std/41C8.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS SUBALIGN(4)
	{
		.TRN2 {
			trn2_TEXT_START = .;

			build/asm/trn2/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/trn2/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/trn2/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/trn2/40.s.o(.data);
			build/asm/trn2/data/5130.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/trn2/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS SUBALIGN(4)
	{
		.TRN {
			trn_TEXT_START = .;

			build/asm/trn/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/trn/40.s.o(.text);
			. = ALIGN(., 4);

			build/asm/trn/40.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/trn/40.s.o(.data);
			build/asm/trn/data/6758.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/trn/40.s.o(.bss);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS SUBALIGN(4)
	{
		.VS {
			build/asm/vs/data/texture.rodata.s.o(.rodata);
			build/asm/vs/data/model_0.rodata.s.o(.rodata);
			build/asm/vs/data/model_1.rodata.s.o(.rodata);
			build/asm/vs/data/model_2.rodata.s.o(.rodata);
			build/asm/vs/data/model_3.rodata.s.o(.rodata);

			vs_TEXT_START = .;

			build/asm/vs/data/header.rodata.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/vs/41C8.s.o(.text);
			. = ALIGN(., 4);

			build/asm/vs/41C8.s.o(.rodata);
			. = ALIGN(., 4);

			build/asm/vs/41C8.s.o(.data);
			build/asm/vs/data/1CEDC.data.s.o(.data);
			. = ALIGN(., 4);

			build/asm/vs/41C8.s.o(.bss);
			. = ALIGN(., 4);
		}
	}
}
