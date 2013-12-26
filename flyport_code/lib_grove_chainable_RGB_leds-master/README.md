lib_grove_chainable_RGB_leds
============================

Flyport library for Grove chainable RGB leds, released under GPL v.3.<br>
The library allows to connect several different RGB leds on a single line. Functions are available to change the number of leds and the color of each one of them, so a stripe of many different colored led can be create. <BR>
Functions can also be used to create transitions or special effects in easy way. An example of usage follows, supporting 2 different leds. More infos on wiki.openpicus.com.
<br>1) import files inside Flyport IDE using the external libs button.<br>
2) add following code example in FlyportTask.c:

<pre>
#include "taskFlyport.h"
#include "grovelib.h"
#include "rgb.h"
 
void FlyportTask()
{  
	char msg[100];
 
	vTaskDelay(50);
	UARTWrite(1,"Welcome to GROVE NEST example!\r\n");
 
	// GROVE board
	void *board = new(GroveNest);
 
	// GROVE devices	
	// RGB leds
	void *rgb = new(Rgb, 2);
 
	// Attach devices
	attachToBoard(board, rgb, DIG4);
 
	// Initialize devices
	configure(rgb, 1, 255, 0, 0);
	configure(rgb, 2, 125, 255, 0);
	set(rgb, ON);
 
	while(1)
	{
		vTaskDelay(100);
		configure(rgb, 2, 255, 0, 0);
		set(rgb, ON);
		vTaskDelay(100);
		configure(rgb, 2, 0, 255, 0);
		set(rgb, ON);
		vTaskDelay(100);
		set(rgb, OFF);
		vTaskDelay(100);
		configure(rgb, 2, 0, 0, 255);
		configure(rgb, 1, 0, 0, 255);
		set(rgb, ON);
		vTaskDelay(100);
		configure(rgb, 1, 255, 0, 0);
		set(rgb, ON);		
	}
}
</pre>
