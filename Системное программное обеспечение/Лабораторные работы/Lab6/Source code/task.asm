.386

.model flat

	extrn	MessageBoxA:DWORD
	extrn	ExitProcess:DWORD
	extrn	GetKeyState:DWORD ;0xC940
	;VK_CAPITAL 0x14
.code
_start:
	push	14h ; CapsLock key code
	call	GetKeyState
	and eax, 01h
	jz capslock_not_toggled

capslock_toggled:
	push	40h
	push	offset App
	push	offset Msg0
	push	0	
	call	MessageBoxA
	jmp prog_exit

capslock_not_toggled:
	push	40h
	push	offset App
	push	offset Msg1
	push	0	
	call	MessageBoxA

prog_exit:
	push	0
	call	ExitProcess

.data	
App	db	'CapsLock Test', 0
Msg0	db	'Success!!!', 0
Msg1	db	'No', 0

end	_start