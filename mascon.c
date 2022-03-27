#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "dot_matrix.h"
// PIN DEFINE

int get_Mascon_Stat()
{
	int stat = 0;
	for (int i = 0; i < 11; i++)
	{
		if (gpio_get(i + 2) == 0)
		{
			stat = i;
			break;
		}
	}
	return stat;
}

void set_Mascon_Stat_Display()
{
	int display_data[32] = {};
	get_Display_Data(display_data, get_Mascon_Stat());
	set_Display(display_data, 0);
}

bool led = false;
int main()
{
	stdio_init_all();

	//
	// uart
	//
	uart_init(uart0, 500000);
	gpio_set_function(0, GPIO_FUNC_UART);
	gpio_set_function(1, GPIO_FUNC_UART);
	uart_puts(uart0, "Hello world!");

	//
	// led
	//
	gpio_init(25);
	gpio_set_dir(25, true);
	gpio_put(25, false);

	//
	// rotary switch pin initialize
	//
	for (int i = 0; i < 11; i++)
	{
		int pin = i + 2;
		gpio_init(pin);
		gpio_set_dir(pin, false);
		gpio_pull_up(pin);
	}

	sleep_ms(100);

	int pre_stat = -1;
	bool led = false;
	while (true)
	{
		int stat = get_Mascon_Stat();
		if (stat == pre_stat || (pre_stat > 1 && stat == 0))
			continue;

		led = led == 0 ? 1 : 0;
		gpio_put(25, led);

		printf("%d\r\n", stat);
		pre_stat = stat;

		// display
		sleep_ms(10);
		set_Mascon_Stat_Display();

		
	}
	return 0;
}
