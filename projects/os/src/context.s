.text
.thumb
.syntax unified

.equ THREAD_CUR, 0
.equ THREAD_CNT, 4
.equ THREAD_SPS, 8

.globl yield
yield:
	STMFD SP!, { R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, LR }

	LDR   R0, =threads
	LDR   R1, [R0, #THREAD_CUR]
	LDR   R2, [R0, #THREAD_CNT]
	ADD   R3, R0, #THREAD_SPS
	LSL   R4, R1, #2
	STR   SP, [R3, R4]

	SUBS  R1, #1
	IT MI
	SUBMI R1, R2, #1
	STR   R1, [R0, #THREAD_CUR]

	LSL   R4, R1, #2
	LDR   SP, [R3, R4]

	LDMFD SP!, { R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, LR }
	BX    LR

.globl getsp
getsp:
	MOV R0, SP
	BX LR
.globl setsp
setsp:
	MOV SP, R0
	BX LR
