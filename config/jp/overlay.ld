SECTIONS
{
	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.BTL {
			KEEP(BUILD_DIR/asm/btl/data/btl.s.o(.data.*));
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
			KEEP(BUILD_DIR/asm/doo2/data/doo2.s.o(.data.*));
		}
	}

	OVERLAY 0x80080000 : NOCROSSREFS
	{
		.DOOA {
			KEEP(BUILD_DIR/asm/dooa/data/dooa.s.o(.data.*));
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EAB {
			KEEP(BUILD_DIR/asm/eab/data/eab.s.o(.data.*));
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.ENDI {
			KEEP(BUILD_DIR/asm/endi/data/endi.s.o(.data.*));
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
