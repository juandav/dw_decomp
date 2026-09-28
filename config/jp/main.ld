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
        BUILD_DIR/src/main/world_object.c.o(.text .text.*);
        BUILD_DIR/src/main/file.c.o(.text .text.*);
        BUILD_DIR/src/main/math.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/14DF8.s.o(.text .text.*));
        BUILD_DIR/src/main/_psstart.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/16190.s.o(.text .text.*));
        BUILD_DIR/src/main/fade.c.o(.text .text.*);
        BUILD_DIR/src/main/aabb.c.o(.text .text.*);
        BUILD_DIR/src/main/efe.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/242CC.s.o(.text .text.*));
        BUILD_DIR/src/main/overworld_evochart_view.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/35208.s.o(.text .text.*));
        BUILD_DIR/src/main/overworld_moves_text.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/36CC0.s.o(.text .text.*));
        BUILD_DIR/src/main/overworld_playerinfo_text.c.o(.text .text.*);
        BUILD_DIR/src/main/overworld_evochart_text.c.o(.text .text.*);
        BUILD_DIR/src/main/overworld_medal_text.c.o(.text .text.*);
        BUILD_DIR/src/main/overworld_card_text.c.o(.text .text.*);
        BUILD_DIR/src/main/map_collision.c.o(.text .text.*);
        BUILD_DIR/src/main/particle.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/39950.s.o(.text .text.*));
        BUILD_DIR/src/main/entity_text.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/621F4.s.o(.text .text.*));
        BUILD_DIR/src/main/file_queue.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/70ECC.s.o(.text .text.*));
        BUILD_DIR/src/main/fish.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/72DFC.s.o(.text .text.*));
        BUILD_DIR/src/main/tournament.c.o(.text .text.*);
        BUILD_DIR/src/main/evl.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/75B80.s.o(.text .text.*));
        BUILD_DIR/src/main/kar.c.o(.text .text.*);
        BUILD_DIR/src/main/efe_table.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/78668.s.o(.text .text.*));
        BUILD_DIR/src/main/butterfly.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/79D94.s.o(.text .text.*));
        BUILD_DIR/src/main/bubble.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/84F04.s.o(.text .text.*));
        main_TEXT_END = .;
        main_TEXT_SIZE = ABSOLUTE(main_TEXT_END - main_TEXT_START);

        main_RODATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.rodata.s.o(.rodata .rodata.*));
        BUILD_DIR/src/main/efe.c.o(.rodata .rodata.*);
        KEEP(BUILD_DIR/asm/main/data/8A71C.rodata.s.o(.rodata .rodata.*));
        BUILD_DIR/src/main/overworld_evochart_view.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/overworld_moves_text.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/overworld_playerinfo_text.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/overworld_medal_text.c.o(.rodata .rodata.*);
        KEEP(BUILD_DIR/asm/main/data/8AD50.rodata.s.o(.rodata .rodata.*));
        BUILD_DIR/src/main/bubble.c.o(.rodata .rodata.*);
        main_RODATA_END = .;
        main_RODATA_SIZE = ABSOLUTE(main_RODATA_END - main_RODATA_START);

        main_DATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.data.s.o(.data .data.*));
        BUILD_DIR/src/main/toilet_data.c.o(.data .data.*);
        BUILD_DIR/src/main/door_mapdata.c.o(.data .data.*);
        BUILD_DIR/src/main/efe.c.o(.data .data.*);
        KEEP(BUILD_DIR/asm/main/data/9F198.data.s.o(.data .data.*));
        BUILD_DIR/src/main/entity_text.c.o(.data .data.*);
        KEEP(BUILD_DIR/asm/main/data/A6BF4.data.s.o(.data .data.*));
        BUILD_DIR/src/main/efe_table.c.o(.data .data.*);
        KEEP(BUILD_DIR/asm/main/data/A8A78.data.s.o(.data .data.*));
        BUILD_DIR/src/main/butterfly.c.o(.data .data.*);
        KEEP(BUILD_DIR/asm/main/data/A9A68.data.s.o(.data .data.*));
        BUILD_DIR/src/main/bubble.c.o(.data .data.*);
        KEEP(BUILD_DIR/asm/main/data/AA8D4.data.s.o(.data .data.*));
        main_DATA_END = .;
        main_DATA_SIZE = ABSOLUTE(main_DATA_END - main_DATA_START);

        main_SDATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/main/efe.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/AD4F0.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/main/entity_text.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/AD6C4.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/main/file_queue.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/AD7C8.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/main/efe_table.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/AD7D8.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/main/bubble.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/AD8E4.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/btl/command_shout.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/AD94C.sdata.s.o(.sdata .sdata.*));
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
        BUILD_DIR/src/main/file_queue.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/unk_0x8013E1FC.sbss.s.o(.sbss.*);
        BUILD_DIR/src/trn/trn_mp.c.o(.sbss.*);
        BUILD_DIR/generated/unk_0x8013E62E.sbss.s.o(.sbss.*);
        BUILD_DIR/src/trn2/trn2_hp_map99.c.o(.sbss.*);
        BUILD_DIR/src/trn2/trn2_def_map99.c.o(.sbss.*);
        BUILD_DIR/generated/unk_0x8013E676.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        main_SBSS_END = .;
        main_SBSS_SIZE = ABSOLUTE(main_SBSS_END - main_SBSS_START);
        main_BSS_START = .;
    }
    __romPos += SIZEOF(.sbss);
    __romPos = ALIGN(__romPos, 4);
    . = ALIGN(., 4);

    bss_ROM_START = __romPos;
    .bss main_BSS_START : AT(bss_ROM_START)
    {
        BUILD_DIR/generated/unk_0x8013E6A8.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/world_object.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/unk_0x80140CF4.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/file_queue.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/unk_0x80168460.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/butterfly.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/unk_0x80168920.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        main_BSS_END = .;
        main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);
    }
    __romPos += SIZEOF(.bss);
    __romPos = ALIGN(__romPos, 4);

    /DISCARD/ :
    {
        *(*);
    }
}
