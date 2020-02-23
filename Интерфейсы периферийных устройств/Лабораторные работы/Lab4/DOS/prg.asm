.model small
.data
	str db 1Bh, 40h, 'Rzaev Ali IVT-42', 0Ah, 0Dh
	db 1Bh, 53h, 0, 'Up index', 1Bh, 54h, 0Ah, 0Dh
	db 1Bh, 53h, 1, 'Down index', 1Bh, 54h, 0Ah, 0Dh
	db 1Bh, 2Dh, 1, 'Underline', 1Bh, 2Dh, 0, 0Ah, 0Dh
	db 1Bh, 47h, 'Double print', 1Bh, 48h, 0Ah, 0Dh
	db 1Bh, 78h, 1, 'NLQ mode', 1Bh, 48h, 0Ah, 0Dh, 'NLQ off', 0Ah, 0Dh
	db 1Bh, 57h, 1, 'Double width', 1Bh, 57h, 0, 0Ah, 0Dh
	db 1Bh, 4Dh, '12 symbols per inch', 0Ah, 0Dh
	db 1Bh, 50h, '10 symbols per inch', 0Ah, 0Dh
	db 1Bh, 0Fh, 'Pressed text', 12h, 0Ah, 0Dh
	db 1Bh, 30h, '1/8 inch between lines', 0Ah, 0Dh
	db 1Bh, 30h, '1/8 inch between lines', 0Ah, 0Dh
	db 1Bh, 30h, '1/8 inch between lines', 0Ah, 0Dh
	db 1Bh, 31h, '1/10 inch between lines', 0Ah, 0Dh
	db 1Bh, 31h, '1/10 inch between lines', 0Ah, 0Dh
	db 1Bh, 31h, '1/10 inch between lines', 0Ah, 0Dh
	db 1Bh, 32h, '1/6 inch between lines', 0Ah, 0Dh
	db 1Bh, 32h, '1/6 inch between lines', 0Ah, 0Dh
	db 1Bh, 32h, '1/6 inch between lines', 0Ah, 0Dh
	db 1bh, 31h
	db 1bh, 2ah, 0, 32, 0
	db 0h, 0h, 3h, 6h, 0Ch, 8h, 18h, 10h, 10h, 10h, 10h, 18h, 8h, 0Ch, 6h, 3h
        db 1h, 3h, 6h, 0Ch, 8h, 18h, 10h, 10h, 10h, 10h, 18h, 8h, 0Ch, 6h, 3h, 0h
        db 0Ah, 0Dh
	db 1bh, 2ah, 0, 32, 0h
	db 0h, 0h, 0E0h, 30h, 18h, 8h, 12h, 4h, 4h, 4h, 4h, 12h, 8h, 18h, 30h, 0E0h
        db 0C0h, 0E0h, 30h, 18h, 8h, 12h, 4h, 4h, 4h, 4h, 12h, 8h, 18h, 30h, 0E0h, 0h
        db 0Ah, 0Dh
	db 1bh, 2ah, 0, 32, 0
	db 0h, 0h, 0h, 40h, 43h, 47h, 47h, 47h, 43h, 40h, 60h, 30h, 18h, 8h, 0Fh, 7h, 7h
        db 7h, 0Fh, 8h, 18h, 30h, 60h, 40h, 40h, 40h, 40h, 40h, 40h, 40h, 40h, 0h
        db 0Ah, 0Dh
	db 1bh, 2ah, 0, 32, 0
	db 0h, 0h, 0h, 4h, 84h, 0C4h, 0C4h, 0C4h, 84h, 4h, 0Ch, 18h, 30h, 20h, 0E0h, 0C0h, 0C0h
        db 0C0h, 0E0h, 20h, 30h, 18h, 0Ch, 4h, 4h, 4h, 4h, 4h, 4h, 4h, 4h, 0h
        db 0Ah, 0Dh
	;db 0h, 0h
	lengthString equ $ - str
.stack 256h
.code
MAIN:
	mov ax, @data
	mov ds, ax
	mov ah, 40h
	mov bx, 04h
	mov cx, lengthString
	lea dx, str
	int 21h
	mov ah, 00h
	int 16h
	mov ah, 4Ch
	int 21h
end MAIN
