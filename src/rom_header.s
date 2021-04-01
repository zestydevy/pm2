/*
 * ROM header
 * Only the first 0x18 bytes matter to the console.
 */

.byte  0x80, 0x37, 0x12, 0x40   /* PI BSD Domain 1 register */
.word  0x0000000F               /* Clockrate setting*/
.word  entry_point              /* Entrypoint */

/* SDK Revision */
.word  0x0000144B

.word  0x00000000               /* Checksum 1 (OVERWRITTEN BY MAKEMASK)*/
.word  0x00000000               /* Checksum 2 (OVERWRITTEN BY MAKEMASK)*/
.word  0x00000000               /* Unknown */
.word  0x00000000               /* Unknown */
.ascii "SUPER MARIO RPG 2   "   /* Internal ROM name (Max 20 characters) */
.word  0x00000000               /* Unknown */
/* Game ID (EXAMPLE: NSME) Begins here */
.word  0x0000004E                /* Cartridge Type (N)*/
.ascii "MQ"                     /* Cartridge ID (SM)*/
.ascii "E"			/* Region (E)*/
.byte  0x00			/* Version */
