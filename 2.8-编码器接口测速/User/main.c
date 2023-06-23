#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int main(void)
{	

	OLED_Init();
	Timer_init();
	Encoder_Init();
	
	while (1)
	{
		OLED_ShowString(1, 1, "num: ");
		OLED_ShowSignedNum(1, 6, Encoder_GetCNT(), 6);
		//此处测速的话，可以选择delay，但是这样比较占用资源
		//好一点的方法是，换一个定时器计时触发中断
		//例如用tim2的内部时钟每隔一秒触发一次中断，在中断中清空一次cnt，每次读取cnt都是一秒内的值
		//这样就不用delay阻塞程序了
	}
}
