global _asm_fet
SECTION .text

_asm_fet:
	push	rbp	
	mov	rbp, rsp
	;mov	[rbp-0x34], edi	; max_it, max_it
	mov	[rbp-0x38], esi	; bailout, bailout
	;mov	[rbp-0x40], rdx	; res
	fild DWORD [rbp-0x38]	; bailout 
	fld	TWORD [rbp+0x20]	; im
	fld	TWORD [rbp+0x10]	; re
	fld	TWORD [rbp+0x40]	; imn			
	fld	TWORD [rbp+0x30]  ; ren			
	fld st1							;y^2			
	fmul st0, st0
	fld st1							;x^2			
	fmul st0, st0
	fld st0;
	fadd st0, st2;
	
	mov ecx, 1
	fcomi st0, st7
	ja .el
.lp:
	ffreep st0
	add ecx, 1
	fld st2	; calculate imn
	fmul st0, st4
	fadd st0, st0
	fadd st0, st6
	fstp st4 
	
	fld st0 ; calculate ren
	fsub st0, st2
	fadd st0, st5
	fstp st3
	
	fld st3				; calc y^2
	fmul st0, st0
	fstp st2
	
	fld st2
	fmul st0, st0
	fstp st1
	
	fld st0
	fadd st0, st2
	
	cmp ecx, edi
	jge .el2
	
	fcomi st0, st7
	
	jbe .lp	
.el:
;*res=0.05 * ( i+log2l(logl(bailout)) - log2l(logl(r2 + i2)));
	fstp st6
	ffreep st0
	ffreep st0
	ffreep st0
	ffreep st0
	ffreep st0
	fld1
	fldl2e
	fdivp st1, st0
	fst st3
	fxch st0, st1
	fyl2x
	fld1
	fxch st0, st1
	fyl2x
	fxch st0, st1
	fld st2
	fxch st0, st1
	fyl2x
	fld1
	fxch st0, st1
	fyl2x
	fsubrp st1, st0
	mov [rbp-0x34], ecx
	fild DWORD [rbp-0x34]
	faddp st1, st0
	mov DWORD [rbp-0x38], 20
	fild DWORD [rbp-0x38]
	fdivp st1, st0
	fstp TWORD [rdx]
	
	nop
	pop	rbp
	ret
.el2:
	ffreep st0
	ffreep st0
	ffreep st0
	ffreep st0
	ffreep st0
	ffreep st0
	ffreep st0
	ffreep st0
	fldz
	fld1
	fsubp st1, st0
	fstp TWORD [rdx]
	nop
	pop	rbp
	ret
