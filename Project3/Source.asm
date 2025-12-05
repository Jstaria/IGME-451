
; Joseph Staria - Project 3a (Simple Variables and Math)

INCLUDELIB kernel32.lib
ExitProcess PROTO

.DATA
num QWORD 100

.CODE
; Assign stack allocation
; sub rsp, 40h ; 64 Bytes allocated to stack

main PROC
	XOR RCX, RCX ; = 0
	XOR RDX, RDX ; = 0
	MOV RCX, 33  ; = 33
	MOV RDX, RCX ; = 33
	MOV RCX, num ; = 100
	MOV num, RDX ; = 33

	constantNum EQU 12

	MOV RCX, constantNum
	MOV RDX, constantNum + 8 ; 12 + 8 = 20
	MOV RCX, constantNum + 8 * 2 ; 12 + 8 * 2 = 28
	MOV RDX, (constantNum + 8) * 2 ; (12 + 8) * 2 = 40
	MOV RCX, constantNum MOD 5 ; 12 % 5 = 2
	MOV RDX, (constantNum - 3) / 3 ; (12 - 3) / 3 = 3

	XCHG RDX, RCX ; 3, 2 => 2, 3

	XOR RDX, RDX ; 0 out register
	XOR RCX, RCX ; 0 out register

; Exit Process seems to work on Windows10/Home PC
; and not Windows11/Lab PC
CALL ExitProcess
main ENDP

END