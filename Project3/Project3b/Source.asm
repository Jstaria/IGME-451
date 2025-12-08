.DATA

nums0 REAL4 1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0
nums1 REAL4 65.0,85.0,45.0,96.0,74.0,2.0,53.0,12.0


.CODE

;sub rsp, 100h



FillArray Proc ; Does some math on two arrays
	
MOVAPS XMM0, nums0
MOVAPS XMM1, nums1 

DIVPS XMM0, XMM1
	
LEA RAX, nums0

RET

FillArray ENDP

DoSum Proc ; Adds two numbers
	MOV RAX, RCX
	ADD RAX, RDX
	RET
DoSum ENDP


DoSub Proc ; Subtracts 2nd number from 1st
	MOV RAX, RCX
	SUB RAX, RDX
	RET
DoSub ENDP

DoFac Proc ; Returns factorial based on input number
	MOV RAX, RCX

	LOOP_START:
		DEC RCX
		MUL RCX
		CMP RCX, 1
		JA LOOP_START
	DONE:
		RET
DoFac ENDP


END