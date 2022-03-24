#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
//PIN DEFINE

bool led = false;
int main()
{
	stdio_init_all();

	//
	//led
	//
	gpio_init(25);
	gpio_set_dir(25,true);
	gpio_put(25,false);

	//
	// rotary switch pin initialize
	//
	for(int i = 0; i < 11;i++){
		int pin = i + 2;
		gpio_init(pin);
		gpio_set_dir(pin,false);
		gpio_pull_up(pin);
	}

	int pre_stat = 0;
	bool led = false;
    while (true) {
		
		int stat = 0;
		for(int i = 0; i < 11;i++){
			if(gpio_get(i + 2) == 0){
				stat = i;
				break;
			}
		}
		
		if(stat == pre_stat || (pre_stat > 1 && stat == 0)) continue;

		led = led == 0 ? 1 : 0;
		gpio_put(25,led);

		printf("%d\r\n",stat);
		pre_stat = stat;

		sleep_ms(10);
		
    }
    return 0;
}
