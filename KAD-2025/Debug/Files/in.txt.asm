includelib kernel32.lib
ExitProcess PROTO
EXTRN lenght: PROC
EXTRN write_int: PROC
EXTRN write_str: PROC
EXTRN intToStr: PROC
EXTRN powNumber: PROC
EXTRN system: PROC

.const
nulError db 'error divided by zero', 0
nul qword 0
pauseStr db "pause",0

	L0 qword 2147483647
	L1 qword 2147483645
	L2 qword 1
	L3 qword 2
.data

	x_main qword 0
.code

;----------- y ------------
y PROC, a_y: QWORD, b_y: QWORD
mov a_y, rcx
mov b_y, rdx

mov rax, a_y
push rax
mov rax, L0
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rax
ret

y ENDP
;------------------------------


;----------- MAIN ------------
main PROC
sub rsp, 28h

mov rax, L1
push rax
sub rsp, 20h
mov rax, L2
mov rcx, rax
mov rax, L3
mov rdx, rax
call y
add rsp, 20h
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rax
mov x_main, rax

jmp goodExit
errorExit:
sub rsp, 28h
lea rcx, nulError
call write_str
add rsp, 28h
goodExit:
sub rsp, 40h
lea rcx, pauseStr
sub rsp, 20h
call system
add rsp, 20h
xor rcx, rcx
sub rsp, 20h
call ExitProcess
main ENDP
end