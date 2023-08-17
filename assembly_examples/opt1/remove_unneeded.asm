

label1: ; before
	mov	r2, r2, lsr #1
	bic	r3, r3, #65280
	mov	r1, r1, lsr #1
	bic	r0, r0, #65280
	bic	r5, ip, #255
	bic	fp, r4, #255

label2: ; after
    ldr	r4, [sp, #4]
	mov	r2, r2, lsr #1
	mov	r1, r1, lsr #1
	orr	r3, r4, r2, asl #8
	orr	r0, r0, r1, asl #8
