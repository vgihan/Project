#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "core_cm3.h"

char a = '\0';
char speed = '0';
char dir = 'A';

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
        GPIO_InitStructure_LED.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure_LED.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure_LED);
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
        if(a=='0' || a == '1'){
          speed = a;
          GPIO_SetBits(GPIOD, GPIO_Pin_2);
        }
        else if(a=='A' || a=='B'){
          dir = a;
          
        }
}

int main() {

   SystemInit();
   RCC_Configure();
   GPIO_Configure();
   USART_Configure();
   NVIC_Configure();

   while(1);

}