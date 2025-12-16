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

	L0 byte "--- Div by Zero ---", 0
	L1 sqword 10
	L2 sqword 0
	L3 byte "About to crash", 0
	L4 sqword 5
	L5 byte "Should not see this", 0
.data

	x_main sqword 0
	y_main sqword 0
	z_main qword 0
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
pop rax
mov x_main, rax

mov rax, L2
push rax
pop rax
mov y_main, rax

lea rax, L3
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

sub rsp, 20h
mov rax, L4
mov rcx, rax
call intToStr
add rsp, 20h
push rax
pop rax
mov z_main, rax

lea rax, L5
push rax
pop rcx
sub rsp, 20h
call write_str
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