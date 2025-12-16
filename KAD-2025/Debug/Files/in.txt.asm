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

	 qword 2
	 db "Hello World", 0
	 db "---TEST---", 0
	 qword 1
	 qword 10
	 qword -2147483648
	 qword 122
	 db "Hello", 0
	 qword 5
	 db "b=", 0
	 qword 3
.data

	b_main qword 0
	x_main qword 0
	aaqqaaqqaaqqaaqqaaqq_main qword 0
	ava_main qword 0
	s_main qword 0
	za_main qword 0
	st_main qword 0
.code

;----------- zaza ------------
zaza PROC, x_zaza: QWORD, y_zaza: QWORD
mov x_zaza, rcx
mov y_zaza, rdx

mov rax, 
push rax
pop rcx
push rcx
cycle1:
cmp qword ptr [rsp], 0
jle end_cycle1

lea rax, 
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

lea rax, 
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

sub rsp, 20h
mov rax, 
mov rcx, rax
mov rax, 
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

mov rax, 
push rax
mov rax, 
push rax
pop rcx
pop rax
sub rax, rcx
push rax
pop rax
mov x_main, rax

mov rax, 
push rax
pop rax
mov aaqqaaqqaaqqaaqqaaqq_main, rax

mov rax, 
push rax
mov rax, 
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
mov rax, 
push rax
pop rcx
pop rax
imul rcx
push rax
mov rax, 
push rax
mov rax, 
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
mov rax, 
push rax
mov rax, 
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

mov rax, 
push rax
pop rax
mov ava_main, rax

mov rax, ava_main
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

mov rax, 
push rax
mov rax, 
push rax
pop rcx
pop rax
sub rax, rcx
push rax
pop rcx
sub rsp, 20h
call write_int
add rsp, 20h

lea rax, 
push rax
pop rax
mov s_main, rax

sub rsp, 20h
mov rax, 
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
mov rax, 
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

lea rax, 
push rax
pop rax
mov st_main, rax

mov rax, 
push rax
mov rax, 
push rax
mov rax, 
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

mov rax, 
push rax
mov rax, 
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