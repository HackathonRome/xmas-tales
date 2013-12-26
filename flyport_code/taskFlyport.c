#include "taskFlyport.h"
#include "grovelib.h"
#include "rgb.h"
#include "string.h"

void FlyportTask()
{	
	vTaskDelay(100);
	UARTWrite(1,"Welcome to GROVE NEST example!\r\n");
	
	// GROVE board
	void *board = new(GroveNest);
	
	// RGB leds
	void *rgb1 = new(Rgb, 2);
	void *rgb2 = new(Rgb, 2);
	void *rgb4 = new(Rgb, 2);
 
	// Attach devices
	attachToBoard(board, rgb1, DIG1);
	attachToBoard(board, rgb2, DIG2);
	attachToBoard(board, rgb4, DIG4);
 
	// Initialize devices
	configure(rgb1, 1, 255, 0, 0);
	configure(rgb1, 2, 0, 255, 0);
	configure(rgb2, 1, 0, 0, 255);
	configure(rgb2, 2, 255, 0, 255);
	configure(rgb4, 1, 0, 255, 255);
	configure(rgb4, 2, 250, 255, 0);
	set(rgb1, ON);
	set(rgb2, ON);
	set(rgb4, ON);
	vTaskDelay(100);
	set(rgb1, OFF);
	set(rgb2, OFF);
	set(rgb4, OFF);
	
	// Connection to Network
	#if defined (FLYPORT_WF)
	WFConnect(WF_DEFAULT);
	while (WFStatus != CONNECTED);
	#endif
	#if defined (FLYPORTETH)
	while(!MACLinked);
	#endif
	UARTWrite(1,"Flyport connected... hello world!\r\n");
	vTaskDelay(200);
	
	TCP_SOCKET SocketTCPServer = INVALID_SOCKET;
	int txt = 0;

	while(1)
	{	
		// Create socket Server connection
		if(SocketTCPServer == INVALID_SOCKET)
		{
			SocketTCPServer = TCPServerOpen("5555");
			UARTWrite(1, "TCP socket opened succesfully\n");
			
			// A couple of flashes, just to be sure that everybody knows that TCP socket is now open
			set(rgb1, ON);
			set(rgb2, ON);
			set(rgb4, ON);
			vTaskDelay(50);
			set(rgb1, OFF);
			set(rgb2, OFF);
			set(rgb4, OFF);
			
			vTaskDelay(50);
			
			set(rgb1, ON);
			set(rgb2, ON);
			set(rgb4, ON);
			vTaskDelay(50);
			set(rgb1, OFF);
			set(rgb2, OFF);
			set(rgb4, OFF);
	
		}
		else
		{
			char readch[100];
			int tcprxlength = TCPRxLen(SocketTCPServer);
			if (tcprxlength > 0)
			{
				int i;
				for(i=0; i < tcprxlength; i++)
				{
					TCPpRead(SocketTCPServer, readch, 1, i);
				}
				if (txt == 1)
				{
					txt=0;
				}
				else
				{
					txt=1;
				}
				TCPRxFlush(SocketTCPServer);
			}	
			if (txt == 0)
			{
				UARTWrite(1, "off\n");
				set(rgb1, OFF);
				set(rgb2, OFF);
				set(rgb4, OFF);
			}
			else
			{
				txt = 0;
				UARTWrite(1, "on\n");
				
				// Lights' ballet code
				set(rgb1, ON);
				set(rgb2, ON);
				set(rgb4, ON);
				
				vTaskDelay(100);
				
				configure(rgb1, 2, 255, 0, 0);
				configure(rgb2, 1, 0, 255, 0);
				configure(rgb2, 2, 0, 0, 255);
				configure(rgb4, 1, 255, 0, 255);
				configure(rgb4, 2, 0, 255, 255);
				configure(rgb1, 1, 250, 255, 0);
				set(rgb1, ON);
				set(rgb2, ON);
				set(rgb4, ON);
		
				vTaskDelay(100);
				
				configure(rgb2, 1, 255, 0, 0);
				configure(rgb2, 2, 0, 255, 0);
				configure(rgb4, 1, 0, 0, 255);
				configure(rgb4, 2, 255, 0, 255);
				configure(rgb1, 1, 0, 255, 255);
				configure(rgb1, 2, 250, 255, 0);
				set(rgb1, ON);
				set(rgb2, ON);
				set(rgb4, ON);

				vTaskDelay(100);
				
				configure(rgb2, 2, 255, 0, 0);
				configure(rgb4, 1, 0, 255, 0);
				configure(rgb4, 2, 0, 0, 255);
				configure(rgb1, 1, 255, 0, 255);
				configure(rgb1, 2, 0, 255, 255);
				configure(rgb2, 1, 250, 255, 0);
				set(rgb1, ON);
				set(rgb2, ON);
				set(rgb4, ON);
				
				vTaskDelay(100);
				
				configure(rgb1, 1, 255, 0, 0);
				configure(rgb1, 2, 0, 255, 0);
				configure(rgb2, 1, 0, 0, 255);
				configure(rgb2, 2, 255, 0, 255);
				configure(rgb4, 1, 0, 255, 255);
				configure(rgb4, 2, 250, 255, 0);
				set(rgb1, ON);
				set(rgb2, ON);
				set(rgb4, ON);
			}
		}
	}
}









