includelib kernel32.lib
ExitProcess PROTO
EXTRN lenght: PROC
EXTRN write_int: PROC
EXTRN write_str: PROC
EXTRN intToStr: PROC
EXTRN powNumber: PROC
EXTRN system: PROC

.const
nulError byte 'error divided by zero', 0
nul sqword 0
 pauseStr db "pause",0

	L0 byte "--- Math Test ---", 0
	L1 sqword 2
	L2 sqword 10
	L3 sqword 5
	L4 sqword 3
.data

	a_main sqword 0
	b_main sqword 0
	c_main sqword 0
.code

;----------- MAIN ------------
main PROC
sub rsp, 28h

lea rax, L0
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

mov rax, L1
push rax
mov rax, L1
push rax
mov rax, L1
push rax
pop rcx
pop rax
imul rcx
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rax
mov a_main, rax

mov rax, a_main
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, L1
push rax
mov rax, L1
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, L1
push rax
pop rcx
pop rax
imul rcx
push rax
pop rax
mov a_main, rax

mov rax, a_main
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, L2
push rax
mov rax, L1
push rax
pop rcx
pop rax
cdq
idiv rcx
push rax
mov rax, L3
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rax
mov b_main, rax

mov rax, b_main
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, L1
push rax
mov rax, L4
push rax
mov rax, L1
push rax
pop rcx
pop rax
mov rdx, rcx
mov rcx, rax
mov rbx, rsp
and rsp, -16
sub rsp, 20h
call powNumber
mov rsp, rbx
push rax
pop rcx
pop rax
mov rdx, rcx
mov rcx, rax
mov rbx, rsp
and rsp, -16
sub rsp, 20h
call powNumber
mov rsp, rbx
push rax
pop rax
mov c_main, rax

mov rax, c_main
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

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