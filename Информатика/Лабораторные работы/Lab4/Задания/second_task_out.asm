; ввод 1 числа
in [1]
in [2]
in [3]
; [0] - для сдвигов
; ввод 2 числа
in [5]
in [6]
in [7]

; СЧП [8]..[13]

sub 0 0 r0 ; r0 <- 0 - счетчик сложений

label_loop1_begin:
    ; копирование 2 числа
	sub 0 0 [15]
	sub 0 0 [16]
	sub 0 0 [17]
	sub [5] 0 [18]
	sub [6] 0 [19]
	sub [7] 0 [20]
	
	;and [5] 0x80 r2
	nand [5] 0x80 r2
	nand r2 r2 r2
	jz r2 label_skip_full2compl; число > 0?
	;число < 0
	;заполнить единицами старше байты
	sub 0xFF 0 [15]
	sub 0xFF 0 [16]
	sub 0xFF 0 [17]
	
label_skip_full2compl:
	
	;ror [1] 1 r2;
	;and r2 0x7F r2
	;ror r2 6 r2 ; старшие биты
	ror [1] 6 r2 ; старшие биты
	
	;and r2 0b11 r2 
	nand r2 0b11 r2
	nand r2 r2 r2
	sub r2 0b00 r3 ; r3 <- 0
	jz r3 label_step ; r2 == 0?
	sub r2 0b01 r3
	jz r3 label_add ; r2 == 1?
	sub r2 0b11 r3
	jz r3 label_step ; r2 == 3?
	; r2 == 2
	; ~b
	;not [15] [15]
	nand [15] [15] [15]
	;not [16] [16]
	nand [16] [16] [16]
	;not [17] [17]
	nand [17] [17] [17]
	;not [18] [18]
	nand [18] [18] [18]
	;not [19] [19]
	nand [19] [19] [19]
	;not [20] [20]
	nand [20] [20] [20]
	
	; +1
	;iadd [20] 1 0 [21] [22] ; [21] <- [20] + 1, [22] <- c
	nand 1 1 r7
	sub r7 0xFF r7
	sub [20] r7 [21]
	nand 0 0 r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [20] [20] r7
	nand 1 1 [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [20] 1 [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [20]         ; [20] <- [20] + 1
	;iadd [19] 0 [22] [21] [22]
	nand 0 0 r7
	sub r7 0xFF r7
	sub [19] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [19] [19] r7
	nand 0 0 [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [19] 0 [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [19]         ; [19] <- [19] + c
	;iadd [18] 0 [22] [21] [22]
	nand 0 0 r7
	sub r7 0xFF r7
	sub [18] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [18] [18] r7
	nand 0 0 [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [18] 0 [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [18]         ; [18] <- [18] + c
	;iadd [17] 0 [22] [21] [22]
	nand 0 0 r7
	sub r7 0xFF r7
	sub [17] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [17] [17] r7
	nand 0 0 [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [17] 0 [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [17]         ; [17] <- [17] + c
	;iadd [16] 0 [22] [21] [22]
	nand 0 0 r7
	sub r7 0xFF r7
	sub [16] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [16] [16] r7
	nand 0 0 [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [16] 0 [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [16]         ; [16] <- [16] + c
	;add [15] [22] [15]      ; [15] <- [15] + c
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [15] r7 [15]
	
label_add:	

	;iadd [13] [20] 0 [21] [22]
	nand [20] [20] r7
	sub r7 0xFF r7
	sub [13] r7 [21]
	nand 0 0 r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [13] [13] r7
	nand [20] [20] [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [13] [20] [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [13]
	;iadd [12] [19] [22] [21] [22]
	nand [19] [19] r7
	sub r7 0xFF r7
	sub [12] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [12] [12] r7
	nand [19] [19] [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [12] [19] [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [12]
	;iadd [11] [18] [22] [21] [22]
	nand [18] [18] r7
	sub r7 0xFF r7
	sub [11] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [11] [11] r7
	nand [18] [18] [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [11] [18] [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [11]
	;iadd [10] [17] [22] [21] [22]
	nand [17] [17] r7
	sub r7 0xFF r7
	sub [10] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [10] [10] r7
	nand [17] [17] [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [10] [17] [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [10]
	;iadd [9] [16] [22] [21] [22]
	nand [16] [16] r7
	sub r7 0xFF r7
	sub [9] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [9] [9] r7
	nand [16] [16] [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [9] [16] [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [9]
	;iadd [8] [15] [22] [21] [22]
	nand [15] [15] r7
	sub r7 0xFF r7
	sub [8] r7 [21]
	nand [22] [22] r7
	sub r7 0xFF r7
	sub [21] r7 [21]
	nand [21] [21] [255]
	nand [8] [8] r7
	nand [15] [15] [22]
	nand r7 [22] [22]
	nand [22] [255] [255]
	nand [255] [255] [255]
	nand [8] [15] [22]
	nand [22] [22] [22]
	nand [22] [22] r7
	nand [255] [255] [22]
	nand r7 [22] [22]
	nand [22] 0x80 [22]
	nand [22] [22] [22]
	ror [22] 7 [22]
	sub [21] 0 [8]

label_step:
    ;add r0 1 r0 ; r0 <- r0 + 1
    nand 1 1 r7
    sub r7 0xFF r7
    sub r0 r7 r0
    sub r0 24 r2 ; r2 <- r0 - 24
		
    jz r2 label_loop1_end ; конец, последний сдвиг не делаем
    ; сдвиг 1 числа
    ror [1] 7 [1] ; rol [1] 1 [1] 1 байт

    ror [2] 7 [2] ; rol [2] 1 [2] 2 байт
    ;and [2] 0x1 r2 ; старший бит 2 байта
    nand [2] 0x1 r2
    nand r2 r2 r2
    ;and [1] 0xFE [1] ; обнулить младший бит 1 байта
    nand [1] 0xFE [1]
    nand [1] [1] [1]
    ;or r2 [1] [1] ; в младший 1 байта
    nand r2 r2 r7
    nand [1] [1] [1]
    nand r7 [1] [1]

    ror [3] 7 [3] ; rol [3] 1 [3] 3 байт
    ;and [3] 0x1 r2 ; старший бит 3 байта
    nand [3] 0x1 r2
    nand r2 r2 r2
    ;and [2] 0xFE [2] ; обнулить младший бит 2 байта
    nand [2] 0xFE [2]
    nand [2] [2] [2]
    ;or r2 [2] [2] ; в младший бит 2 байта
    nand r2 r2 r7
    nand [2] [2] [2]
    nand r7 [2] [2]

    ;and [3] 0xFE [3] ; обнулить младший бит 3 байта
    nand [3] 0xFE [3]
    nand [3] [3] [3]
    
    ; сдвиг СЧП
    ror [8] 7 [8] ; rol [8] 1 [8]

    ror [9] 7 [9] ; rol [9] 1 [9]
    ;and [9] 0x1 r2 ; старший бит
    nand [9] 0x1 r2
    nand r2 r2 r2
    ;and [8] 0xFE [8] ; обнулить младший бит
    nand [8] 0xFE [8]
    nand [8] [8] [8]
    ;or r2 [8] [8]
    nand r2 r2 r7
    nand [8] [8] [8]
    nand r7 [8] [8]

    ror [10] 7 [10] ; rol [10] 1 [10]
    ;and [10] 0x1 r2 ; старший бит
    nand [10] 0x1 r2
    nand r2 r2 r2
    ;and [9] 0xFE [9] ; обнулить младший бит
    nand [9] 0xFE [9]
    nand [9] [9] [9]
    ;or r2 [9] [9]
    nand r2 r2 r7
    nand [9] [9] [9]
    nand r7 [9] [9]

    ror [11] 7 [11] ; rol [11] 1 [11]
    ;and [11] 0x1 r2 ; старший бит
    nand [11] 0x1 r2
    nand r2 r2 r2
    ;and [10] 0xFE [10] ; обнулить младший бит
    nand [10] 0xFE [10]
    nand [10] [10] [10]
    ;or r2 [10] [10]
    nand r2 r2 r7
    nand [10] [10] [10]
    nand r7 [10] [10]

    ror [12] 7 [12] ; rol [12] 1 [12]
    ;and [12] 0x1 r2 ; старший бит
    nand [12] 0x1 r2
    nand r2 r2 r2
    ;and [11] 0xFE [11] ; обнулить младший бит
    nand [11] 0xFE [11]
    nand [11] [11] [11]
    ;or r2 [11] [11]
    nand r2 r2 r7
    nand [11] [11] [11]
    nand r7 [11] [11]

    ror [13] 7 [13] ; rol [13] 7 [13]
    ;and [13] 0x1 r2 ; старший бит
    nand [13] 0x1 r2
    nand r2 r2 r2
    ;and [12] 0xFE [12] ; обнулить младший бит
    nand [12] 0xFE [12]
    nand [12] [12] [12]
    ;or r2 [12] [12]
    nand r2 r2 r7
    nand [12] [12] [12]
    nand r7 [12] [12]
    
    ;and [13] 0xFE [13] ; обнулить младший бит
    nand [13] 0xFE [13]
    nand [13] [13] [13]
    ; сдвиг завершен
    
    jz 0 label_loop1_begin
label_loop1_end:

; вывод результата
sub 8 0 r0
label_loop_pr_begin:
    sub r0 14 r2 ; r2 <- r0 - 14
    jz r2 label_loop_pr_end ; r0 == 14?
    out [r0]
    ;add r0 1 r0 ; r0 <- r0 + 1
    nand 1 1 r7
    sub r7 0xFF r7
    sub r0 r7 r0
    jz 0 label_loop_pr_begin
label_loop_pr_end:
