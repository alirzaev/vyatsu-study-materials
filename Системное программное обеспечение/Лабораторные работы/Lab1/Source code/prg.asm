; Рзаев Али, ИВТ-32
;
; Ввести строку и символ с клавиатуры. Проверить, есть ли такой
; символ в строке. Если такой символ встречается не один раз,
; поместить их все в начало строки
;

.model small
.data
	message0 db  'String: $'
	message1 db  0Ah, 0Dh, 'Symbol: $'
	message2 db  0Ah, 0Dh, 'Result: $'
	FullData STRUC
		one db 11 dup(36) ; name type count init
		two db 11 dup(36)
		chr db 1  dup(36)
	FullData ENDS
	FD FullData 1 dup(<>)
.stack 256h
.code

GETSTRING proc near ; string will be written to [bx]
	push cx
	push dx

	mov cx, 0Ah ; string length
	mov dx, 01h ; break after Enter

getstring_loop:
	call GETCHAR ; get next char
	cmp byte ptr [bx], 0Dh ; test if char is Enter
	je getstring_quit
	inc bx
	loop getstring_loop

getstring_quit:
	pop dx
	pop cx
	ret
GETSTRING endp

GETCHAR proc near ; char will be written to [bx]
	push ax
	push dx

getchar_begin:
	mov ah, 08h
	int 21h     ; read char
	cmp al, 00h ; test if char is functional
	jne getchar_obtain_char

	mov ah, 08h
	int 21h
	jmp getchar_begin

getchar_obtain_char:
	cmp dx, 01h
	jne getchar_test_control
	cmp al, 0Dh ; test if char is Enter
	je getchar_write_char

getchar_test_control:
	cmp al, 31 ; test if char is control
	jle getchar_begin

getchar_write_char:
	mov [bx], al
	mov dl, al ; write symbol to stdout
	mov ah, 02h
	int 21h

	pop dx
	pop ax
	ret
GETCHAR endp

MESSAGE proc near ; data located in dx
	push ax
	push cx

	mov ah, 09h ; write string to stdout
	int 21h

	pop cx
	pop ax
	ret
MESSAGE endp

main:
	mov ax, @data
	mov ds, ax    ; load data segment

	lea dx, message0
	call MESSAGE

	lea bx, FD.one
	call GETSTRING

	lea dx, message1
	call MESSAGE

	lea bx, FD.chr
	xor dx, dx
	call GETCHAR

	mov al, FD.chr
	xor dx, dx ; char counter
	mov cx, 0Ah; loop counter
	lea bx, FD.one

; count of char FD.chr in FD.one
main_loop1_begin:
	cmp [bx], al
	jne main_loop1_end
	inc dx
main_loop1_end:
	inc bx
	loop main_loop1_begin

	lea di, FD.one
	cmp dx, 01h            ; if count FD.chr is not greater then
	jle main_print_string1 ; write original string to stdout

; copy dx chars to begin of the string
	mov cx, dx
	lea di, FD.two
main_loop2_begin:
	mov [di], al
	inc di
main_loop2_end:
	loop main_loop2_begin

; copy other chars
	mov cx, 0Ah
	lea si, FD.one
	lea di, FD.two
	add di, dx
main_loop3_begin:
	cmp byte ptr [si], al
	je main_loop3_end
	mov ah, [si]
	mov byte ptr [di], ah
	inc di
main_loop3_end:
	inc si
	loop main_loop3_begin

	lea di, FD.two
main_print_string1:
	lea dx, message2
	call MESSAGE
	mov byte ptr [di + 10], 24h
	mov dx, di
	call MESSAGE

	mov ah, 4Ch
	int 21h
end main