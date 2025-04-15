.syntax unified
.cpu cortex-m3
.thumb

.global _estack ;;show stack pointer to linker
.global Reset_Handler ;; show startup function

.section .isr_vector, "a", %progbits

.word _estack 
.word Reset_Handler
.word Default_Handler ;;handler for NMI
.word Default_Handler ;;handler for Hardfault
.word Default_Handler ;; handler for memory management  
.word Default_Handler ;;hander for bus fault
.word Default_Handler ;; Usage Fault
.word 0               ;; reserved memory zeroed
.word 0
.word 0 
.word 0
.word Default_Handler ;; SVCall
.word Default_Handler ;;debug monitor
.word 0
.word Default_Handler        /* PendSV */
.word Default_Handler        /* SysTick */



.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  /* Copy .data section from Flash to RAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
1:
  cmp r0, r1
  ittt lt
  ldrlt r3, [r2], #4
  strlt r3, [r0], #4
  blt 1b

  /* Zero initialize .bss section */
  ldr r0, =_sbss
  ldr r1, =_ebss
2:
  cmp r0, r1
  it lt
  movlt r2, #0
  strlt r2, [r0], #4
  blt 2b

  
  bl main

  /* Infinite loop if main returns */
  b .

/* Default interrupt handlers */
.weak Default_Handler
.type Default_Handler, %function
Default_Handler:
  b .