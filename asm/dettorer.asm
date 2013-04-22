        name dettorer
        org 680

start:  ldaa %10010 ; beginning
        stab $20
sub     subd $06
        suba @25
        cmpa 12

 ; loop
loop:   bcc $3; lol loop
        ldaa #$20
        ror $12,Y
        lsr $2020
    ; some instructions to check the line wrap
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020
        lsr $2020

        end
