@sum
M=0

(LOOP)
	@sum
	D=M
	@R2
	M=D
	@R1
	D=M
	@END
	D;JEQ // if mul == 0 goto end
	@R0
	D=M
	@sum
	M=M+D
	@R1
	M=M-1
	@LOOP
	0;JMP
(END)
	@END
	0;JMP
	
