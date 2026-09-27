ENTRY(__start)

SECTIONS
{
    __romPos = 0;
    _gp = main_SDATA_START + 0x8000;
    header_ROM_START = __romPos;
    .header 0x80090000 : AT(header_ROM_START)
    {
        KEEP(BUILD_DIR/asm/main/data/header.data.s.o(.data .data.*));
        . = ALIGN(., 4);
    }
    __romPos += SIZEOF(.header);
    __romPos = ALIGN(__romPos, 4);
    . = ALIGN(., 4);

    main_ROM_START = __romPos;
    .main : AT(main_ROM_START)
    {
        main_TEXT_START = .;
        KEEP(BUILD_DIR/asm/main/main.s.o(.text .text.*));
        BUILD_DIR/src/main/fade.c.o(.text .text.*);
        BUILD_DIR/src/main/aabb.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/20E40.s.o(.text .text.*));
        main_TEXT_END = .;
        main_TEXT_SIZE = ABSOLUTE(main_TEXT_END - main_TEXT_START);

        main_RODATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.rodata.s.o(.rodata .rodata.*));
        main_RODATA_END = .;
        main_RODATA_SIZE = ABSOLUTE(main_RODATA_END - main_RODATA_START);

        main_DATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.data.s.o(.data .data.*));
        main_DATA_END = .;
        main_DATA_SIZE = ABSOLUTE(main_DATA_END - main_DATA_START);

        main_SDATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.sdata.s.o(.sdata .sdata.*));
        main_SDATA_END = .;
        main_SDATA_SIZE = ABSOLUTE(main_SDATA_END - main_SDATA_START);

        main_SBSS_START = .;
        . = ALIGN(., 0x800);
    }
    __romPos += SIZEOF(.main);
    __romPos = ALIGN(__romPos, 4);
    main_ROM_END = __romPos;
    main_ROM_SIZE = ABSOLUTE(main_ROM_END - main_ROM_START);
    . = ALIGN(., 4);

    sbss_ROM_START = __romPos;
    .sbss main_SBSS_START : AT(sbss_ROM_START)
    {
        BUILD_DIR/generated/unk_0x8013DF1C.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/fade.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/unk_0x8013DF94.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        main_SBSS_END = .;
        main_SBSS_SIZE = ABSOLUTE(main_SBSS_END - main_SBSS_START);
    }
    __romPos += SIZEOF(.sbss);
    __romPos = ALIGN(__romPos, 4);

    /DISCARD/ :
    {
        *(*);
    }
}
