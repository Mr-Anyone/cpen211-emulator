RUN_TIME:
    MOV R6, stack_begin
    LDR R6, [R6]
    MOV R0, #10 ; calculate fib(10)
    BL fib
    HALT
fib:                                    
	MOV	R4, #-5
	ADD	R6, R4, R6
	STR	R7, [R6, #3]                    
	STR	R3, [R6, #4]                    
	STR	R0, [R6, #1]
	LDR	R0, [R6, #1]
	MOV	R4, #0
	CMP	R0, R4
	BEQ	.LBB0_2
	B	.LBB0_1
.LBB0_1:                                
	LDR	R0, [R6, #1]
	MOV	R4, #1
	CMP	R0, R4
	BNE	.LBB0_3
	B	.LBB0_2
.LBB0_2:                                
	LDR	R0, [R6, #1]
	STR	R0, [R6, #2]
	B	.LBB0_4
.LBB0_3:                                
	LDR	R0, [R6, #1]
	MOV	R4, #-1
	ADD	R0, R4, R0
	BL	fib
	STR	R0, [R6, #0]                    
	LDR	R0, [R6, #1]
	MOV	R4, #-2
	ADD	R0, R4, R0
	BL	fib
	LDR	R1, [R6, #0]                    
	ADD	R0, R1, R0
	STR	R0, [R6, #2]
	B	.LBB0_4
.LBB0_4:                                
	LDR	R0, [R6, #2]
	LDR	R7, [R6, #3]                    
	LDR	R3, [R6, #4]                    
	MOV	R4, #5
	ADD	R6, R4, R6
	BX	R7

stack_begin:
    .word 0xFF
