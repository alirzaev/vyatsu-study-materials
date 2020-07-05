.MODEL FLAT, C

.CODE

pmult proc
	push ebp
	mov ebp, esp

	push ebx
	push ecx
	push edx
	push edi
	
	; edi <- n1 + n2
	mov ecx, [ebp + 12]
	add ecx, [ebp + 20]
	mov edi, ecx

	; n1 <- n1 * 4 + f
	mov ecx, [ebp + 12]
	imul ecx, ecx, 4
	add ecx, [ebp + 8]
	mov [ebp + 12], ecx

	; n2 <- n2 * 4 + s
	mov ecx, [ebp + 20]
	imul ecx, ecx, 4
	add ecx, [ebp + 16]
	mov [ebp + 20], ecx

	; set zeros
	mov ecx, edi
	mov edx, [ebp + 24]
	setzero:
		mov dword ptr [edx + 4 * ecx - 4], 0
	loop setzero

	;multiply
	mov eax, [ebp + 8]	; eax <- i
	for1_begin:
		cmp eax, [ebp + 12]; i != i + n1 ?
		je for1_end

		mov ebx, [ebp + 16]
		for2_begin:
			cmp ebx, [ebp + 20]; j != j + n2 ?
			je for2_end

			; ecx <- i * 4 + j * 4 + r
			mov ecx, eax
			add ecx, ebx
			sub ecx, [ebp + 8]
			sub ecx, [ebp + 16]
			add ecx, [ebp + 24]

			; edx <- f[i] * s[j]
			; r[i + j] += edx
			mov edx, [eax]
			imul edx, [ebx]
			add dword ptr [ecx], edx

			; j++
			add ebx, 4
			jmp for2_begin
		for2_end:
			; i++
			add eax, 4
			jmp for1_begin
	for1_end:


	pop edi
	pop edx
	pop ecx
	pop ebx

	pop ebp

	ret
pmult endp

END