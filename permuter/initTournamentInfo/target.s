.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

/* Handwritten function */
nonmatching initTournamentInfo, 0x1AC

glabel initTournamentInfo
    /* 5D0 80080DD0 C8FFBD27 */  addiu      $sp, $sp, -0x38
    /* 5D4 80080DD4 2000BFAF */  sw         $ra, 0x20($sp)
    /* 5D8 80080DD8 1C00B1AF */  sw         $s1, 0x1C($sp)
    /* 5DC 80080DDC 1800B0AF */  sw         $s0, 0x18($sp)
    /* 5E0 80080DE0 0C008010 */  beqz       $a0, .L80080E14
    /* 5E4 80080DE4 849884AF */   sw        $a0, %gp_rel(MAIN_D_801353B0)($gp)
    /* 5E8 80080DE8 E1001124 */  addiu      $s1, $zero, 0xE1
    /* 5EC 80080DEC B818040C */  jal        readPStat
    /* 5F0 80080DF0 FE000424 */   addiu     $a0, $zero, 0xFE
    /* 5F4 80080DF4 21204000 */  addu       $a0, $v0, $zero
    /* 5F8 80080DF8 1E04040C */  jal        setupBoxOrigin
    /* 5FC 80080DFC 3000A527 */   addiu     $a1, $sp, 0x30
    /* 600 80080E00 B1FF0224 */  addiu      $v0, $zero, -0x4F
    /* 604 80080E04 001C0200 */  sll        $v1, $v0, 16
    /* 608 80080E08 031C0300 */  sra        $v1, $v1, 16
    /* 60C 80080E0C 25000010 */  b          .L80080EA4
    /* 610 80080E10 08001024 */   addiu     $s0, $zero, 0x8
  .L80080E14:
    /* 614 80080E14 1480013C */  lui        $at, %hi(UI_BOX_DATA + 0x50)
    /* 618 80080E18 E0D32284 */  lh         $v0, %lo(UI_BOX_DATA + 0x50)($at)
    /* 61C 80080E1C 80988393 */  lbu        $v1, %gp_rel(TOURNAMENT_SELECTED_COLUMN)($gp)
    /* 620 80080E20 04004220 */  addi       $v0, $v0, 0x4 /* handwritten instruction */
    /* 624 80080E24 00240200 */  sll        $a0, $v0, 16
    /* 628 80080E28 1480013C */  lui        $at, %hi(UI_BOX_DATA + 0x52)
    /* 62C 80080E2C E2D32284 */  lh         $v0, %lo(UI_BOX_DATA + 0x52)($at)
    /* 630 80080E30 03240400 */  sra        $a0, $a0, 16
    /* 634 80080E34 03004220 */  addi       $v0, $v0, 0x3 /* handwritten instruction */
    /* 638 80080E38 002C0200 */  sll        $a1, $v0, 16
    /* 63C 80080E3C 00110300 */  sll        $v0, $v1, 4
    /* 640 80080E40 20184300 */  add        $v1, $v0, $v1 /* handwritten instruction */
    /* 644 80080E44 80100300 */  sll        $v0, $v1, 2
    /* 648 80080E48 22104300 */  sub        $v0, $v0, $v1 /* handwritten instruction */
    /* 64C 80080E4C 20108200 */  add        $v0, $a0, $v0 /* handwritten instruction */
    /* 650 80080E50 03004220 */  addi       $v0, $v0, 0x3 /* handwritten instruction */
    /* 654 80080E54 00240200 */  sll        $a0, $v0, 16
    /* 658 80080E58 81988293 */  lbu        $v0, %gp_rel(TOURNAMENT_SELECTED_ROW)($gp)
    /* 65C 80080E5C 03240400 */  sra        $a0, $a0, 16
    /* 660 80080E60 032C0500 */  sra        $a1, $a1, 16
    /* 664 80080E64 00110200 */  sll        $v0, $v0, 4
    /* 668 80080E68 2010A200 */  add        $v0, $a1, $v0 /* handwritten instruction */
    /* 66C 80080E6C 16004220 */  addi       $v0, $v0, 0x16 /* handwritten instruction */
    /* 670 80080E70 002C0200 */  sll        $a1, $v0, 16
    /* 674 80080E74 032C0500 */  sra        $a1, $a1, 16
    /* 678 80080E78 3000A4A7 */  sh         $a0, 0x30($sp)
    /* 67C 80080E7C 3200A5A7 */  sh         $a1, 0x32($sp)
    /* 680 80080E80 2A000224 */  addiu      $v0, $zero, 0x2A
    /* 684 80080E84 3400A2A7 */  sh         $v0, 0x34($sp)
    /* 688 80080E88 0D000224 */  addiu      $v0, $zero, 0xD
    /* 68C 80080E8C 3600A2A7 */  sh         $v0, 0x36($sp)
    /* 690 80080E90 CFFF0224 */  addiu      $v0, $zero, -0x31
    /* 694 80080E94 001C0200 */  sll        $v1, $v0, 16
    /* 698 80080E98 C1001124 */  addiu      $s1, $zero, 0xC1
    /* 69C 80080E9C 031C0300 */  sra        $v1, $v1, 16
    /* 6A0 80080EA0 21800000 */  addu       $s0, $zero, $zero
  .L80080EA4:
    /* 6A4 80080EA4 82FF0224 */  addiu      $v0, $zero, -0x7E
    /* 6A8 80080EA8 2800A2A7 */  sh         $v0, 0x28($sp)
    /* 6AC 80080EAC 2A00A3A7 */  sh         $v1, 0x2A($sp)
    /* 6B0 80080EB0 FC000224 */  addiu      $v0, $zero, 0xFC
    /* 6B4 80080EB4 2C00A2A7 */  sh         $v0, 0x2C($sp)
    /* 6B8 80080EB8 63000224 */  addiu      $v0, $zero, 0x63
    /* 6BC 80080EBC 2E00A2A7 */  sh         $v0, 0x2E($sp)
    /* 6C0 80080EC0 0880023C */  lui        $v0, %hi(tickTournamentInfo)
    /* 6C4 80080EC4 28194224 */  addiu      $v0, $v0, %lo(tickTournamentInfo)
    /* 6C8 80080EC8 1000A2AF */  sw         $v0, 0x10($sp)
    /* 6CC 80080ECC 0880023C */  lui        $v0, %hi(renderTournamentInfo)
    /* 6D0 80080ED0 A4194224 */  addiu      $v0, $v0, %lo(renderTournamentInfo)
    /* 6D4 80080ED4 1400A2AF */  sw         $v0, 0x14($sp)
    /* 6D8 80080ED8 03000424 */  addiu      $a0, $zero, 0x3
    /* 6DC 80080EDC 21282002 */  addu       $a1, $s1, $zero
    /* 6E0 80080EE0 2800A627 */  addiu      $a2, $sp, 0x28
    /* 6E4 80080EE4 C001040C */  jal        createTextbox
    /* 6E8 80080EE8 3000A727 */   addiu     $a3, $sp, 0x30
    /* 6EC 80080EEC 1000A0AF */  sw         $zero, 0x10($sp)
    /* 6F0 80080EF0 03000424 */  addiu      $a0, $zero, 0x3
    /* 6F4 80080EF4 21280002 */  addu       $a1, $s0, $zero
    /* 6F8 80080EF8 07000624 */  addiu      $a2, $zero, 0x7
    /* 6FC 80080EFC 0E02040C */  jal        registerTextbox
    /* 700 80080F00 21380000 */   addu      $a3, $zero, $zero
    /* 704 80080F04 80988393 */  lbu        $v1, %gp_rel(TOURNAMENT_SELECTED_COLUMN)($gp)
    /* 708 80080F08 B094908F */  lw         $s0, %gp_rel(MAIN_D_80134FDC)($gp)
    /* 70C 80080F0C 40100300 */  sll        $v0, $v1, 1
    /* 710 80080F10 20104300 */  add        $v0, $v0, $v1 /* handwritten instruction */
    /* 714 80080F14 40200200 */  sll        $a0, $v0, 1
    /* 718 80080F18 7C98838F */  lw         $v1, %gp_rel(TOURNAMENT_ARRAY)($gp)
    /* 71C 80080F1C 81988293 */  lbu        $v0, %gp_rel(TOURNAMENT_SELECTED_ROW)($gp)
    /* 720 80080F20 21186400 */  addu       $v1, $v1, $a0
    /* 724 80080F24 21106200 */  addu       $v0, $v1, $v0
    /* 728 80080F28 00004590 */  lbu        $a1, 0x0($v0)
    /* 72C 80080F2C 0A000424 */  addiu      $a0, $zero, 0xA
    /* 730 80080F30 3F00A230 */  andi       $v0, $a1, 0x3F
    /* 734 80080F34 D49B030C */  jal        getCupDataJumpTable
    /* 738 80080F38 FF004530 */   andi      $a1, $v0, 0xFF
    /* 73C 80080F3C 21204000 */  addu       $a0, $v0, $zero
    /* 740 80080F40 E79B030C */  jal        getCupDataJumpTableEntry
    /* 744 80080F44 21280000 */   addu      $a1, $zero, $zero
    /* 748 80080F48 02004224 */  addiu      $v0, $v0, 0x2
    /* 74C 80080F4C B09482AF */  sw         $v0, %gp_rel(MAIN_D_80134FDC)($gp)
    /* 750 80080F50 03000424 */  addiu      $a0, $zero, 0x3
    /* 754 80080F54 BE07040C */  jal        MAIN_func_80101EF8
    /* 758 80080F58 FF000524 */   addiu     $a1, $zero, 0xFF
    /* 75C 80080F5C 64000224 */  addiu      $v0, $zero, 0x64
    /* 760 80080F60 BC9482A3 */  sb         $v0, %gp_rel(ACTIVE_INSTRUCTION)($gp)
    /* 764 80080F64 B09490AF */  sw         $s0, %gp_rel(MAIN_D_80134FDC)($gp)
    /* 768 80080F68 2000BF8F */  lw         $ra, 0x20($sp)
    /* 76C 80080F6C 1C00B18F */  lw         $s1, 0x1C($sp)
    /* 770 80080F70 1800B08F */  lw         $s0, 0x18($sp)
    /* 774 80080F74 0800E003 */  jr         $ra
    /* 778 80080F78 3800BD27 */   addiu     $sp, $sp, 0x38
endlabel initTournamentInfo
