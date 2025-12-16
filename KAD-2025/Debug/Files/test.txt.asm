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

	L0 sqword -10
.data

	a_main sqword 0
.code

;----------- MAIN ------------
main PROC
sub rsp, 28h

mov rax, L0
push rax
pop rax
mov a_main, rax

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