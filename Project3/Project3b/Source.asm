.CODE

;sub rsp, 100h

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