label1: ; Before	
    ldr	r6, [sp, #40]
	ldr	r9, [sp, #36]
	ldr	ip, [sp, #176]
	add	r3, r6, r8
	add	r1, r9, r8
	add	r2, ip, r8
	ldr	r0, [r3, #8]
	ldr	r6, [r1, #8]
	ldr	r2, [r2, #4]
	ldr	r1, [r1, #4]
	str	r0, [sp, #88]
	str	r2, [sp, #120]
	add	r9, sp, #160
	ldmia	r9, {r9, ip}	@ phole ldm
	ldr	r0, [sp, #168]
	str	r1, [sp, #104]
	ldr	r2, [sp, #172]
	ldr	r1, [sp, #172]
	ldr	r8, [r8, #4]
	ldr	r9, [r9, #8]
	ldr	ip, [ip, #0]
	ldr	r3, [r3, #4]
	ldr	r0, [r0, #0]
	ldr	r1, [r1, #0]
	ldr	r2, [r2, #8]
	str	r6, [sp, #108]
	str	r8, [sp, #68]
	str	r9, [sp, #72]
	str	ip, [sp, #80]
	str	r3, [sp, #84]
	str	r0, [sp, #100]
	str	r1, [sp, #116]
	str	r2, [sp, #124]

label2 ; After
	ldr	r5, [sp, #168]
	ldr	r6, [sp, #168]
	ldr	ip, [sp, #168]
	ldr	r0, [sp, #128]
	ldr	r5, [r5, #0]
	ldr	r6, [r6, #4]
	ldr	ip, [ip, #8]
	ldr	r0, [r0, #0]
	str	r5, [sp, #32]
	str	r6, [sp, #36]
	str	ip, [sp, #40]
	str	r0, [sp, #48]
	ldr	r1, [sp, #128]
	ldr	r4, [sp, #124]
	ldr	r5, [sp, #124]
	ldr	r6, [sp, #120]
	ldr	ip, [sp, #120]
	ldr	r0, [sp, #120]
	ldr	r1, [r1, #4]
	ldr	r2, [r2, #8]
	ldr	r3, [r3, #0]
	ldr	r4, [r4, #4]
	ldr	r5, [r5, #8]
	ldr	r6, [r6, #0]
	ldr	ip, [ip, #4]
	ldr	r0, [r0, #8]
	str	r1, [sp, #52]
	str	r2, [sp, #56]
	str	r3, [sp, #64]
	str	r4, [sp, #68]
	str	r5, [sp, #72]
	str	r6, [sp, #80]
	str	ip, [sp, #84]
	str	r0, [sp, #88]