# This file is used by modern gas.
# Defines the expected assembly macros

# Evaluate this file only once in case it's included more than once
.ifndef _MACRO_INC_GUARD
.internal _MACRO_INC_GUARD
.set _MACRO_INC_GUARD, 1

# A function symbol.
.macro glabel label, visibility=global
    .section .text.\label
    .\visibility \label
    .type \label, @function
    \label:
        .ent \label
.endm

# The end of a function symbol.
.macro endlabel label
    .size \label, . - \label
    .end \label
.endm

# An alternative entry to a function.
.macro alabel label, visibility=global
    .\visibility \label
    .type \label, @function
    \label:
        .aent \label
.endm

# A label referenced by an error handler table.
.macro ehlabel label, visibility=global
    .\visibility \label
    \label:
.endm


# A label referenced by a jumptable.
# Local by default, so jumptable entries are relocated against the function's
# section like in compiled code. Define _GLOBAL_JLABELS when the jumptables are
# assembled from a separate file (PsyQ libraries).
.macro jlabel label, visibility=local
.ifdef _GLOBAL_JLABELS
    .global \label
.else
    .\visibility \label
.endif
    \label:
.endm


# A data symbol.
.macro dlabel label, visibility=global
    .pushsection %S.\label
    .\visibility \label
    .type \label, @object
    \label:
.endm

# End of a data symbol.
.macro enddlabel label
    .size \label, . - \label
    .popsection
.endm


# Label to signal the symbol haven't been matched yet.
.macro nonmatching label, size=0
    .global \label\().NON_MATCHING
    .type \label\().NON_MATCHING, @object
    .size \label\().NON_MATCHING, \size
    \label\().NON_MATCHING:
.endm

.include "gte_macros.inc"

.endif
