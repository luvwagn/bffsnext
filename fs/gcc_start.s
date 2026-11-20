; gcc_start.s - entry point and interrupt stub for the m68k-amigaos-gcc
; build of BFFS (vasm, Motorola syntax).  Equivalent of stack.asm and
; inthand.asm from the DICE build; must be the first object linked so
; that _bffs_start is the first code in the first hunk.
;
; The stack swap routine was written by Ken Dyke at Commodore (12/14/93),
; see stack.asm.

	XDEF	_bffs_start
	XDEF	_IntHandler
	XREF	_handler

STACKSPACE	EQU	8192	; gcc code uses more stack than DICE's 2048

_LVOAllocVec	EQU	-684
_LVOFreeVec	EQU	-690
_LVOStackSwap	EQU	-732
_LVOSignal	EQU	-324
stk_Lower	EQU	0
stk_Upper	EQU	4
stk_Pointer	EQU	8

	section	text,code

_bffs_start:
	movem.l	d2-d7/a2-a6,-(sp)
	movea.l	4.w,a6			; ExecBase
	move.l	#STACKSPACE+12,d0	; stack plus StackSwap structure
	moveq	#0,d1			; any memory
	jsr	_LVOAllocVec(a6)
	tst.l	d0
	beq.s	.exit

	movea.l	d0,a2			; StackSwap struct
	movea.l	d0,a3			; allocation, for FreeVec
	add.l	#12,d0
	move.l	d0,stk_Lower(a2)
	add.l	#STACKSPACE,d0
	move.l	d0,stk_Upper(a2)
	subq.l	#4,d0
	move.l	d0,stk_Pointer(a2)

	movea.l	a2,a0
	jsr	_LVOStackSwap(a6)

	movem.l	a2-a3/a6,-(sp)
	jsr	_handler
	movem.l	(sp)+,a2-a3/a6

	movea.l	a2,a0
	jsr	_LVOStackSwap(a6)

	movea.l	a3,a1
	jsr	_LVOFreeVec(a6)
.exit:
	movem.l	(sp)+,d2-d7/a2-a6
	moveq	#0,d0
	rts

; Interrupt server: a1 = is_Data -> { task pointer, signal mask }
_IntHandler:
	move.l	a6,-(sp)
	move.l	a1,-(sp)
	movea.l	4.w,a6
	move.l	4(a1),d0
	movea.l	(a1),a1
	jsr	_LVOSignal(a6)
	movea.l	(sp)+,a1
	movea.l	(sp)+,a6
	moveq	#0,d0			; Z set: continue the server chain
	rts
