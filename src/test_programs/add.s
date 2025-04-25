RUNTIME:
    MOV R6, =stack_begin
    LDR R6, [R6]
    MOV R0, #10
    BL add_n
    HALT

add_n:
    MOV	R4, #-3
    ADD	R6, R4, R6
    STR	R0, [R6, #2] // Parameter
    MOV	R0, #0
    STR	R0, [R6, #1] // result?
    STR	R0, [R6, #0] // i
    B	.LBB0_1
.LBB0_1:
    LDR	R1, [R6, #0] // R1 = i
    LDR	R0, [R6, #2] // R0 = parameter
    CMP	R0, R1
    BLE	.LBB0_4      // i < parameter
    B	.LBB0_2
.LBB0_2:
    LDR	R0, [R6, #0] // R0 = 0
    LDR	R1, [R6, #1] // R1 = result
    ADD	R0, R1, R0   // result +=  result
    STR	R0, [R6, #1]
    B	.LBB0_3
.LBB0_3:
    LDR	R0, [R6, #0]
    MOV	R4, #1
    ADD	R0, R4, R0  // i +=1?
    STR	R0, [R6, #0]
    B	.LBB0_1
.LBB0_4:
    LDR	R0, [R6, #1]
    MOV	R4, #3
    ADD	R6, R4, R6
    BX	R7

stack_begin:
    .word 0xff
