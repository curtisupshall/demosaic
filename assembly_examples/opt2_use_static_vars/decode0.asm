	.arch armv4t
	.eabi_attribute 27, 3
	.fpu neon
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 18, 4
	.file	"decode.c"
	.text
	.align	2
	.global	decodeImage
	.type	decodeImage, %function
decodeImage:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 240
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	cmp	r3, #0
	sub	sp, sp, #240
	str	r3, [sp, #48]
	str	r0, [sp, #56]
	str	r1, [sp, #52]
	beq	.L17
	mov	r3, r1, asl #2
	mov	r8, r1, asl #4
	mov	r9, r1, asl #3
	rsb	ip, r3, r8
	mov	r2, r2, lsr #2
	add	r1, r0, r3
	str	r3, [sp, #44]
	str	r8, [sp, #36]
	str	r2, [sp, #32]
	add	r3, r0, ip
	add	r2, r0, r9
	mov	r8, #0
	str	r9, [sp, #40]
	str	ip, [sp, #184]
	str	r0, [sp, #168]
	str	r1, [sp, #172]
	str	r2, [sp, #176]
	str	r3, [sp, #180]
	str	r8, [sp, #196]
.L5:
	ldr	r0, [sp, #168]
	add	r2, sp, #40
	ldmia	r2, {r2, ip}	@ phole ldm
	ldr	r8, [sp, #184]
	ldr	r9, [sp, #32]
	add	r3, ip, r0
	add	r1, r2, r0
	add	r2, r8, r0
	ldr	ip, [r1, #8]
	ldr	r2, [r2, #4]
	ldr	r0, [r0, #4]
	cmp	r9, #0
	ldr	r9, [r3, #8]
	str	ip, [sp, #112]
	str	r9, [sp, #92]
	str	r2, [sp, #124]
	str	r0, [sp, #72]
	add	r2, sp, #168
	ldmia	r2, {r2, r8, r9, ip}	@ phole ldm
	ldr	r0, [sp, #180]
	ldr	r2, [r2, #8]
	ldr	r8, [r8, #0]
	ldr	r3, [r3, #4]
	ldr	r9, [r9, #0]
	ldr	r1, [r1, #4]
	ldr	ip, [ip, #0]
	ldr	r0, [r0, #8]
	str	r2, [sp, #76]
	str	r8, [sp, #84]
	str	r3, [sp, #88]
	str	r9, [sp, #104]
	str	r1, [sp, #108]
	str	ip, [sp, #120]
	str	r0, [sp, #128]
	beq	.L3
	and	r2, r3, #255
	ldr	r1, [sp, #72]
	ldr	r3, [sp, #108]
	str	r2, [sp, #16]
	ldr	r8, [sp, #124]
	and	r9, r1, #65280
	ldr	r2, [sp, #68]
	and	r1, r3, #65280
	ldr	r3, [sp, #16]
	and	r8, r8, #255
	mov	ip, #0
	str	r8, [sp, #24]
	mov	r2, r2, lsr #8
	str	ip, [sp, #60]
	str	r3, [sp, #96]
	ldr	r8, [sp, #84]
	ldr	ip, [sp, #104]
	ldr	r3, [sp, #120]
	str	r2, [sp, #80]
	ldr	r2, [sp, #24]
	mov	r0, r9, lsr #8
	and	r8, r8, #255
	mov	ip, ip, lsr #8
	and	r3, r3, #255
	str	r0, [sp, #188]
	str	r8, [sp, #100]
	mov	r0, r1, lsr #8
	str	ip, [sp, #116]
	str	r2, [sp, #132]
	str	r3, [sp, #136]
	mov	fp, #0
	b	.L4
.L18:
	ldr	ip, [sp, #88]
	ldr	r3, [sp, #124]
	ldr	r8, [sp, #72]
	and	ip, ip, #255
	ldr	r2, [sp, #108]
	and	r3, r3, #255
	str	ip, [sp, #16]
	str	r3, [sp, #24]
	and	r9, r8, #65280
	and	r1, r2, #65280
.L4:
#APP
@ 212 "src/decode.c" 1
	Label1
@ 0 "" 2
	mov	r1, r1, lsr #8
	ldr	r3, [sp, #96]
	str	r1, [sp, #192]
	add	r1, r1, r0
	ldr	r0, [sp, #16]
	ldr	r8, [sp, #188]
	add	ip, r0, r3
	ldr	r0, [sp, #24]
	ldr	r3, [sp, #132]
	mov	r9, r9, lsr #8
	add	r2, r9, r8
	add	r8, r0, r3
#APP
@ 239 "src/decode.c" 1
	Label2
@ 0 "" 2
	ldr	r0, [sp, #68]
	ldr	sl, [sp, #168]
	bic	r3, r0, #16711680
	mov	r2, r2, lsr #1
	orr	r3, r3, r2, asl #16
	str	r3, [sl, fp]!
	ldr	r3, [sp, #104]
	ldr	r2, [sp, #88]
	bic	r0, r3, #16711680
	mov	r1, r1, lsr #1
	ldr	r3, [sp, #84]
	orr	r0, r0, r1, asl #16
	and	r6, r2, #16711680
	ldr	r1, [sp, #100]
	mov	r6, r6, lsr #16
	add	r2, r6, r1
	mov	ip, ip, lsr #1
	bic	r1, r3, #-16777216
	ldr	r7, [sp, #176]
	ldr	r3, [sp, #124]
	orr	r1, r1, ip, asl #8
	ldr	ip, [sl, #12]
	ldr	r5, [sp, #172]
	str	r0, [r7, fp]!
	and	r4, r3, #16711680
	ldr	r0, [sp, #136]
	mov	r2, r2, lsr #1
	str	ip, [sp, #68]
	ldr	ip, [sp, #120]
	orr	r1, r1, r2, asl #24
	mov	r4, r4, lsr #16
	add	r3, r4, r0
	bic	r2, ip, #-16777216
	ldr	r0, [r7, #12]
	mov	r8, r8, lsr #1
	str	r1, [r5, fp]!
	ldr	r1, [sp, #92]
	orr	r2, r2, r8, asl #8
	mov	r3, r3, lsr #1
	orr	r2, r2, r3, asl #24
	and	r3, r1, #16711680
	mov	r3, r3, lsr #16
	ldr	r8, [sp, #80]
	str	r3, [sp, #96]
	str	r0, [sp, #104]
	ldr	r3, [r5, #12]
	ldr	r0, [sp, #72]
	str	r3, [sp, #84]
	add	r1, r8, r0, lsr #24
	ldr	r3, [sp, #128]
	ldr	ip, [sp, #180]
	orr	r1, r0, r1, lsr #1
	ldr	r8, [sp, #96]
	ldr	r0, [sp, #16]
	and	r3, r3, #16711680
	str	r2, [ip, fp]!
	str	r3, [sp, #4]
	add	r2, r8, r0
	ldr	r3, [sp, #108]
	ldr	r8, [sp, #116]
	add	r0, r8, r3, lsr #24
	ldr	r8, [sp, #4]
	mov	r8, r8, lsr #16
	str	r8, [sp, #132]
	ldr	r3, [ip, #12]
	ldr	r8, [sp, #68]
	str	r3, [sp, #120]
	mov	r3, r8, lsr #8
	ldr	r8, [sp, #88]
	mov	r2, r2, lsr #1
	orr	r2, r8, r2, asl #24
	and	r3, r3, #255
	str	r1, [sl, #4]
	str	r2, [sp, #0]
	ldr	r1, [sp, #108]
	str	r3, [sp, #80]
	ldr	r2, [sp, #132]
	ldr	r3, [sp, #24]
	ldr	r8, [sp, #76]
	orr	r0, r1, r0, lsr #1
	mov	r8, r8, lsr #24
	add	r1, r2, r3
	add	r2, sp, #76
	ldmia	r2, {r2, r3}	@ phole ldm
	str	r8, [sp, #188]
	orr	r8, r2, r3, asl #16
	ldr	r2, [sp, #188]
	add	r9, r2, r9
	ldr	r2, [sp, #0]
	ldr	r3, [sl, #16]
	str	r0, [r7, #4]
	ldr	r0, [r7, #16]
	str	r2, [r5, #4]
	ldr	r2, [sp, #84]
	str	r0, [sp, #108]
	ldr	r0, [sp, #104]
	and	r2, r2, #255
	str	r3, [sp, #72]
	str	r2, [sp, #100]
	mov	r3, r0, lsr #8
	add	r6, r6, r2
	ldr	r0, [sp, #124]
	ldr	r2, [sp, #112]
	orr	r8, r8, r9, lsr #1
	mov	r1, r1, lsr #1
	ldr	r9, [sp, #192]
	orr	r1, r0, r1, asl #24
	and	r3, r3, #255
	mov	r0, r2, lsr #24
	str	r3, [sp, #116]
	orr	r3, r2, r3, asl #16
	add	r2, r0, r9
	ldr	r9, [r5, #16]
	str	r1, [ip, #4]
	ldr	r1, [ip, #16]
	str	r9, [sp, #88]
	str	r1, [sp, #124]
	str	r8, [sl, #8]
	ldr	r8, [sp, #120]
	ldr	r9, [sp, #92]
	and	r8, r8, #255
	mov	r6, r6, lsr #1
	orr	r3, r3, r2, lsr #1
	orr	r6, r9, r6, asl #8
	ldr	r2, [sp, #128]
	add	r4, r4, r8
	ldr	sl, [sl, #20]
	ldr	r1, [sp, #60]
	str	r3, [r7, #8]
	mov	r4, r4, lsr #1
	ldr	r7, [r7, #20]
	str	r6, [r5, #8]
	orr	r4, r2, r4, asl #8
	ldr	r3, [sp, #32]
	add	r1, r1, #1
	ldr	r5, [r5, #20]
	str	r4, [ip, #8]
	ldr	ip, [ip, #20]
	cmp	r3, r1
	str	r8, [sp, #136]
	str	sl, [sp, #76]
	str	r7, [sp, #112]
	str	r1, [sp, #60]
	str	r5, [sp, #92]
	str	ip, [sp, #128]
	add	fp, fp, #12
	bhi	.L18
.L3:
	ldr	r8, [sp, #196]
	ldr	r0, [sp, #36]
	ldr	ip, [sp, #168]
	add	r1, sp, #172
	ldmia	r1, {r1, r2, r3}	@ phole ldm
	ldr	r9, [sp, #48]
	add	r8, r8, #4
	add	ip, ip, r0
	add	r1, r1, r0
	add	r2, r2, r0
	add	r3, r3, r0
	cmp	r9, r8
	str	r8, [sp, #196]
	str	ip, [sp, #168]
	str	r1, [sp, #172]
	str	r2, [sp, #176]
	str	r3, [sp, #180]
	bhi	.L5
	ldr	r3, [sp, #36]
	ldr	r8, [sp, #44]
.L2:
	ldr	r9, [sp, #56]
	ldr	ip, [sp, #52]
	add	r0, r9, r8
	rsb	r2, r8, r3
	ldr	r8, [sp, #48]
	add	r2, r9, r2
	add	r3, ip, #1
	ldr	r1, [sp, #40]
	str	r2, [sp, #20]
	sub	r8, r8, #2
	mov	r2, #0
	mov	r3, r3, asl #2
	add	r1, r9, r1
	add	r3, r9, r3
	str	r9, [sp, #144]
	str	r0, [sp, #12]
	str	r0, [sp, #152]
	ldr	r9, [sp, #20]
	mov	ip, r2
	mov	r0, r8
	cmp	ip, r0
	str	r1, [sp, #28]
	str	r2, [sp, #64]
	str	r8, [sp, #140]
	str	r3, [sp, #156]
	str	r1, [sp, #160]
	str	r9, [sp, #164]
	str	r2, [sp, #148]
	bcs	.L19
.L9:
	ldr	r8, [sp, #32]
	ldr	ip, [sp, #148]
	cmp	r8, #0
	ldr	r8, [sp, #20]
	add	r1, r8, ip
	ldr	r0, [sp, #12]
	ldr	r1, [r1, #4]
	ldr	r9, [sp, #28]
	add	r2, r0, ip
	str	r1, [sp, #228]
	ldr	r0, [sp, #144]
	ldr	r1, [sp, #144]
	add	r3, r9, ip
	ldr	ip, [r3, #8]
	ldr	r2, [r2, #8]
	ldr	r0, [r0, #0]
	ldr	r1, [r1, #4]
	str	r2, [sp, #212]
	str	ip, [sp, #224]
	ldr	r2, [sp, #144]
	str	r0, [sp, #200]
	str	r1, [sp, #204]
	ldr	ip, [sp, #156]
	add	r0, sp, #160
	ldmia	r0, {r0, r1}	@ phole ldm
	ldr	r9, [sp, #64]
	ldr	ip, [ip, #0]
	ldr	fp, [r2, #8]
	ldr	r8, [sp, #152]
	ldr	r0, [r0, #0]
	ldr	r3, [r3, #4]
	ldr	r2, [r1, #8]
	add	r9, r9, #2
	str	r9, [sp, #64]
	str	ip, [sp, #208]
	ldr	r9, [r8, #0]
	str	r0, [sp, #216]
	str	r3, [sp, #220]
	ldr	ip, [r1, #0]
	str	r2, [sp, #232]
	beq	.L6
	mov	r3, #0
	str	r3, [sp, #236]
	mov	sl, r3
.L7:
	mov	r2, r9, lsr #1
	ldr	r8, [sp, #216]
	add	r2, r2, ip, lsr #1
	and	r1, r9, #255
	and	r3, ip, #255
	and	r2, r2, #-16777216
	ldr	r0, [sp, #200]
	add	r3, r3, r1
	orr	r2, r2, r8
	ldr	r7, [sp, #144]
	orr	r2, r2, r3, lsr #1
	and	r1, r0, #16711680
	and	r3, r2, #16711680
	str	r0, [r7, sl]!
	ldr	r6, [sp, #160]
	add	r3, r3, r1
	mov	r3, r3, lsr #1
	ldr	r1, [r7, #12]
	ldr	r5, [sp, #152]
	str	r2, [r6, sl]!
	and	r3, r3, #16711680
	orr	r3, r3, r9
	ldr	r2, [r6, #12]
	str	r3, [r5, sl]!
	add	r3, sp, #204
	ldmia	r3, {r3, r9}	@ phole ldm
	and	r8, r3, #65024
	orr	r8, r8, r9
	str	r1, [sp, #200]
	mov	r3, r8, lsr #1
	ldr	r1, [sp, #224]
	ldr	r4, [sp, #164]
	str	r2, [sp, #216]
	mov	r0, fp, lsr #1
	ldr	r2, [sp, #228]
	and	r3, r3, #8323072
	str	r3, [sp, #4]
	add	r0, r0, r1, lsr #1
	ldr	r3, [sp, #212]
	ldr	r9, [r5, #12]
	and	r0, r0, #-16777216
	str	ip, [r4, sl]!
	mov	ip, r2, lsr #1
	and	r2, r1, #255
	and	r1, fp, #255
	add	r2, r2, r1
	orr	r0, r0, r3
	ldr	r1, [sp, #4]
	orr	r0, r0, r2, lsr #1
	and	ip, ip, #8323072
	ldr	r2, [sp, #220]
	add	r3, r1, ip
	ldr	r1, [sp, #204]
	orr	r3, r3, r2
	ldr	ip, [r4, #12]
	str	r1, [r7, #4]
	ldr	r1, [r7, #16]
	str	r3, [r6, #4]
	ldr	r2, [r6, #16]
	ldr	r3, [sp, #232]
	str	r8, [r5, #4]
	ldr	r8, [r5, #16]
	str	r1, [sp, #204]
	str	r2, [sp, #220]
	str	r8, [sp, #208]
	mov	r2, r3, lsr #1
	ldr	r1, [sp, #228]
	mov	r3, r0, lsr #1
	and	r2, r2, #32512
	ldr	r8, [sp, #224]
	and	r3, r3, #32512
	str	r1, [r4, #4]
	add	r3, r3, r2
	ldr	r2, [r4, #16]
	str	fp, [r7, #8]
	orr	r3, r3, r8
	ldr	fp, [r7, #20]
	str	r2, [sp, #228]
	str	r3, [r6, #8]
	ldr	r2, [sp, #232]
	ldr	r6, [r6, #20]
	str	r0, [r5, #8]
	ldr	r0, [sp, #236]
	ldr	r1, [sp, #32]
	add	r0, r0, #1
	ldr	r5, [r5, #20]
	str	r2, [r4, #8]
	ldr	r4, [r4, #20]
	cmp	r1, r0
	str	r6, [sp, #224]
	str	r0, [sp, #236]
	str	r5, [sp, #212]
	add	sl, sl, #12
	str	r4, [sp, #232]
	bhi	.L7
.L6:
	ldr	r8, [sp, #40]
	ldr	ip, [sp, #152]
	ldr	r0, [sp, #156]
	add	ip, ip, r8
	add	r0, r0, r8
	add	r3, sp, #144
	ldmia	r3, {r3, r9}	@ phole ldm
	add	r1, sp, #160
	ldmia	r1, {r1, r2}	@ phole ldm
	str	ip, [sp, #152]
	str	r0, [sp, #156]
	ldr	ip, [sp, #64]
	ldr	r0, [sp, #140]
	add	r3, r3, r8
	add	r9, r9, r8
	add	r1, r1, r8
	add	r2, r2, r8
	cmp	ip, r0
	str	r3, [sp, #144]
	str	r9, [sp, #148]
	str	r1, [sp, #160]
	str	r2, [sp, #164]
	bcc	.L9
.L19:
	add	sp, sp, #240
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	bx	lr
.L17:
	mov	r2, r2, lsr #2
	str	r2, [sp, #32]
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #52]
	mov	r0, r0, asl #4
	mov	r1, r1, asl #2
	mov	r2, r2, asl #3
	str	r0, [sp, #36]
	str	r1, [sp, #44]
	str	r2, [sp, #40]
	mov	r3, r0
	mov	r8, r1
	b	.L2
	.size	decodeImage, .-decodeImage
	.align	2
	.global	writeImage
	.type	writeImage, %function
writeImage:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, lr}
	mov	r8, r1
	ldr	r1, .L24
	mov	r5, r2
	mov	r4, r3
	mov	r7, r0
	bl	fopen
	subs	r6, r0, #0
	beq	.L23
	mov	r3, r6
	mov	r0, r5
	mov	r1, #14
	mov	r2, #1
	bl	fwrite
	mov	r3, r6
	mov	r0, r4
	mov	r1, #40
	mov	r2, #1
	bl	fwrite
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	ldrb	r0, [r4, #6]	@ zero_extendqisi2
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	orr	r3, r3, r2, asl #8
	orr	r3, r3, r0, asl #16
	orr	r3, r3, r1, asl #24
	add	r3, r3, #1
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	ldrb	r0, [r4, #8]	@ zero_extendqisi2
	mov	r2, r3, asl #1
	ldrb	ip, [r4, #10]	@ zero_extendqisi2
	add	r2, r2, r3
	bic	r2, r2, #3
	orr	r0, r0, r1, asl #8
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	cmp	r2, #0
	orr	r0, r0, ip, asl #16
	add	r3, r2, #3
	movlt	r2, r3
	orr	r0, r0, r1, asl #24
	mov	r0, r0, asl #2
	mov	r2, r2, asr #2
	mul	r1, r2, r0
	mov	r3, r6
	mov	r2, #1
	mov	r0, r8
	bl	fwrite
	mov	r0, r8
	bl	free
	mov	r0, r6
	bl	fclose
	ldmfd	sp!, {r4, r5, r6, r7, r8, lr}
	bx	lr
.L23:
	mov	r1, r7
	ldr	r0, .L24+4
	bl	printf
	mov	r0, r8
	bl	free
	mov	r0, #1
	bl	exit
.L25:
	.align	2
.L24:
	.word	.LC0
	.word	.LC1
	.size	writeImage, .-writeImage
	.align	2
	.global	loadImage
	.type	loadImage, %function
loadImage:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, lr}
	mov	r5, r1
	ldr	r1, .L36
	mov	r4, r2
	mov	r6, r0
	bl	fopen
	subs	r7, r0, #0
	beq	.L33
	mov	r1, #14
	mov	r2, #1
	mov	r3, r7
	mov	r0, r5
	bl	fread
	mov	r1, #40
	mov	r2, #1
	mov	r3, r7
	mov	r0, r4
	bl	fread
	ldrb	r1, [r5, #1]	@ zero_extendqisi2
	ldrb	r2, [r5, #0]	@ zero_extendqisi2
	mov	r3, #19712
	orr	r2, r2, r1, asl #8
	add	r3, r3, #66
	cmp	r2, r3
	ldrne	r0, .L36+4
	bne	.L32
	ldrb	r3, [r4, #14]	@ zero_extendqisi2
	ldrb	r2, [r4, #15]	@ zero_extendqisi2
	orr	r3, r3, r2, asl #8
	cmp	r3, #24
	bne	.L34
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	ldrb	r0, [r4, #6]	@ zero_extendqisi2
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	orr	r3, r3, r2, asl #8
	orr	r3, r3, r0, asl #16
	orr	r3, r3, r1, asl #24
	add	r3, r3, #1
	ldrb	r0, [r4, #9]	@ zero_extendqisi2
	mov	r2, r3, asl #1
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	add	r2, r2, r3
	ldrb	ip, [r4, #10]	@ zero_extendqisi2
	bic	r2, r2, #3
	orr	r1, r1, r0, asl #8
	ldrb	r0, [r4, #11]	@ zero_extendqisi2
	cmp	r2, #0
	orr	r1, r1, ip, asl #16
	add	r3, r2, #3
	movlt	r2, r3
	orr	r1, r1, r0, asl #24
	mov	r5, r2, asr #2
	mov	r1, r1, asl #2
	mul	r0, r5, r1
	bl	malloc
	subs	r6, r0, #0
	beq	.L35
	ldrb	r2, [r4, #9]	@ zero_extendqisi2
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	ldrb	ip, [r4, #10]	@ zero_extendqisi2
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	orr	r3, r3, r2, asl #8
	orr	r3, r3, ip, asl #16
	orr	r3, r3, r1, asl #24
	mov	r3, r3, asl #2
	mul	r1, r5, r3
	mov	r2, #1
	mov	r3, r7
	bl	fread
	mov	r0, r7
	bl	fclose
	mov	r0, r6
	ldmfd	sp!, {r4, r5, r6, r7, r8, lr}
	bx	lr
.L34:
	ldr	r0, .L36+8
.L32:
	bl	puts
	mov	r0, r7
	bl	fclose
	mov	r0, #1
	bl	exit
.L33:
	mov	r1, r6
	ldr	r0, .L36+12
	bl	printf
	mov	r0, #1
	bl	exit
.L35:
	ldr	r0, .L36+16
	bl	puts
	mov	r0, #1
	bl	exit
.L37:
	.align	2
.L36:
	.word	.LC2
	.word	.LC4
	.word	.LC5
	.word	.LC3
	.word	.LC6
	.size	loadImage, .-loadImage
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 1080
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	ldr	r0, .L47
	sub	sp, sp, #1088
	bl	opendir
	subs	r6, r0, #0
	bne	.L43
	ldr	r0, .L47+4
	bl	perror
	mov	r0, #1
	bl	exit
.L43:
	add	r8, sp, #16
	add	sl, sp, #1072
	add	r9, sp, #1024
	sub	r8, r8, #6
	add	sl, sl, #2
	add	r9, r9, #10
.L45:
	mov	r0, r6
	bl	readdir
	subs	r4, r0, #0
	beq	.L46
	add	r7, r4, #11
	mov	r0, r7
	mov	r1, #46
	bl	strrchr
	cmp	r0, #0
	beq	.L45
	ldr	r1, .L47+8
	bl	strcmp
	cmp	r0, #0
	bne	.L45
	ldr	r3, .L47
	ldr	r2, .L47+12
	mov	r0, r8
	mov	r1, #1024
	str	r7, [sp, #0]
	bl	snprintf
	mov	r1, r8
	ldr	r0, .L47+16
	bl	printf
	mov	r1, sl
	mov	r2, r9
	mov	r0, r8
	bl	loadImage
	add	r1, sp, #1024
	ldrh	r3, [r1, #16]
	ldrh	r2, [r1, #14]
	orr	r2, r2, r3, asl #16
	add	ip, r2, #1
	mov	r1, ip, asl #1
	add	r1, r1, ip
	bic	r1, r1, #3
	add	r3, sp, #1024
	add	ip, r1, #3
	cmp	r1, #0
	ldrh	r4, [r3, #18]
	ldrh	r3, [r3, #20]
	movlt	r1, ip
	orr	r3, r4, r3, asl #16
	mov	r1, r1, asr #2
	mov	r5, r0
	bl	decodeImage
	mov	r0, r8
	mov	r1, #1024
	ldr	r2, .L47+12
	ldr	r3, .L47+20
	str	r7, [sp, #0]
	bl	snprintf
	mov	r0, r8
	mov	r1, r5
	mov	r2, sl
	mov	r3, r9
	bl	writeImage
	b	.L45
.L46:
	mov	r0, r6
	bl	closedir
	ldr	r0, .L47+24
	bl	puts
	mov	r0, r4
	bl	exit
.L48:
	.align	2
.L47:
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"wb\000"
	.space	1
.LC1:
	.ascii	"Error creating output file: %s\012\000"
.LC2:
	.ascii	"rb\000"
	.space	1
.LC3:
	.ascii	"Error opening file: %s.\012\000"
	.space	3
.LC4:
	.ascii	"Invalid BMP file.\000"
	.space	2
.LC5:
	.ascii	"Unsupported bit depth. Only 24-bit BMP is supported"
	.ascii	".\000"
	.space	3
.LC6:
	.ascii	"Failed to allocate enough memory.\000"
	.space	2
.LC7:
	.ascii	"data/encoded\000"
	.space	3
.LC8:
	.ascii	"Error opening encoded images directory\000"
	.space	1
.LC9:
	.ascii	".bmp\000"
	.space	3
.LC10:
	.ascii	"%s/%s\000"
	.space	2
.LC11:
	.ascii	"Decoding image: %s\012\000"
.LC12:
	.ascii	"data/decoded\000"
	.space	3
.LC13:
	.ascii	"Goodbye!\000"
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits
