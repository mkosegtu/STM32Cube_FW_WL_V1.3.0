;********************************************************************************
;* File Name          : startup_stm32wl55xx_cm4.s
;* Author             : MCD Application Team
;* Description        : STM32WL55xx devices vector table for MDK-ARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM4 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   
;********************************************************************************
;* @attention
;*
;* Copyright (c) 2020-2021 STMicroelectronics.
;* All rights reserved.
;*
;* This software is licensed under terms that can be found in the LICENSE file
;* in the root directory of this software component.
;* If no LICENSE file comes with this software, it is provided AS-IS.
;*
;********************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000800

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                      ; Top of Stack
                DCD     Reset_Handler                     ; Reset Handler
                DCD     NMI_Handler                       ; NMI Handler
                DCD     HardFault_Handler                 ; Hard Fault Handler
                DCD     MemManage_Handler                 ; MPU Fault Handler
                DCD     BusFault_Handler                  ; Bus Fault Handler
                DCD     UsageFault_Handler                ; Usage Fault Handler
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     SVC_Handler                       ; SVCall Handler
                DCD     DebugMon_Handler                  ; Debug Monitor Handler
                DCD     0                                 ; Reserved
                DCD     PendSV_Handler                    ; PendSV Handler
                DCD     SysTick_Handler                   ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler                   ; Window WatchDog
                DCD     PVD_PVM_IRQHandler                ; PVD and PVM detector
                DCD     TAMP_STAMP_LSECSS_SSRU_IRQHandler ; RTC Tamper, RTC TimeStamp, LSECSS and RTC SSR Underflow Interrupts
                DCD     RTC_WKUP_IRQHandler               ; RTC Wakeup Interrupt
                DCD     FLASH_IRQHandler                  ; FLASH global Interrupt
                DCD     RCC_IRQHandler                    ; RCC Interrupt
                DCD     EXTI0_IRQHandler                  ; EXTI Line 0 Interrupt
                DCD     EXTI1_IRQHandler                  ; EXTI Line 1 Interrupt
                DCD     EXTI2_IRQHandler                  ; EXTI Line 2 Interrupt
                DCD     EXTI3_IRQHandler                  ; EXTI Line 3 Interrupt
                DCD     EXTI4_IRQHandler                  ; EXTI Line 4 Interrupt
                DCD     DMA1_Channel1_IRQHandler          ; DMA1 Channel 1 Interrupt
                DCD     DMA1_Channel2_IRQHandler          ; DMA1 Channel 2 Interrupt
                DCD     DMA1_Channel3_IRQHandler          ; DMA1 Channel 3 Interrupt
                DCD     DMA1_Channel4_IRQHandler          ; DMA1 Channel 4 Interrupt
                DCD     DMA1_Channel5_IRQHandler          ; DMA1 Channel 5 Interrupt
                DCD     DMA1_Channel6_IRQHandler          ; DMA1 Channel 6 Interrupt
                DCD     DMA1_Channel7_IRQHandler          ; DMA1 Channel 7 Interrupt
                DCD     ADC_IRQHandler                    ; ADC Interrupt
                DCD     DAC_IRQHandler                    ; DAC Interrupt
                DCD     C2SEV_PWR_C2H_IRQHandler          ; CPU M0+ SEV and PWR CPU M0+ HOLD wakeup Interrupt
                DCD     COMP_IRQHandler                   ; COMP1 and COMP2 Interrupts
                DCD     EXTI9_5_IRQHandler                ; EXTI Lines [9:5] Interrupt
                DCD     TIM1_BRK_IRQHandler               ; TIM1 Break Interrupt
                DCD     TIM1_UP_IRQHandler                ; TIM1 Update Interrupts
                DCD     TIM1_TRG_COM_IRQHandler           ; TIM1 Trigger and Communication Interrupts
                DCD     TIM1_CC_IRQHandler                ; TIM1 Capture Compare Interrupt
                DCD     TIM2_IRQHandler                   ; TIM2 Global Interrupt
                DCD     TIM16_IRQHandler                  ; TIM16 Global Interrupt
                DCD     TIM17_IRQHandler                  ; TIM17 Global Interrupt
                DCD     I2C1_EV_IRQHandler                ; I2C1 Event Interrupt
                DCD     I2C1_ER_IRQHandler                ; I2C1 Error Interrupt
                DCD     I2C2_EV_IRQHandler                ; I2C2 Event Interrupt
                DCD     I2C2_ER_IRQHandler                ; I2C2 Error Interrupt
                DCD     SPI1_IRQHandler                   ; SPI1 Interrupt
                DCD     SPI2_IRQHandler                   ; SPI2 Interrupt
                DCD     USART1_IRQHandler                 ; USART1 Interrupt
                DCD     USART2_IRQHandler                 ; USART2 Interrupt
                DCD     LPUART1_IRQHandler                ; LPUART1 Interrupt
                DCD     LPTIM1_IRQHandler                 ; LPTIM1 Interrupt
                DCD     LPTIM2_IRQHandler                 ; LPTIM2 Interrupt
                DCD     EXTI15_10_IRQHandler              ; EXTI Lines1[15:10 ]Interrupts
                DCD     RTC_Alarm_IRQHandler              ; RTC Alarms (A and B) Interrupt
                DCD     LPTIM3_IRQHandler                 ; LPTIM3 Interrupt
                DCD     SUBGHZSPI_IRQHandler              ; SUBGHZSPI Interrupt
                DCD     IPCC_C1_RX_IRQHandler             ; IPCC CPU1 RX occupied interrupt
                DCD     IPCC_C1_TX_IRQHandler             ; IPCC CPU1 RX free interrupt
                DCD     HSEM_IRQHandler                   ; HSEM0 Interrupt
                DCD     I2C3_EV_IRQHandler                ; I2C3 Event Interrupt
                DCD     I2C3_ER_IRQHandler                ; I2C3 Error Interrupt
                DCD     SUBGHZ_Radio_IRQHandler           ; SUBGHZ Radio Interrupt
                DCD     AES_IRQHandler                    ; AES Interrupt
                DCD     RNG_IRQHandler                    ; RNG1 Interrupt
                DCD     PKA_IRQHandler                    ; PKA Interrupt
                DCD     DMA2_Channel1_IRQHandler          ; DMA2 Channel 1 Interrupt
                DCD     DMA2_Channel2_IRQHandler          ; DMA2 Channel 2 Interrupt
                DCD     DMA2_Channel3_IRQHandler          ; DMA2 Channel 3 Interrupt
                DCD     DMA2_Channel4_IRQHandler          ; DMA2 Channel 4 Interrupt
                DCD     DMA2_Channel5_IRQHandler          ; DMA2 Channel 5 Interrupt
                DCD     DMA2_Channel6_IRQHandler          ; DMA2 Channel 6 Interrupt
                DCD     DMA2_Channel7_IRQHandler          ; DMA2 Channel 7 Interrupt
                DCD     DMAMUX1_OVR_IRQHandler            ; DMAMUX overrun Interrupt

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler                 [WEAK]
        IMPORT  SystemInit
        IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                    [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler              [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler              [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler               [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler             [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                    [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                 [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler                   [WEAK]
                EXPORT  PVD_PVM_IRQHandler                [WEAK]
                EXPORT  TAMP_STAMP_LSECSS_SSRU_IRQHandler [WEAK]
                EXPORT  RTC_WKUP_IRQHandler               [WEAK]
                EXPORT  FLASH_IRQHandler                  [WEAK]
                EXPORT  RCC_IRQHandler                    [WEAK]
                EXPORT  EXTI0_IRQHandler                  [WEAK]
                EXPORT  EXTI1_IRQHandler                  [WEAK]
                EXPORT  EXTI2_IRQHandler                  [WEAK]
                EXPORT  EXTI3_IRQHandler                  [WEAK]
                EXPORT  EXTI4_IRQHandler                  [WEAK]
                EXPORT  DMA1_Channel1_IRQHandler          [WEAK]
                EXPORT  DMA1_Channel2_IRQHandler          [WEAK]
                EXPORT  DMA1_Channel3_IRQHandler          [WEAK]
                EXPORT  DMA1_Channel4_IRQHandler          [WEAK]
                EXPORT  DMA1_Channel5_IRQHandler          [WEAK]
                EXPORT  DMA1_Channel6_IRQHandler          [WEAK]
                EXPORT  DMA1_Channel7_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler                    [WEAK]
                EXPORT  DAC_IRQHandler                    [WEAK]
                EXPORT  C2SEV_PWR_C2H_IRQHandler          [WEAK]
                EXPORT  COMP_IRQHandler                   [WEAK]
                EXPORT  EXTI9_5_IRQHandler                [WEAK]
                EXPORT  TIM1_BRK_IRQHandler               [WEAK]
                EXPORT  TIM1_UP_IRQHandler                [WEAK]
                EXPORT  TIM1_TRG_COM_IRQHandler           [WEAK]
                EXPORT  TIM1_CC_IRQHandler                [WEAK]
                EXPORT  TIM2_IRQHandler                   [WEAK]
                EXPORT  TIM16_IRQHandler                  [WEAK]
                EXPORT  TIM17_IRQHandler                  [WEAK]
                EXPORT  I2C1_EV_IRQHandler                [WEAK]
                EXPORT  I2C1_ER_IRQHandler                [WEAK]
                EXPORT  I2C2_EV_IRQHandler                [WEAK]
                EXPORT  I2C2_ER_IRQHandler                [WEAK]
                EXPORT  SPI1_IRQHandler                   [WEAK]
                EXPORT  SPI2_IRQHandler                   [WEAK]
                EXPORT  USART1_IRQHandler                 [WEAK]
                EXPORT  USART2_IRQHandler                 [WEAK]
                EXPORT  LPUART1_IRQHandler                [WEAK]
                EXPORT  LPTIM1_IRQHandler                 [WEAK]
                EXPORT  LPTIM2_IRQHandler                 [WEAK]
                EXPORT  EXTI15_10_IRQHandler              [WEAK]
                EXPORT  RTC_Alarm_IRQHandler              [WEAK]
                EXPORT  LPTIM3_IRQHandler                 [WEAK]
                EXPORT  SUBGHZSPI_IRQHandler              [WEAK]
                EXPORT  IPCC_C1_RX_IRQHandler             [WEAK]
                EXPORT  IPCC_C1_TX_IRQHandler             [WEAK]
                EXPORT  HSEM_IRQHandler                   [WEAK]
                EXPORT  I2C3_EV_IRQHandler                [WEAK]
                EXPORT  I2C3_ER_IRQHandler                [WEAK]
                EXPORT  SUBGHZ_Radio_IRQHandler           [WEAK]
                EXPORT  AES_IRQHandler                    [WEAK]
                EXPORT  RNG_IRQHandler                    [WEAK]
                EXPORT  PKA_IRQHandler                    [WEAK]
                EXPORT  DMA2_Channel1_IRQHandler          [WEAK]
                EXPORT  DMA2_Channel2_IRQHandler          [WEAK]
                EXPORT  DMA2_Channel3_IRQHandler          [WEAK]
                EXPORT  DMA2_Channel4_IRQHandler          [WEAK]
                EXPORT  DMA2_Channel5_IRQHandler          [WEAK]
                EXPORT  DMA2_Channel6_IRQHandler          [WEAK]
                EXPORT  DMA2_Channel7_IRQHandler          [WEAK]
                EXPORT  DMAMUX1_OVR_IRQHandler            [WEAK]

WWDG_IRQHandler
PVD_PVM_IRQHandler
TAMP_STAMP_LSECSS_SSRU_IRQHandler
RTC_WKUP_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Channel1_IRQHandler
DMA1_Channel2_IRQHandler
DMA1_Channel3_IRQHandler
DMA1_Channel4_IRQHandler
DMA1_Channel5_IRQHandler
DMA1_Channel6_IRQHandler
DMA1_Channel7_IRQHandler
ADC_IRQHandler
DAC_IRQHandler
C2SEV_PWR_C2H_IRQHandler
COMP_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_IRQHandler
TIM1_UP_IRQHandler
TIM1_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM16_IRQHandler
TIM17_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler                
USART2_IRQHandler
LPUART1_IRQHandler
LPTIM1_IRQHandler
LPTIM2_IRQHandler
EXTI15_10_IRQHandler
RTC_Alarm_IRQHandler
LPTIM3_IRQHandler
SUBGHZSPI_IRQHandler
IPCC_C1_RX_IRQHandler
IPCC_C1_TX_IRQHandler
HSEM_IRQHandler
I2C3_EV_IRQHandler
I2C3_ER_IRQHandler
SUBGHZ_Radio_IRQHandler
AES_IRQHandler
RNG_IRQHandler
PKA_IRQHandler
DMA2_Channel1_IRQHandler
DMA2_Channel2_IRQHandler
DMA2_Channel3_IRQHandler
DMA2_Channel4_IRQHandler
DMA2_Channel5_IRQHandler
DMA2_Channel6_IRQHandler
DMA2_Channel7_IRQHandler
DMAMUX1_OVR_IRQHandler

                B       .

                ENDP

                ALIGN

;********************************************************************************
; User Stack and Heap initialization
;********************************************************************************
                 IF      :DEF:__MICROLIB

                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit

                 ELSE

                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap

__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

;*******************************************************************************
; Element for aligning the Firmware binary size on the AES block size (16 bytes)
;*******************************************************************************
  AREA ALIGNTOAESBLOCK, DATA, READONLY, MERGE=1, STRINGS
				 DCB	"0123456789ABCDE",0

                 END
