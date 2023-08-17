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
	@ args = 0, pretend = 0, frame = 176
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	sub	sp, sp, #176
	str	r0, [sp, #24]
	ldr	r1, [sp, #24]
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #24]
	add	r0, r0, #11968
	add	r1, r1, #23808
	add	r3, r3, #71303168
	add	r0, r0, #32
	add	r1, r1, #192
	add	r2, r2, #35840
	add	r3, r3, #696320
	add	r2, r2, #160
	add	r3, r3, #512
	str	r0, [sp, #100]
	str	r1, [sp, #104]
	str	r4, [sp, #96]
	str	r4, [sp, #168]
	str	r0, [sp, #128]
	str	r1, [sp, #124]
	str	r2, [sp, #108]
	str	r3, [sp, #8]
	str	r2, [sp, #120]
	mov	r3, r1
	mov	r2, r0
.L3:
#APP
@ 165 "src/decode.c" 1
	Label1:
@ 0 "" 2
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
#APP
@ 178 "src/decode.c" 1
	Label2:
@ 0 "" 2
	ldr	r2, [sp, #52]
	and	r2, r2, #255
	str	r2, [sp, #16]
	ldr	r1, [sp, #36]
	ldr	r6, [sp, #16]
	and	r3, r1, #65280
	and	r2, r4, #65280
	ldr	r0, [sp, #48]
	and	r4, ip, #255
	ldr	r5, [sp, #32]
	ldr	r1, [sp, #64]
	str	r4, [sp, #20]
	str	r6, [sp, #60]
	str	r4, [sp, #92]
	ldr	r6, [sp, #128]
	ldr	r4, [sp, #80]
	mov	r5, r5, lsr #8
	and	ip, r0, #255
	mov	r1, r1, lsr #8
	mov	r0, #0
	str	r5, [sp, #44]
	str	r0, [sp, #28]
	mov	r8, r3, lsr #8
	mov	r7, r2, lsr #8
	str	r1, [sp, #76]
	and	r5, r4, #255
	ldr	fp, [sp, #168]
	str	r6, [sp, #160]
	ldr	r0, [sp, #16]
	b	.L2
.L14:
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #84]
	ldr	r2, [sp, #36]
	and	r4, r4, #255
	ldr	r6, [sp, #68]
	and	r0, r0, #255
	str	r4, [sp, #16]
	and	r3, r2, #65280
	str	r0, [sp, #20]
	and	r2, r6, #65280
	mov	r0, r4
.L2:
	ldr	r1, [sp, #52]
	ldr	r4, [sp, #84]
	and	sl, r1, #16711680
	ldr	r1, [sp, #60]
	and	r9, r4, #16711680
	add	r6, r0, r1
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #92]
	mov	r2, r2, lsr #8
	mov	r3, r3, lsr #8
	str	r2, [sp, #116]
	add	r2, r2, r7
	add	r7, r4, r0
	ldr	r4, [sp, #32]
	str	r3, [sp, #112]
	add	r3, r3, r8
	ldr	r0, [sp, #64]
	bic	r1, r4, #16711680
	mov	r3, r3, lsr #1
	orr	r1, r1, r3, asl #16
	ldr	r3, [sp, #48]
	bic	r8, r0, #16711680
	mov	r2, r2, lsr #1
	orr	r8, r8, r2, asl #16
	bic	r2, r3, #-16777216
	ldr	r3, [sp, #80]
	mov	r9, r9, lsr #16
	add	r0, fp, #23808
	add	r4, r9, r5
	mov	sl, sl, lsr #16
	bic	r5, r3, #-16777216
	add	r0, r0, #192
	mov	r6, r6, lsr #1
	mov	r7, r7, lsr #1
	str	r1, [fp, #0]
	add	ip, sl, ip
	orr	r2, r2, r6, asl #8
	orr	r5, r5, r7, asl #8
	ldr	r6, [fp, #12]
	mov	r4, r4, lsr #1
	str	r8, [r0, #0]
	add	r3, fp, #23808
	ldr	r0, [sp, #56]
	orr	r5, r5, r4, asl #24
	mov	ip, ip, lsr #1
	ldr	r4, [sp, #88]
	add	r3, r3, #204
	ldr	r3, [r3, #0]
	orr	r2, r2, ip, asl #24
	and	ip, r0, #16711680
	ldr	r0, [sp, #160]
	str	r6, [sp, #32]
	and	r6, r4, #16711680
	str	r2, [r0, #0]
	str	r3, [sp, #64]
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #36]
	mov	r6, r6, lsr #16
	str	r6, [sp, #92]
	ldr	r6, [sp, #160]
	add	r0, r2, r3, lsr #24
	add	r1, fp, #35840
	ldr	r2, [sp, #76]
	ldr	r3, [sp, #68]
	ldr	r6, [r6, #12]
	add	r1, r1, #160
	mov	ip, ip, lsr #16
	str	r5, [r1, #0]
	add	r4, r2, r3, lsr #24
	str	ip, [sp, #60]
	add	r3, fp, #35840
	ldr	ip, [sp, #36]
	ldr	r1, [sp, #68]
	ldr	r5, [sp, #60]
	str	r6, [sp, #48]
	add	r3, r3, #172
	ldr	r6, [sp, #16]
	add	r2, fp, #23808
	ldr	r3, [r3, #0]
	orr	r0, ip, r0, lsr #1
	orr	r4, r1, r4, lsr #1
	add	r2, r2, #196
	add	r1, r5, r6
	ldr	r5, [sp, #92]
	ldr	r6, [sp, #20]
	str	r3, [sp, #80]
	str	r0, [fp, #4]
	ldr	r0, [fp, #16]
	str	r4, [r2, #0]
	ldr	r4, [sp, #32]
	add	ip, r5, r6
	ldr	r5, [sp, #64]
	str	r0, [sp, #36]
	mov	r0, r4, lsr #8
	ldr	r6, [sp, #52]
	add	r3, fp, #23808
	mov	r4, r5, lsr #8
	and	r0, r0, #255
	str	r0, [sp, #44]
	and	r4, r4, #255
	mov	r1, r1, lsr #1
	add	r3, r3, #208
	ldr	r3, [r3, #0]
	ldr	r5, [sp, #84]
	orr	r1, r6, r1, asl #24
	str	r4, [sp, #76]
	ldr	r6, [sp, #40]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #44]
	mov	ip, ip, lsr #1
	orr	ip, r5, ip, asl #24
	mov	r8, r6, lsr #24
	mov	r7, r0, lsr #24
	str	r3, [sp, #68]
	orr	r0, r6, r4, asl #16
	ldr	r3, [sp, #160]
	add	r5, sp, #72
	ldmia	r5, {r5, r6}	@ phole ldm
	str	r1, [r3, #4]
	orr	r5, r5, r6, asl #16
	ldr	r1, [sp, #112]
	str	r5, [sp, #4]
	ldr	r3, [sp, #116]
	add	r2, fp, #35840
	ldr	r5, [sp, #160]
	add	r6, r8, r1
	add	r2, r2, #164
	ldr	r1, [sp, #48]
	ldr	r5, [r5, #16]
	add	r4, r7, r3
	str	ip, [r2, #0]
	add	r3, fp, #35840
	orr	r0, r0, r6, lsr #1
	and	ip, r1, #255
	ldr	r6, [sp, #4]
	ldr	r1, [sp, #80]
	add	r3, r3, #176
	ldr	r3, [r3, #0]
	add	r2, fp, #23808
	str	r0, [fp, #8]
	ldr	r0, [fp, #20]
	str	r5, [sp, #52]
	add	r2, r2, #200
	and	r5, r1, #255
	orr	r1, r6, r4, lsr #1
	str	r3, [sp, #84]
	str	r0, [sp, #40]
	add	r3, fp, #23808
	str	r1, [r2, #0]
	ldr	r4, [sp, #88]
	ldr	r1, [sp, #56]
	add	sl, sl, ip
	add	r9, r9, r5
	add	r3, r3, #212
	ldr	r3, [r3, #0]
	ldr	r6, [sp, #160]
	add	r2, fp, #35840
	mov	sl, sl, lsr #1
	mov	r9, r9, lsr #1
	orr	sl, r1, sl, asl #8
	orr	r9, r4, r9, asl #8
	add	r2, r2, #168
	str	sl, [r6, #8]
	ldr	r1, [r6, #20]
	ldr	r0, [sp, #28]
	str	r3, [sp, #72]
	str	r9, [r2, #0]
	add	r3, fp, #35840
	add	r3, r3, #180
	add	r0, r0, #1
	ldr	r3, [r3, #0]
	add	r6, r6, #12
	cmp	r0, #1000
	str	r0, [sp, #28]
	str	r1, [sp, #56]
	str	r3, [sp, #88]
	add	fp, fp, #12
	str	r6, [sp, #160]
	bne	.L14
	ldr	r1, [sp, #168]
	ldr	r2, [sp, #128]
	ldr	r3, [sp, #124]
	ldr	r4, [sp, #120]
	add	r1, r1, #47872
	ldr	r5, [sp, #8]
	add	r1, r1, #128
	add	r2, r2, #47872
	add	r3, r3, #47872
	add	r4, r4, #47872
	add	r2, r2, #128
	add	r3, r3, #128
	add	r4, r4, #128
	cmp	r1, r5
	str	r1, [sp, #168]
	str	r2, [sp, #128]
	str	r3, [sp, #124]
	str	r4, [sp, #120]
	bne	.L3
	ldr	r6, [sp, #24]
	add	r6, r6, #71303168
	add	r6, r6, #671744
	add	r6, r6, #1088
	str	r6, [sp, #12]
.L8:
	ldr	r0, [sp, #96]
	ldr	r1, [sp, #96]
	ldr	r0, [r0, #4]
	ldr	r2, [sp, #100]
	ldr	ip, [sp, #96]
	add	r3, sp, #100
	ldmia	r3, {r3, r5}	@ phole ldm
	ldr	r1, [r1, #8]
	ldr	r2, [r2, #0]
	str	r0, [sp, #136]
	ldr	r0, [sp, #108]
	ldr	ip, [ip, #0]
	ldr	r4, [r3, #8]
	str	r1, [sp, #140]
	str	r2, [sp, #144]
	ldr	r6, [r5, #4]
	ldmib	r0, {r1, r2}	@ phole ldm
	str	ip, [sp, #132]
	mov	sl, r3
	ldr	ip, [r3, #4]
	mov	r3, #0
	str	r4, [sp, #148]
	str	r6, [sp, #152]
	ldr	r4, [r5, #0]
	ldr	r9, [r5, #8]
	ldr	fp, [r0, #0]
	str	r1, [sp, #156]
	str	r2, [sp, #164]
	ldr	r8, [sp, #96]
	str	r3, [sp, #172]
.L4:
	ldr	r5, [sp, #144]
	mov	r2, r5, lsr #1
	add	r2, r2, fp, lsr #1
	and	r1, r5, #255
	and	r3, fp, #255
	and	r2, r2, #-16777216
	ldr	r6, [sp, #132]
	add	r3, r3, r1
	orr	r2, r2, r4
	orr	r2, r2, r3, lsr #1
	and	r1, r6, #16711680
	and	r3, r2, #16711680
	add	r3, r3, r1
	add	r1, r8, #23808
	ldr	r0, [sp, #136]
	add	r1, r1, #192
	str	r6, [r8, #0]
	mov	r3, r3, lsr #1
	ldr	r6, [r8, #12]
	str	r2, [r1, #0]
	ldr	r2, [sp, #144]
	and	r7, r0, #65024
	and	r3, r3, #16711680
	ldr	r0, [sp, #156]
	orr	r7, r7, ip
	ldr	r4, [sp, #140]
	orr	r3, r3, r2
	str	r3, [sp, #0]
	mov	ip, r0, lsr #1
	ldr	r3, [sp, #140]
	mov	r1, r7, lsr #1
	mov	r5, r4, lsr #1
	and	ip, ip, #8323072
	and	r1, r1, #8323072
	and	r2, r3, #255
	add	r5, r5, r9, lsr #1
	ldr	r3, [sp, #148]
	str	r6, [sp, #132]
	add	r4, r8, #23808
	add	r6, r8, #35840
	add	r1, r1, ip
	ldr	ip, [sp, #0]
	add	r6, r6, #160
	and	r0, r9, #255
	and	r5, r5, #-16777216
	add	r4, r4, #204
	ldr	r4, [r4, #0]
	add	r0, r0, r2
	str	ip, [sl, #0]
	ldr	r2, [sl, #12]
	orr	r5, r5, r3
	str	fp, [r6, #0]
	add	r3, r8, #35840
	ldr	r6, [sp, #152]
	add	r3, r3, #172
	ldr	fp, [r3, #0]
	ldr	ip, [sp, #136]
	ldr	r3, [sp, #164]
	str	r2, [sp, #144]
	add	r2, r8, #23808
	orr	r5, r5, r0, lsr #1
	orr	r1, r1, r6
	add	r2, r2, #196
	str	ip, [r8, #4]
	ldr	r6, [r8, #16]
	mov	r0, r3, lsr #1
	str	r1, [r2, #0]
	add	r3, r8, #23808
	mov	r2, r5, lsr #1
	and	r0, r0, #32512
	and	r2, r2, #32512
	add	r3, r3, #208
	ldr	r3, [r3, #0]
	add	r1, r8, #35840
	add	r2, r2, r0
	ldr	r0, [sp, #156]
	add	r1, r1, #164
	str	r7, [sl, #4]
	ldr	ip, [sl, #16]
	str	r3, [sp, #152]
	str	r0, [r1, #0]
	add	r3, r8, #35840
	add	r3, r3, #176
	ldr	r3, [r3, #0]
	str	r6, [sp, #136]
	add	r0, r8, #23808
	str	r3, [sp, #156]
	ldr	r1, [sp, #140]
	orr	r2, r2, r9
	add	r0, r0, #200
	str	r1, [r8, #8]
	ldr	r6, [r8, #20]
	add	r3, r8, #23808
	str	r2, [r0, #0]
	add	r3, r3, #212
	ldr	r9, [r3, #0]
	add	r1, r8, #35840
	ldr	r3, [sp, #164]
	add	r1, r1, #168
	str	r5, [sl, #8]
	ldr	r2, [sl, #20]
	ldr	r0, [sp, #172]
	str	r3, [r1, #0]
	add	r3, r8, #35840
	add	r3, r3, #180
	add	r0, r0, #1
	ldr	r3, [r3, #0]
	cmp	r0, #1000
	str	r6, [sp, #140]
	str	r0, [sp, #172]
	str	r2, [sp, #148]
	str	r3, [sp, #164]
	add	r8, r8, #12
	add	sl, sl, #12
	bne	.L4
	add	r4, sp, #96
	ldmia	r4, {r4, r5, r6, ip}	@ phole ldm
	add	r4, r4, #23808
	ldr	r0, [sp, #12]
	add	r4, r4, #192
	add	r5, r5, #23808
	add	r6, r6, #23808
	add	ip, ip, #23808
	add	r5, r5, #192
	add	r6, r6, #192
	add	ip, ip, #192
	cmp	r4, r0
	str	r4, [sp, #96]
	str	r5, [sp, #100]
	str	r6, [sp, #104]
	str	ip, [sp, #108]
	bne	.L8
	add	sp, sp, #176
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	bx	lr
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
	ldr	r1, .L19
	mov	r6, r2
	mov	r4, r3
	mov	r7, r0
	bl	fopen
	subs	r5, r0, #0
	beq	.L18
	mov	r3, r5
	mov	r0, r6
	mov	r1, #14
	mov	r2, #1
	bl	fwrite
	mov	r3, r5
	mov	r0, r4
	mov	r1, #40
	mov	r2, #1
	bl	fwrite
	ldrb	r3, [r4, #9]	@ zero_extendqisi2
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
	ldrb	r2, [r4, #11]	@ zero_extendqisi2
	orr	r1, r1, r3, asl #8
	orr	r1, r1, r0, asl #16
	orr	r1, r1, r2, asl #24
	mov	r0, r1, asl #7
	mov	r1, r1, asl #5
	add	r1, r1, r0
	mov	r3, r1, asl #2
	add	r1, r1, r3
	mov	r2, r1, asl #4
	rsb	r1, r1, r2
	mov	r3, r5
	mov	r2, #1
	mov	r0, r8
	bl	fwrite
	mov	r0, r8
	bl	free
	mov	r0, r5
	bl	fclose
	ldmfd	sp!, {r4, r5, r6, r7, r8, lr}
	bx	lr
.L18:
	mov	r1, r7
	ldr	r0, .L19+4
	bl	printf
	mov	r0, r8
	bl	free
	mov	r0, #1
	bl	exit
.L20:
	.align	2
.L19:
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
	ldr	r1, .L31
	mov	r4, r2
	mov	r6, r0
	bl	fopen
	subs	r7, r0, #0
	beq	.L28
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
	ldrne	r0, .L31+4
	bne	.L27
	ldrb	r3, [r4, #14]	@ zero_extendqisi2
	ldrb	r2, [r4, #15]	@ zero_extendqisi2
	orr	r3, r3, r2, asl #8
	cmp	r3, #24
	bne	.L29
	ldrb	r3, [r4, #9]	@ zero_extendqisi2
	ldrb	r0, [r4, #8]	@ zero_extendqisi2
	ldrb	r1, [r4, #10]	@ zero_extendqisi2
	ldrb	r2, [r4, #11]	@ zero_extendqisi2
	orr	r0, r0, r3, asl #8
	orr	r0, r0, r1, asl #16
	orr	r0, r0, r2, asl #24
	mov	r1, r0, asl #7
	mov	r0, r0, asl #5
	add	r0, r0, r1
	mov	r3, r0, asl #2
	add	r0, r0, r3
	mov	r2, r0, asl #4
	rsb	r0, r0, r2
	bl	malloc
	subs	r5, r0, #0
	beq	.L30
	ldrb	r3, [r4, #9]	@ zero_extendqisi2
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	ldrb	ip, [r4, #10]	@ zero_extendqisi2
	ldrb	r2, [r4, #11]	@ zero_extendqisi2
	orr	r1, r1, r3, asl #8
	orr	r1, r1, ip, asl #16
	orr	r1, r1, r2, asl #24
	mov	ip, r1, asl #7
	mov	r1, r1, asl #5
	add	r1, r1, ip
	mov	r3, r1, asl #2
	add	r1, r1, r3
	mov	r2, r1, asl #4
	rsb	r1, r1, r2
	mov	r3, r7
	mov	r2, #1
	bl	fread
	mov	r0, r7
	bl	fclose
	mov	r0, r5
	ldmfd	sp!, {r4, r5, r6, r7, r8, lr}
	bx	lr
.L29:
	ldr	r0, .L31+8
.L27:
	bl	puts
	mov	r0, r7
	bl	fclose
	mov	r0, #1
	bl	exit
.L28:
	mov	r1, r6
	ldr	r0, .L31+12
	bl	printf
	mov	r0, #1
	bl	exit
.L30:
	ldr	r0, .L31+16
	bl	puts
	mov	r0, #1
	bl	exit
.L32:
	.align	2
.L31:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, sl, lr}
	sub	sp, sp, #1088
	sub	sp, sp, #4
	ldr	r0, .L42
	bl	opendir
	subs	r6, r0, #0
	bne	.L38
	ldr	r0, .L42+4
	bl	perror
	mov	r0, #1
	bl	exit
.L38:
	add	r7, sp, #16
	add	r8, sp, #1072
	add	sl, sp, #1024
	sub	r7, r7, #6
	add	r8, r8, #2
	add	sl, sl, #10
.L40:
	mov	r0, r6
	bl	readdir
	subs	r4, r0, #0
	beq	.L41
	add	r5, r4, #11
	mov	r0, r5
	mov	r1, #46
	bl	strrchr
	cmp	r0, #0
	beq	.L40
	ldr	r1, .L42+8
	bl	strcmp
	cmp	r0, #0
	bne	.L40
	ldr	r3, .L42
	ldr	r2, .L42+12
	mov	r0, r7
	mov	r1, #1024
	str	r5, [sp, #0]
	bl	snprintf
	mov	r1, r7
	ldr	r0, .L42+16
	bl	printf
	mov	r1, r8
	mov	r2, sl
	mov	r0, r7
	bl	loadImage
	mov	r4, r0
	bl	decodeImage
	mov	r0, r7
	mov	r1, #1024
	ldr	r2, .L42+12
	ldr	r3, .L42+20
	str	r5, [sp, #0]
	bl	snprintf
	mov	r0, r7
	mov	r1, r4
	mov	r2, r8
	mov	r3, sl
	bl	writeImage
	b	.L40
.L41:
	mov	r0, r6
	bl	closedir
	ldr	r0, .L42+24
	bl	puts
	mov	r0, r4
	bl	exit
.L43:
	.align	2
.L42:
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
