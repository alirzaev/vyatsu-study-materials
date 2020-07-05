; Рзаев Али, ИВТ-32
;
; Прочитать содержимое корневого каталога и 
; вывести список файлов с атрибутами и датой создания
;

.model small
.data
        sbuf     db 512 dup (0)
        entr_cnt dw 0000h
        copy_cnt db 00h
        resr_cnt dw 0000h
        fat_size dw 0000h
        day      db  '  $'
        year     db  '    $'
        message0 db  'Ok$'
        space    db  ' $'
        newline  db 0Ah, 0Dh, '$'
        fileinfo db  '            $'
.stack 200h
.code

MESSAGE proc near ; data located in dx
        push ax
        push cx

        mov ah, 09h ; write string to stdout
        int 21h

        pop cx
        pop ax
        ret
MESSAGE endp

LOADSECTOR proc near ; load sector dx in [bx]
        push ax
        push cx

        mov al, 00h
        mov cx, 01h
        lea bx, sbuf
        int 25h
        pop cx

        pop cx
        pop ax
        ret
LOADSECTOR endp

WRITEINT proc near ; write digit in bx
        push ax
        push cx
        push dx

        mov ax, bx
        xor dx, dx
        mov cx, 0Ah
        div cx

        mov ah, 02h
        add dl, 30h
        int 21h

        pop dx
        pop cx
        pop ax
        ret
WRITEINT endp

WRITEFILEINFO proc near ; directory entry in [bx]
        push si
        push di
        push ax
        push cx
        push dx

        mov si, bx
        lea di, fileinfo
        mov cx, 0Ch

writefileinfo_clear_buffer:
        mov byte ptr [di], 20h
        inc di
        loop writefileinfo_clear_buffer

        lea di, fileinfo
        mov cx, 08h

writefileinfo_copy_name:
        mov dl, [si]  ; write file name
        mov [di], dl
        inc si
        inc di
        loop writefileinfo_copy_name
        
        mov cl, byte ptr [bx+0Bh]
        and cl, 10h
        jne writefileinfo_write  ;  do not write extension for dir

        mov byte ptr [di], 2Eh
        inc di
        mov cx, 03h

writefileinfo_copy_ext:
        mov dl, [si]  ; write file extension
        mov [di], dl
        inc si
        inc di
        loop writefileinfo_copy_ext

writefileinfo_write:
        lea dx, fileinfo
        call MESSAGE
        lea dx, space
        call MESSAGE

        mov al, byte ptr [bx+0Bh]
        mov cx, 06h
        push bx

writefileinfo_write_attrs:
        xor bx, bx 
        mov bl, al
        and bl, 01h
        call WRITEINT
        shr al, 01h
        loop writefileinfo_write_attrs

        pop bx
        lea dx, space
        call MESSAGE

writefileinfo_write_date:
        mov ax, word ptr [bx+18h]
        lea di, day
        inc di
        mov cx, 02h
        mov si, 0Ah
        and ax, 1Fh

writefileinfo_write_day:
        xor dx, dx
        div si
        add dl, 30h
        mov byte ptr [di], dl
        dec di
        loop writefileinfo_write_day

        lea dx, day
        call MESSAGE
        lea dx, space
        call MESSAGE

        mov ax, word ptr [bx+18h]
        lea di, day
        inc di
        mov cl, 05h
        mov si, 0Ah
        shr ax, cl
        mov cx, 02h
        and ax, 0Fh

writefileinfo_write_month:
        xor dx, dx
        div si
        add dl, 30h
        mov byte ptr [di], dl
        dec di
        loop writefileinfo_write_month

        lea dx, day
        call MESSAGE
        lea dx, space
        call MESSAGE

        mov ax, word ptr [bx+18h]
        lea di, year
        add di, 03h
        mov cl, 09h
        shr ax, cl
        add ax, 07BCh
        mov si, 0Ah
        mov cx, 04h

writefileinfo_write_year:
        xor dx, dx
        div si
        add dl, 30h
        mov byte ptr [di], dl
        dec di
        loop writefileinfo_write_year

        lea dx, year
        call MESSAGE

        pop dx
        pop cx
        pop ax
        pop di
        pop si
        ret
WRITEFILEINFO endp

main:
        mov ax, @data
        mov ds, ax    ; load data segment

        mov dx, 00h   ; load first sector
        call LOADSECTOR
        lea bx, entr_cnt
        mov cx, word ptr sbuf[11h]
        mov word ptr [bx], cx ; load entry count

        lea bx, copy_cnt ; load fat copies count
        mov cl, byte ptr sbuf[10h]
        mov byte ptr [bx], cl

        lea bx, resr_cnt ; load reserved sectors count
        mov cx, word ptr sbuf[0Eh]
        mov word ptr [bx], cx

        lea bx, fat_size ; load size of fat
        mov cx, word ptr sbuf[16h]
        mov word ptr [bx], cx

        mov ax, cx ; calc first sector of root dir
        xor ch, ch
        mov cl, byte ptr copy_cnt[00h]
        xor dx, dx
        mul cx
        mov cx, word ptr resr_cnt[00h]
        add ax, cx

        mov dx, ax ; root dir
        mov cx, word ptr entr_cnt[00h]
        
main_loop1_begin:
        call LOADSECTOR

        push bx
        push cx
        push dx

        mov ax, 10h
        cmp cx, ax
        jl main_cond1 
        mov cx, 10h
main_cond1:
        lea bx, sbuf

        push cx

main_loop2_begin:
        mov al, [bx]
        cmp al, 00h
        je main_loop2_end
        cmp al, 0E5h
        je main_loop2_end
        
        mov al, [bx+0Bh]
        cmp al, 0Fh
        je main_loop2_end

        call WRITEFILEINFO

        lea dx, newline
        call MESSAGE

main_loop2_end:
        add bx, 20h
        loop main_loop2_begin

        pop ax

        pop dx
        pop cx
        pop bx
        inc dx

        sub cx, ax
        jz main_loop1_end

        jmp main_loop1_begin
main_loop1_end:

exit:
        mov ah, 4Ch
        int 21h
end main
