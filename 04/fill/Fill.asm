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
D=M
@KBDLOOP 
D;JNE // state == black ? goto kbdloop : fill black
@FILL
0;JMP

(SHOULDFILLWHITE)
@state
D=M
@KBDLOOP
D;JEQ // state == white ? goto kbdloop : fill white
@FILL
0;JMP

	(FILL)
	@8191
	D=M
	@SCREEN
	A=A+D
	M=!M
	@8191
	MD=M+1
	D=D-A // curr word - total words < 0 ? goto loop : continue
	@FILL
	D;JLE
	@state
	M=!M // toggle state
	@KBDLOOP
	0;JMP
