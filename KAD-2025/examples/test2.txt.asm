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

	L0 byte "--- Nested Cycles ---", 0
	L1 sqword 0
	L2 sqword 3
	L3 byte "Outer", 0
	L4 sqword 2
	L5 byte "Inner", 0
	L6 sqword 1
	L7 byte "Total iterations:", 0
.data

	counter_main sqword 0
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
mov counter_main, rax

mov rax, L2
push rax
pop rcx
push rcx
cycle1:
cmp qword ptr [rsp], 0
jle end_cycle1

lea rax, L3
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

mov rax, L4
push rax
pop rcx
push rcx
cycle2:
cmp qword ptr [rsp], 0
jle end_cycle2

lea rax, L5
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

mov rax, counter_main
push rax
mov rax, L6
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rax
mov counter_main, rax

dec qword ptr [rsp]
jmp cycle2
end_cycle2:
pop rcx

dec qword ptr [rsp]
jmp cycle1
end_cycle1:
pop rcx

lea rax, L7
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

mov rax, counter_main
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