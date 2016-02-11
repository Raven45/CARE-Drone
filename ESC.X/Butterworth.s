; ..............................................................................
;    File   Butterworth.s
; ..............................................................................

		.equ ButterworthNumSections, 2

; ..............................................................................
; Allocate and initialize filter coefficients
;
; These coefficients have been designed for use in the Canonic filter only

		.section .xdata, data, xmemory

ButterworthCoefs:
.hword	0xF2C9	; a( 1,2)/2
.hword	0x363A	; a( 1,1)/2
.hword	0x0714	; b( 1,2)/2
.hword	0x0E29	; b( 1,1)/2
.hword	0x0714	; b( 1,0)/2
.hword	0xDB68	; a( 2,2)/2
.hword	0x46A5	; a( 2,1)/2
.hword	0x10EA	; b( 2,2)/2
.hword	0x21D5	; b( 2,1)/2
.hword	0x10EA	; b( 2,0)/2

; ..............................................................................
; Allocate states buffer in (uninitialized) Y data space

		.section .ybss, bss, ymemory
ButterworthStates:
		.space ButterworthNumSections*2*2

; ..............................................................................
; Allocate and intialize filter structure

		.section .data
		.global _ButterworthFilter

_ButterworthFilter:
.hword ButterworthNumSections-1
.hword ButterworthCoefs
.hword 0xFF00
.hword ButterworthStates
.hword 0x16FC
.hword 0x0000

; ..............................................................................
; Sample assembly language calling program
;  The following declarations can be cut and pasted as needed into a program
;		.extern	_IIRCanonicInit
;		.extern	_IIRCanonicStruct
;		.extern	_ButterworthFilter
;
;		.section	.bss
;
;	 The input and output buffers can be made any desired size
;	   the value 40 is just an example - however, one must ensure
;	   that the output buffer is at least as long as the number of samples
;	   to be filtered (parameter 1)
;input:		.space	40
;output:	.space	40
;		.text
;
;
;  This code can be copied and pasted as needed into a program
;
;
; Set up pointers to access input samples, filter taps, delay line and
; output samples.
;		mov	#_ButterworthFilter, W3	; Initalize W0 to filter structure
;		call	_IIRCanonicInit	; call this function once
;
; The next 4 instructions are required prior to each subroutine call
; to _IIRCanonic
;		mov	#_ButterworthFilter, W3	; Initalize W3 to filter structure
;		mov	#input, W2	; Initalize W2 to input buffer 
;		mov	#output, W1	; Initalize W1 to output buffer
;		mov	#20, W0	; Initialize W0 with number of required output samples
;		call	_IIRCanonic	; call as many times as needed
