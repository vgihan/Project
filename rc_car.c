#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "core_cm3.h"

char a = '\0';
char speed = '0';
char dir1 = 'A';
char dir2 = 'C';
uint16_t speed1_value = 0;
uint16_t speed2_value = 0;

volatile unsigned int Timer2_Counter=0;
volatile unsigned int Timer3_Counter=0;
void RCC_Configure() {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        //USART  Clock Enable
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}
void GPIO_Configure() {

        GPIO_InitTypeDef GPIO_InitStructure_TX, GPIO_InitStructure_RX;
        GPIO_InitTypeDef GPIO_InitStructure_LED;
        
        GPIO_InitStructure_LED.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure_LED.GPIO_Pin = GPIO_Pin_13;
        GPIO_InitStructure_LED.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure_LED);
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
}

void USART_Configure() {
        USART_InitTypeDef USART_InitStructure;
        [
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
        if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
                a= (char)USART_ReceiveData(USART2); 
      USART_SendData(USART1,(char)USART_ReceiveData(USART2));
                USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
        if(a=='0' || a == '1' || a=='2' || a=='3' || a == '4' || a == '5'){
          speed = a;
          GPIO_SetBits(GPIOC, GPIO_Pin_13);
        }
        else if(a=='A' || a=='B'){
          dir1 = a;
        }
        else if(a=='C' || a=='D'){
          dir2 = a;
        }
}

void init_port() {
  GPIO_InitTypeDef PORTA;
  GPIO_InitTypeDef PORTB;
  GPIO_InitTypeDef PORTC;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
  
  PORTA.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  PORTA.GPIO_Mode = GPIO_Mode_AF_PP;
  PORTA.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &PORTA);
  
  PORTB.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4);
  PORTB.GPIO_Mode = GPIO_Mode_Out_PP;
  PORTB.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &PORTB);
  
  PORTC.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4);
  PORTC.GPIO_Mode = GPIO_Mode_Out_PP;
  PORTC.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &PORTC);
 
} 

void TIM2_IRQHandler(void) {
  if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // Clear the interrupt flag 
    Timer2_Counter++;
    GPIO_ResetBits(GPIOB, GPIO_Pin_0); 
  } 
  if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET) {
    TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
    GPIO_SetBits(GPIOB, GPIO_Pin_0); 
  } 
} 

void TIM3_IRQHandler(void) {
  if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // Clear the interrupt flag 
    Timer3_Counter++;
    GPIO_ResetBits(GPIOC, GPIO_Pin_0); 
  } 
  if(TIM_GetITStatus(TIM3,TIM_IT_CC1) != RESET) {
    TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
    GPIO_SetBits(GPIOC, GPIO_Pin_0); 
  } 
} 

void init_Timer2() {
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef OutputChannel; /* TIM2 Clock Enable */ 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); /* Enable TIM2 Global Interrupt */ 
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); /* TIM2 Initialize */ 
  TIM_TimeBaseStructure.TIM_Period=100-1; // 100kHz 
  TIM_TimeBaseStructure.TIM_Prescaler=24-1;
  TIM_TimeBaseStructure.TIM_ClockDivision=0;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); /* TIM2 PWM Initialize */ 
  OutputChannel.TIM_OCMode = TIM_OCMode_PWM1;
  OutputChannel.TIM_OutputState=TIM_OutputState_Enable;
  OutputChannel.TIM_OutputNState=TIM_OutputNState_Enable;
  OutputChannel.TIM_Pulse=50-1; // 50% duty ratio
  OutputChannel.TIM_OCPolarity=TIM_OCPolarity_Low;
  OutputChannel.TIM_OCNPolarity=TIM_OCNPolarity_High;
  OutputChannel.TIM_OCIdleState=TIM_OCIdleState_Set;
  OutputChannel.TIM_OCNIdleState=TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM2,&OutputChannel); /* TIM2 Enale */ 
  TIM_Cmd(TIM2,ENABLE);
  TIM_ITConfig(TIM2,TIM_IT_Update | TIM_IT_CC1 ,ENABLE); // interrupt enable
}

