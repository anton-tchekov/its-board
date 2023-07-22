.text
.word     0x20004000
.word     main
.word     NMI_Handler
.word     HardFault_Handler
.word     MemManage_Handler
.word     BusFault_Handler
.word     UsageFault_Handler
.word     0
.word     0
.word     0
.word     0
.word     SVC_Handler
.word     DebugMon_Handler
.word     0
.word     PendSV_Handler
.word     SysTick_Handler
.word     WWDG_IRQHandler
.word     PVD_IRQHandler
.word     TAMP_STAMP_IRQHandler
.word     RTC_WKUP_IRQHandler
.word     FLASH_IRQHandler
.word     RCC_IRQHandler
.word     EXTI0_IRQHandler
.word     EXTI1_IRQHandler
.word     EXTI2_IRQHandler
.word     EXTI3_IRQHandler
.word     EXTI4_IRQHandler
.word     DMA1_Stream0_IRQHandler
.word     DMA1_Stream1_IRQHandler
.word     DMA1_Stream2_IRQHandler
.word     DMA1_Stream3_IRQHandler
.word     DMA1_Stream4_IRQHandler
.word     DMA1_Stream5_IRQHandler
.word     DMA1_Stream6_IRQHandler
.word     ADC_IRQHandler
.word     CAN1_TX_IRQHandler
.word     CAN1_RX0_IRQHandler
.word     CAN1_RX1_IRQHandler
.word     CAN1_SCE_IRQHandler
.word     EXTI9_5_IRQHandler
.word     TIM1_BRK_TIM9_IRQHandler
.word     TIM1_UP_TIM10_IRQHandler
.word     TIM1_TRG_COM_TIM11_IRQHandler
.word     TIM1_CC_IRQHandler
.word     TIM2_IRQHandler
.word     TIM3_IRQHandler
.word     TIM4_IRQHandler
.word     I2C1_EV_IRQHandler
.word     I2C1_ER_IRQHandler
.word     I2C2_EV_IRQHandler
.word     I2C2_ER_IRQHandler
.word     SPI1_IRQHandler
.word     SPI2_IRQHandler
.word     USART1_IRQHandler
.word     USART2_IRQHandler
.word     USART3_IRQHandler
.word     EXTI15_10_IRQHandler
.word     RTC_Alarm_IRQHandler
.word     OTG_FS_WKUP_IRQHandler
.word     TIM8_BRK_TIM12_IRQHandler
.word     TIM8_UP_TIM13_IRQHandler
.word     TIM8_TRG_COM_TIM14_IRQHandler
.word     TIM8_CC_IRQHandler
.word     DMA1_Stream7_IRQHandler
.word     FMC_IRQHandler
.word     SDIO_IRQHandler
.word     TIM5_IRQHandler
.word     SPI3_IRQHandler
.word     UART4_IRQHandler
.word     UART5_IRQHandler
.word     TIM6_DAC_IRQHandler
.word     TIM7_IRQHandler
.word     DMA2_Stream0_IRQHandler
.word     DMA2_Stream1_IRQHandler
.word     DMA2_Stream2_IRQHandler
.word     DMA2_Stream3_IRQHandler
.word     DMA2_Stream4_IRQHandler
.word     ETH_IRQHandler
.word     ETH_WKUP_IRQHandler
.word     CAN2_TX_IRQHandler
.word     CAN2_RX0_IRQHandler
.word     CAN2_RX1_IRQHandler
.word     CAN2_SCE_IRQHandler
.word     OTG_FS_IRQHandler
.word     DMA2_Stream5_IRQHandler
.word     DMA2_Stream6_IRQHandler
.word     DMA2_Stream7_IRQHandler
.word     USART6_IRQHandler
.word     I2C3_EV_IRQHandler
.word     I2C3_ER_IRQHandler
.word     OTG_HS_EP1_OUT_IRQHandler
.word     OTG_HS_EP1_IN_IRQHandler
.word     OTG_HS_WKUP_IRQHandler
.word     OTG_HS_IRQHandler
.word     DCMI_IRQHandler
.word     0
.word     HASH_RNG_IRQHandler
.word     FPU_IRQHandler
.word     UART7_IRQHandler
.word     UART8_IRQHandler
.word     SPI4_IRQHandler
.word     SPI5_IRQHandler
.word     SPI6_IRQHandler
.word     SAI1_IRQHandler
.word     LTDC_IRQHandler
.word     LTDC_ER_IRQHandler
.word     DMA2D_IRQHandler

