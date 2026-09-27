SECTIONS
{
	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.BTL {
			KEEP(BUILD_DIR/asm/btl/data/texture.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_0.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_1.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_2.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_3.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/btl.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/btl/data/btl.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS
	{
		.DGET {
			KEEP(BUILD_DIR/asm/dget/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/dget/dget.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/dget/data/dget.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS
	{
		.DOO2 {
			KEEP(BUILD_DIR/asm/doo2/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/doo2/doo2.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/doo2/data/doo2.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80080000 : NOCROSSREFS
	{
		.DOOA {
			KEEP(BUILD_DIR/asm/dooa/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/dooa/dooa.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/dooa/data/dooa.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EAB {
			KEEP(BUILD_DIR/asm/eab/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/eab/eab.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/eab/data/eab.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.ENDI {
			KEEP(BUILD_DIR/asm/endi/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/endi/endi.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/endi/data/endi.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EVL {
			KEEP(BUILD_DIR/asm/evl/data/evl.s.o(.data.*));
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS
	{
		.FISH {
			KEEP(BUILD_DIR/asm/fish/data/fish.s.o(.data.*));
		}
	}

	OVERLAY 0x80053800 : NOCROSSREFS
	{
		.KAR {
			KEEP(BUILD_DIR/asm/kar/data/kar.s.o(.data.*));
		}
	}

	OVERLAY 0x80010000 : NOCROSSREFS
	{
		.MOV {
			KEEP(BUILD_DIR/asm/mov/data/mov.s.o(.data.*));
		}
	}

	OVERLAY 0x8007C000 : NOCROSSREFS
	{
		.MURD {
			KEEP(BUILD_DIR/asm/murd/data/murd.s.o(.data.*));
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS
	{
		.SHOP {
			KEEP(BUILD_DIR/asm/shop/data/shop.s.o(.data.*));
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.STD {
			KEEP(BUILD_DIR/asm/std/data/std.s.o(.data.*));
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN2 {
			KEEP(BUILD_DIR/asm/trn2/data/trn2.s.o(.data.*));
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN {
			KEEP(BUILD_DIR/asm/trn/data/trn.s.o(.data.*));
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.VS {
			KEEP(BUILD_DIR/asm/vs/data/vs.s.o(.data.*));
		}
	}
}
