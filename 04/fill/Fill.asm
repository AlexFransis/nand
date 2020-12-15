@state
M=0 // 0 == white; 1 == black

(KBDLOOP)
@8191 // 256 x 32 - 1; row * col
M=0 // reset word pointer

@24576 // kdb address
D=M
@SHOULDFILLWHITE
D;JEQ
@SHOULDFILLBLACK
D;JNE

(SHOULDFILLBLACK)
@state
D=M-1
@KBDLOOP 
D;JEQ // state == black ? goto kbdloop : fill black
@FILLBLACK
0;JMP

(SHOULDFILLWHITE)
@state
D=M
@KBDLOOP
D;JEQ // state == white ? goto kbdloop : fill white
@FILLWHITE
0;JMP

(FILLBLACK)
@8191
D=M
@SCREEN
A=A+D
M=!M
@8191
MD=M+1
D=D-A // curr word - total words < 0 ? goto loop : continue
@FILLBLACK
D;JLE
@state
M=1 // black
@KBDLOOP
0;JMP

(FILLWHITE)
@8191
D=M
@SCREEN
A=A+D
M=0
@8191
MD=M+1
D=D-A // curr word - total words < 0 ? goto loop : continue
@FILLWHITE
D;JLE
@state
M=0 // white
@KBDLOOP
0;JMP