NMI_Handler:
HardFault_Handler:
MemManage_Handler:
BusFault_Handler:
UsageFault_Handler:
SVC_Handler:
DebugMon_Handler:
PendSV_Handler:
SysTick_Handler:
Default_Handler:
WWDG_IRQHandler:
PVD_IRQHandler:
TAMP_STAMP_IRQHandler:
RTC_WKUP_IRQHandler:
FLASH_IRQHandler:
RCC_IRQHandler:
EXTI0_IRQHandler:
EXTI1_IRQHandler:
EXTI2_IRQHandler:
EXTI3_IRQHandler:
EXTI4_IRQHandler:
DMA1_Stream0_IRQHandler:
DMA1_Stream1_IRQHandler:
DMA1_Stream2_IRQHandler:
DMA1_Stream3_IRQHandler:
DMA1_Stream4_IRQHandler:
DMA1_Stream5_IRQHandler:
DMA1_Stream6_IRQHandler:
ADC_IRQHandler:
CAN1_TX_IRQHandler:
CAN1_RX0_IRQHandler:
CAN1_RX1_IRQHandler:
CAN1_SCE_IRQHandler:
EXTI9_5_IRQHandler:
TIM1_BRK_TIM9_IRQHandler:
TIM1_UP_TIM10_IRQHandler:
TIM1_TRG_COM_TIM11_IRQHandler:
TIM1_CC_IRQHandler:
TIM2_IRQHandler:
TIM3_IRQHandler:
TIM4_IRQHandler:
I2C1_EV_IRQHandler:
I2C1_ER_IRQHandler:
I2C2_EV_IRQHandler:
I2C2_ER_IRQHandler:
SPI1_IRQHandler:
SPI2_IRQHandler:
USART1_IRQHandler:
USART2_IRQHandler:
USART3_IRQHandler:
EXTI15_10_IRQHandler:
RTC_Alarm_IRQHandler:
OTG_FS_WKUP_IRQHandler:
TIM8_BRK_TIM12_IRQHandler:
TIM8_UP_TIM13_IRQHandler:
TIM8_TRG_COM_TIM14_IRQHandler:
TIM8_CC_IRQHandler:
DMA1_Stream7_IRQHandler:
FMC_IRQHandler:
SDIO_IRQHandler:
TIM5_IRQHandler:
SPI3_IRQHandler:
UART4_IRQHandler:
UART5_IRQHandler:
TIM6_DAC_IRQHandler:
TIM7_IRQHandler:
DMA2_Stream0_IRQHandler:
DMA2_Stream1_IRQHandler:
DMA2_Stream2_IRQHandler:
DMA2_Stream3_IRQHandler:
DMA2_Stream4_IRQHandler:
ETH_IRQHandler:
ETH_WKUP_IRQHandler:
CAN2_TX_IRQHandler:
CAN2_RX0_IRQHandler:
CAN2_RX1_IRQHandler:
CAN2_SCE_IRQHandler:
OTG_FS_IRQHandler:
DMA2_Stream5_IRQHandler:
DMA2_Stream6_IRQHandler:
DMA2_Stream7_IRQHandler:
USART6_IRQHandler:
I2C3_EV_IRQHandler:
I2C3_ER_IRQHandler:
OTG_HS_EP1_OUT_IRQHandler:
OTG_HS_EP1_IN_IRQHandler:
OTG_HS_WKUP_IRQHandler:
OTG_HS_IRQHandler:
DCMI_IRQHandler:
HASH_RNG_IRQHandler:
FPU_IRQHandler:
UART7_IRQHandler:
UART8_IRQHandler:
SPI4_IRQHandler:
SPI5_IRQHandler:
SPI6_IRQHandler:
SAI1_IRQHandler:
LTDC_IRQHandler:
LTDC_ER_IRQHandler:
DMA2D_IRQHandler:
	B .
