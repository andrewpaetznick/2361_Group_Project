.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _arp_delay_1ms, _arp_delay_100us

_arp_delay_1ms: ; 62.5*16000 = 1ms
    clr LATA
    repeat #15994
    nop
    return
    
_arp_delay_100us: ; 62.5ns*1600 = 100us
    repeat #1594
    nop
    return


