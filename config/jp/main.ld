ENTRY(__start)

SECTIONS
{
    __romPos = 0;
    header_ROM_START = __romPos;
    .header 0x80090000 : AT(header_ROM_START)
    {
        KEEP(BUILD_DIR/asm/main/data/header.s.o(.data.*));
    }
    __romPos += SIZEOF(.header);

    main_ROM_START = __romPos;
    .main : AT(main_ROM_START)
    {
        KEEP(BUILD_DIR/asm/main/data/main.s.o(.data.*));
        . = ALIGN(., 0x800);
    }
    __romPos += SIZEOF(.main);

    /DISCARD/ :
    {
        *(*);
    }
}
