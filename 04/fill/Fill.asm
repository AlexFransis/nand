@state
M=0 // x0000 == white; xFFFF == black

(KBDLOOP)
@8191 // 256 x 32 - 1; row * col
M=0 // reset word pointer

@KBD
D=M
@state
D=D|M
@KBDLOOP 
D;JEQ

@KBD
D=M
@state
D=D&M
@KBDLOOP 
D;JNE

	(FILL)
	@8191
	D=M
	@SCREEN
	A=A+D
	M=!M
	@8191
	MD=M+1
	D=D-A
	@FILL // curr word - total words < 0 ? goto fill : goto kbdloop
	D;JLE
	@state
	M=!M // toggle state
	@KBDLOOP
	0;JMP
