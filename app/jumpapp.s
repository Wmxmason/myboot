    .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb

    .section .text.JumpApp,"ax",%progbits
    .global JumpApp
    .type JumpApp, %function

JumpApp:
    LDR     SP, [R0, #0]
    LDR     PC, [R0, #4]
    .size JumpApp, .-JumpApp
