        name long
var     equ $5050
other   equ %01011
        org 680

start:  ldaa %10010 ; comment
        stab $20
sub     subd $06
        suba @25
        cmpa 12

 ; loop
loop    bcc $3     ; loop
        ldaa 'E
        ror $12,Y
        lsr $2020
    ; some instructions to check the line wrap
        lsr $2020
        lsr var
        lsr $2020
        lsr $2020
        bcc loop
        lsr $2020
        lsr $2020
        lsr $2020
        beq sub
        lsr $2020
        lsr $2020
        lsr $2020
        lsr other
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020

        end
