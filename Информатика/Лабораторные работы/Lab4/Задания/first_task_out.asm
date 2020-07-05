; in, out, ror, nand, sub, jz
;ввод числа (старшие->младшие)
in [0]
in [1]
in [2]
;конец ввода числа

sub 0 0 r1 ; счетчик байтов (0, 1, 2)
sub 0 0 r4 ; r4 <- 0

label_for1_begin:
sub 3 r1 r6 ; r6 <- 3 - r1
jz r6 label_for1_end
	; тело цикла (по байтам)
	sub 7 0 [3] ; номер бита в байте
	label_for2_begin:
		sub [r1] 0 r2 ; r2 <- [r1]
		ror r2 [3] r2 ; требуемый бит -> младший
		;and r2 0x1 r2
		nand r2 0x1 r2
		nand r2 r2 r2
		;add r4 r2 r4 ; r4 <- r4 + 1
		nand r2 r2 r7
		sub r7 0xFF r7
		sub r4 r7 r4
		sub [3] 1 [3] ; уменшить номер бита на 1
	nand [3] [3] r6
	jz r6 label_for2_end ; r6 == 255 (-1) ?
	jz 0 label_for2_begin
	label_for2_end:
	;add r1 0x1 r1 ; r1 <- r1 + 1
	nand 0x1 0x1 r7
	sub r7 0xFF r7
	sub r1 r7 r1
	jz 0 label_for1_begin
	; конец тела цикла
label_for1_end:

out r4
