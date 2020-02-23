; Рзаев Али, ИВТ-42
;
; Ввести фамилию и имя посередине экрана в рамке с тенью
;

.286
.model small
.data
	studentName db 'Ali Rzaev', 0
	groupName db "IVT-42", 0
.stack 256h
.code

;процедура заливки окна
fill proc near
	mov bp, sp
	pusha

	mov ax, 0B800h
	mov es, ax
	mov di, 0

             ;цикл заливки целого экрана заданным цветом
	mov ax, [bp+2]
	mov cx, 2000
	l:
		add di, 2
		mov es:[di], ax
		loop l
	popa
	ret 2
fill endp

; Процедура рисования рамки
drawBorders proc near
	mov bp, sp
	pusha

	mov ax, 0B800h
	mov es, ax
	mov di, 1010

	;Тень
	mov ax, 0h
	mov bx, 0
	shadowLoop:
		mov cx, 40
		shadowFillLoop: ; цикл заливки строки
			add di, 2
			mov es:[di], ax
			loop shadowFillLoop
		inc bx
		cmp bx, 10
		jg shadowExit
		add di, 80
		jmp shadowLoop
	shadowExit:

	mov di, 680
	mov ah, 00010000b

	;Заливка фона
	rectLoop:
		mov cx, 40
		fillLoop:
			add di, 2
			mov es:[di], ax
			loop fillLoop
		cmp di, 2280
		jg rectExit
		add di, 80
		jmp rectLoop
	rectExit:

	mov di, 680

	mov ah, 00011111b ; параметры рамки
	mov al, 0C9h
	mov es:[di], ax ; Левый верхний угол

	mov al, 0CDh
	mov cx, 39
	topLineLoop: ; цикл рисования верхней части рамки
		add di, 2
		mov es:[di], ax
		loop topLineLoop

	add di, 2
	mov al, 187
	mov es:[di], ax

	mov al, 186
	mov cx, 20
	rightLineLoop: ; цикл рисования боковых линий рамки
		add di, 80
		mov es:[di], ax
		loop rightLineLoop

	add di, 160
	mov al, 188
	mov es:[di], ax

	mov al, 205
	mov cx, 39
	bottomLineLoop: ; цикл рисования нижней части рамки
		sub di, 2
		mov es:[di], ax
		loop bottomLineLoop

	sub di, 2
	mov al, 200
	mov es:[di], ax

	popa
	ret
drawBorders endp

;Процедура вывода текста на экран
drawText proc near
	mov bp, sp
	pusha

	mov ax, 0B800h
	mov es, ax
	mov ax, [bp+6] ; получаем базовый цвет и фон
	mov bx, [bp+2] ; адрес первого символа строки
	;add bx, 1E0h
	mov di, [bp+4] ; координата начала

	textLoop: ; цикл рисования букв
		mov cx, 7
		colorLoop: ; цикл отсчета цетов, после 10 цветов сброс на начальный
			mov al, [bx]
			test al, al
			jz textExit ; если дошли до конца строки, то выход
			inc bx ; переход к след. символу
			;inc ah ; выбор нового цвета
			add ah, 11h
			mov es:[di], ax
			add di, 2
			loop colorLoop
		mov ax, [bp+6]
		jmp textLoop
	textExit:

	popa
	ret 6
drawText endp

main:
	mov ax, @data
	mov ds, ax

	;Переход в текстовый режим
	mov al, 01h
	mov ah, 00h
	int 10h

	;Заливка цветом и рисование рамки
	push 2000h ; green
	call fill
	call drawBorders

	; Вывод имени
	push 7800h ; базовый цвет
	push 1352 ; позиция
	lea bx, studentName
	push bx ; строка
	call drawText

	;Вывод группы
	push 7800h
	push 1514
	lea bx, groupName
	push bx
	call drawText

	;Ожидание клавиши
	mov ah,00h
	mov al,13h
	int 16h

        ;Переход в текстовый режим
	mov al, 01h
	mov ah, 00h
	int 10h

	pop es

	;Выход из программы
	mov ax, 4c00h
	int 21h
end main