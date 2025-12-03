INCLUDELIB kernel32.lib
ExitProcess PROTO

.DATA
var QWORD 100

.CODE
; Assign stack allocation
; sub rsp, 28h ?
main PROC
	XOR RCX, RCX
	XOR RDX, RDX
	MOV RCX, 33
	MOV RDX, RCX
	MOV RCX, var
	MOV var, RDX

CALL ExitProcess
main ENDP

END