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

	L0 byte "--- 10 Params Test ---", 0
	L1 sqword 1
.data

	sum_manyParams sqword 0
	res_main sqword 0
.code

;----------- manyParams ------------
manyParams PROC, a_manyParams: QWORD, b_manyParams: QWORD, c_manyParams: QWORD, d_manyParams: QWORD, e_manyParams: QWORD, f_manyParams: QWORD, g_manyParams: QWORD, h_manyParams: QWORD, i_manyParams: QWORD, j_manyParams: QWORD

mov rax, a_manyParams
push rax
mov rax, b_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, c_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, d_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, e_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, f_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, g_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, h_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, i_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
mov rax, j_manyParams
push rax
pop rcx
pop rax
add rax, rcx
push rax
pop rax
mov sum_manyParams, rax

mov rax, sum_manyParams
push rax
pop rax
ret

manyParams ENDP
;------------------------------


;----------- MAIN ------------
main PROC
sub rsp, 28h

lea rax, L0
push rax
pop rcx
sub rsp, 20h
call write_str
add rsp, 20h

