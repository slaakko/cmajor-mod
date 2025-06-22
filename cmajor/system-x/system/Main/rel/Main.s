        .LINK

        EXTERN   Main

        .CODE

Main FUNC
        STO      fp,sp
        SET      fp,sp
        INCL     sp,#0020
@0      LDOU     $0,fp,#08
        LDOU     $1,fp,#10
        LDOU     $2,fp,#18
        SET      p0,$0
        SET      p1,$1
        SET      p2,$2
        CALL     1,main
        SET      p0,ax
        CALL     1,exit
        SET      sp,fp
        LDO      fp,sp
        RET     
Main ENDF
