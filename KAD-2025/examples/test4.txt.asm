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

	L0 sqword 2
	L1 byte "--- Complex Write ---", 0
	L2 sqword 10
	L3 sqword -2147483648
	L4 sqword 122
	L5 byte "Hello", 0
	L6 sqword 5
.data

	x_main sqword 0
	aaqqaaqqaaqqaaqqaaqq_main sqword 0
	s_main qword 0
.code

;----------- zaza ------------
zaza PROC

mov rax, L0
push rax
mov rax, L0
push rax
pop rcx
pop rax
imul rcx
push rax
pop rax
ret

zaza ENDP
;------------------------------


;----------- MAIN ------------
main PROC
sub rsp, 28h

lea rax, L1
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

mov rax, L2
push rax
pop rax
mov x_main, rax

mov rax, L3
push rax
pop rax
mov aaqqaaqqaaqqaaqqaaqq_main, rax

mov rax, L0
push rax
mov rax, L4
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, x_main
push rax
mov rax, L0
push rax
pop rcx
pop rax
imul rcx
push rax
mov rax, L2
push rax
mov rax, L0
push rax
pop rcx
pop rax
cdq
idiv rcx
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, L0
push rax
mov rax, L0
push rax
mov rax, L0
push rax
pop rcx
pop rax
imul rcx
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

lea rax, L5
push rax
pop rax
mov s_main, rax

sub rsp, 20h
mov rax, s_main
mov rcx, rax
call lenght
add rsp, 20h
push rax
mov rax, L6
push rax
pop rcx
pop rax
add rax, rcx
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