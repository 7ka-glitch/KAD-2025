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

	L0 qword 2
	L1 db "Hello World", 0
	L2 db "---TEST---", 0
	L3 qword 1
	L4 qword 10
	L5 qword -2147483648
	L6 qword 122
	L7 qword 3
	L8 db "Hello", 0
	L9 qword 5
	L10 db "b=", 0
.data

	b_main qword 0
	x_main qword 0
	aaqqaaqqaaqqaaqqaaqq_main qword 0
	s_main qword 0
	za_main qword 0
	st_main qword 0
.code

;----------- zaza ------------
zaza PROC, x_zaza: QWORD, y_zaza: QWORD
mov x_zaza, rcx
mov y_zaza, rdx

mov rax, L0
push rax
pop rcx
push rcx
cycle1:
cmp qword ptr [rsp], 0
jle end_cycle1

lea rax, L1
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

dec qword ptr [rsp]
jmp cycle1
end_cycle1:
pop rcx

mov rax, x_zaza
push rax
mov rax, y_zaza
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

lea rax, L2
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

sub rsp, 20h
mov rax, L3
mov rcx, rax
mov rax, L0
mov rdx, rax
call zaza
add rsp, 20h
push rax
pop rax
mov b_main, rax

mov rax, b_main
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, L4
push rax
mov rax, L0
push rax
pop rcx
pop rax
sub rax, rcx
push rax
pop rax
mov x_main, rax

mov rax, L5
push rax
pop rax
mov aaqqaaqqaaqqaaqqaaqq_main, rax

mov rax, L0
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

mov rax, x_main
push rax
mov rax, L0
push rax
pop rcx
pop rax
imul rcx
push rax
mov rax, L4
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

mov rax, x_main
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

mov rax, L7
push rax
mov rax, L0
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, L0
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
sub rsp, 20h
call write_int
add rsp, 20h

lea rax, L8
push rax
pop rax
mov s_main, rax

sub rsp, 20h
mov rax, L0
mov rcx, rax
call intToStr
add rsp, 20h
push rax
pop rax
mov za_main, rax

sub rsp, 20h
mov rax, za_main
mov rcx, rax
call lenght
add rsp, 20h
push rax
mov rax, L9
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, za_main
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

lea rax, L10
push rax
pop rax
mov st_main, rax

mov rax, L0
push rax
mov rax, L7
push rax
mov rax, L0
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
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, L3
push rax
mov rax, L7
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rcx
push rcx
cycle2:
cmp qword ptr [rsp], 0
jle end_cycle2

mov rax, st_main
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

mov rax, b_main
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

dec qword ptr [rsp]
jmp cycle2
end_cycle2:
pop rcx

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