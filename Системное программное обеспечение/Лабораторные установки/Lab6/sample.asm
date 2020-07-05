	.386
	
	.model flat
	
	extrn	MessageBoxA:DWORD
	extrn	ExitProcess:DWORD
	
	.code
_start:

	push	40h
	push	offset App
	push	offset Msg
	push	0	
	call	MessageBoxA
	push	0
	call	ExitProcess

	.data	
	
App	db	'PE Linker Test', 0
	
Msg	db	'Success!!!', 0
		
	end	_start