void init_Timer3() {
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef OutputChannel; /* TIM2 Clock Enable */ 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); /* Enable TIM2 Global Interrupt */ 
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); /* TIM2 Initialize */ 
  TIM_TimeBaseStructure.TIM_Period=100-1; // 100kHz 
  TIM_TimeBaseStructure.TIM_Prescaler=24-1;
  TIM_TimeBaseStructure.TIM_ClockDivision=0;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); /* TIM2 PWM Initialize */ 
  OutputChannel.TIM_OCMode = TIM_OCMode_PWM1;
  OutputChannel.TIM_OutputState=TIM_OutputState_Enable;
  OutputChannel.TIM_OutputNState=TIM_OutputNState_Enable;
  OutputChannel.TIM_Pulse=50-1; // 50% duty ratio
  OutputChannel.TIM_OCPolarity=TIM_OCPolarity_Low;
  OutputChannel.TIM_OCNPolarity=TIM_OCNPolarity_High;
  OutputChannel.TIM_OCIdleState=TIM_OCIdleState_Set;
  OutputChannel.TIM_OCNIdleState=TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM2,&OutputChannel); /* TIM2 Enale */ 
  TIM_Cmd(TIM3,ENABLE);
  TIM_ITConfig(TIM3,TIM_IT_Update | TIM_IT_CC1 ,ENABLE); // interrupt enable
}
void make_pwm1(uint16_t val) {
  TIM_OCInitTypeDef OutputChannel; 
  OutputChannel.TIM_OCMode = TIM_OCMode_PWM1;
  OutputChannel.TIM_OutputState=TIM_OutputState_Enable;
  OutputChannel.TIM_OutputNState=TIM_OutputNState_Enable;
  OutputChannel.TIM_Pulse=val;
  OutputChannel.TIM_OCPolarity=TIM_OCPolarity_Low;
  OutputChannel.TIM_OCNPolarity=TIM_OCNPolarity_High;
  OutputChannel.TIM_OCIdleState=TIM_OCIdleState_Set;
  OutputChannel.TIM_OCNIdleState=TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM2,&OutputChannel);
}

void make_pwm2(uint16_t val){
  TIM_OCInitTypeDef OutputChannel; 
  OutputChannel.TIM_OCMode = TIM_OCMode_PWM1;
  OutputChannel.TIM_OutputState=TIM_OutputState_Enable;
  OutputChannel.TIM_OutputNState=TIM_OutputNState_Enable;
  OutputChannel.TIM_Pulse=val;
  OutputChannel.TIM_OCPolarity=TIM_OCPolarity_Low;
  OutputChannel.TIM_OCNPolarity=TIM_OCNPolarity_High;
  OutputChannel.TIM_OCIdleState=TIM_OCIdleState_Set;
  OutputChannel.TIM_OCNIdleState=TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM3,&OutputChannel);
}

int main() {
  SystemInit();
   RCC_Configure();
   GPIO_Configure();
   USART_Configure();
   NVIC_Configure();
   init_port();
   init_Timer2();
   init_Timer3();
  GPIO_ResetBits(GPIOB, GPIO_Pin_3);
  GPIO_ResetBits(GPIOB, GPIO_Pin_4);
  
  while(1) {
      switch(a){
       case('0'):
         speed1_value = 0;
         speed2_value = 0;
         break;
       case('1'):
         speed1_value = 50;
         speed2_value = 50;
         break;
       case('2'):
         speed1_value = 80;
         speed2_value = 80;
         break;
       case('3'):
         speed1_value = 140;
         speed2_value = 140;
         break;
       case('4'):
         speed1_value = 180;
         speed2_value = 180;
         break;
       case('5'):
         speed1_value = 220;
         speed2_value = 220;
         break;
       case('6'):
         speed1_value = 250;
         speed2_value = 250;
         break;
     }
     if(dir1 == 'B'){
       if(speed1_value != 0 || speed2_value != 0){
         speed1_value -= 30;
       }
     }
     if(dir2 == 'D'){
       if(speed1_value != 0 || speed2_value != 0){
         speed2_value -= 30;
       }
     }
    TIM2->CRR1 = speed1_value;
    TIM3->CRR1 = speed2_value;
  }
}