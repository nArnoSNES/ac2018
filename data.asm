.include "hdr.asm"

.section ".rodata1" superfree

snesfont: .incbin "snesfont.pic" /* 3072 */

bg1_tiles:
.incbin "bg1.pic" /* 2752 */
bg1_tiles_end:

bg1_map:
.incbin "bg1.map" /* 1792 */
bg1_map_end:

bg1_pal:
.incbin "bg1.pal" /* 512 */
bg1_pal_end:

col1_map:
.incbin "coll1.clm" /* 1792 */
col1_map_end:

.ends

.section ".rodata2" superfree

sprites:
.incbin "sprites.pic" /* 10294 */
sprites_end:

sprites_pal:
.incbin "sprites.pal" /* 32 */
sprites_pal_end:

.ends
