#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "touch.h"
#include "stm32f10x_exti.h"

#define ADC1_DR_Address ((uint32_t)0x4001244C)
#define ARRAYSIZE 4*4
#define Cycles ADC_SampleTime_239Cycles5
volatile uint16_t ADC_values[ARRAYSIZE];
volatile char Send_Data[4];

int send_state = 0;

void RCC_Configure() {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        //USART  Clock Enable
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}
void GPIO_Configure() {

        GPIO_InitTypeDef GPIO_InitStructure_TX,
                                      GPIO_InitStructure_RX;
        //USART 1,2 TX
        GPIO_InitStructure_TX.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure_TX.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_2);
        GPIO_InitStructure_TX.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure_TX);
        //USART 1,2 RX
        GPIO_InitStructure_RX.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure_RX.GPIO_Pin = (GPIO_Pin_10 | GPIO_Pin_3);
        GPIO_InitStructure_RX.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure_RX);
        GPIOD->CRH &= ~(GPIO_CRH_CNF11);
        GPIOD->CRH |= GPIO_CRH_CNF11_1;
}

void USART_Configure() {
        USART_InitTypeDef USART_InitStructure;
        
        //USART Configuration
        USART_InitStructure.USART_BaudRate = 9600;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = (USART_Mode_Rx | USART_Mode_Tx);
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_Init(USART1, &USART_InitStructure);
        USART_Init(USART2, &USART_InitStructure);
        
        USART_Cmd(USART1, ENABLE);
        USART_Cmd(USART2, ENABLE);
   
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void NVIC_Configure() {
   NVIC_InitTypeDef NVIC_InitStructure_1,
                                     NVIC_InitStructure_2;
        
        //USART1
        NVIC_InitStructure_1.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure_1.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure_1.NVIC_IRQChannelPreemptionPriority = 0x00;
        NVIC_InitStructure_1.NVIC_IRQChannelSubPriority = 0x00;
        NVIC_Init(&NVIC_InitStructure_1);
        
        //USART2
        NVIC_InitStructure_2.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure_2.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure_2.NVIC_IRQChannelPreemptionPriority = 0x00;
        NVIC_InitStructure_2.NVIC_IRQChannelSubPriority = 0x00;
        NVIC_Init(&NVIC_InitStructure_2);
}


void USART1_IRQHandler(void){
        if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
                USART_SendData(USART2,(char)USART_ReceiveData(USART1));       
                USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        }
}

void USART2_IRQHandler(void){
        if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
              USART_SendData(USART1,(char)USART_ReceiveData(USART2));
              USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
}

void ADCInit(void){
    //--Enable ADC1 and GPIOA--
    GPIO_InitTypeDef GPIO_InitStructure; //Variable used to setup the GPIO pins
    //==Configure ADC pins (PA0 -> Channel 0 to PA7 -> Channel 7) as analog inputs==
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure, if not it can cause issues...
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
 
    ADC_InitTypeDef ADC_InitStructure;
    //ADC1 configuration
 
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //We will convert multiple channels
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    //select continuous conversion mode
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    //select no external triggering
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    //right 12-bit data alignment in ADC data register
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //8 channels conversion
    ADC_InitStructure.ADC_NbrOfChannel = 4;
    //load structure values to control and status registers
    ADC_Init(ADC1, &ADC_InitStructure);
    //wake up temperature sensor
    //ADC_TempSensorVrefintCmd(ENABLE);
    //configure each channel
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, Cycles);
    //Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    //enable DMA for ADC
    ADC_DMACmd(ADC1, ENABLE);
    //Enable ADC1 reset calibration register
    ADC_ResetCalibration(ADC1);
    //Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));
    //Start ADC1 calibration
    ADC_StartCalibration(ADC1);
    //Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
}
void DMAInit(void){
    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //enable DMA1 clock
    //create DMA structure
    //reset DMA1 channe1 to default values;
    DMA_DeInit(DMA1_Channel1);
    //channel will be used for memory to memory transfer
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    //setting normal mode (non circular)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;
    //medium priority
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //source and destination data size word=32bit
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //automatic memory destination increment enable.
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //source address increment disable
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //Location assigned to peripheral register will be source
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //chunk of data to be transfered
    DMA_InitStructure.DMA_BufferSize = ARRAYSIZE;
    //source and destination start addresses
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_values;
    //send values to DMA registers
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    // Enable DMA1 Channel Transfer Complete interrupt
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1
        
    //Enable DMA1 channel IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
 
void DMA1_Channel1_IRQHandler(void){
  //Test on DMA1 Channel1 Transfer Complete interrupt
  if(DMA_GetITStatus(DMA1_IT_TC1))
  {
    DMA_ClearITPendingBit(DMA1_IT_TC1 | DMA1_IT_GL1);

  }
}
void init_adc(){
  ADCInit();
  DMAInit();
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void mapping(){
  if(ADC_values[0]<1000){
    Send_Data[0]='0';
  }
  else{
    Send_Data[0]='1';
  }
  if(ADC_values[0]<5000){
    Send_Data[1]='A';
  }
  else{
    Send_Data[1]='B';
  }
  
  
  
}
void delay(){
  int i;
  for(i=0; i<100000;i++){
  }
}
int main() {

   SystemInit();
   RCC_Configure();
   GPIO_Configure();
   USART_Configure();
   NVIC_Configure();
   init_adc();
   LCD_Init(); 
   LCD_Clear(WHITE);
  
   GPIOD->BSRR |= GPIO_BRR_BR11;
   while(1){
    LCD_ShowNum(120, 160, ADC_values[0], 4, BLACK, WHITE);
    LCD_ShowNum(120, 140, ADC_values[1], 4, BLACK, WHITE);
    if(((GPIOD->IDR) & 0x800) == 0){
      send_state = 1;
    }
    if(send_state ==1){
      mapping();
      USART_SendData(USART2,Send_Data[0]);
      delay();
    } 
   }
}