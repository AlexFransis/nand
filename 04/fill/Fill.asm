@SCREEN
M=!M
A=A+1
M=!M
A=A+1
M=!M
@SCREEN
M=!M

//@32
//@256
//M=A
//
//(LOOP1)
//	@256
//	MD=A-1
//	@END
//	D;JEQ
//	@32
//	M=A
//	(LOOP2)
//		@SCREEN
//		M=!M
//		@32
//		MD=M-1
//		@LOOP1
//		D;JEQ
//		@SCREEN
//		A=A+1
//		@LOOP2
//		0;JMP
//	
//(END)
//	@END
//	0;JMP
