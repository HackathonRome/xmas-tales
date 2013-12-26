/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        rgb.c
 *  Module:          FlyPort WI-FI - FlyPort ETH
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Vicca Davide	     1.0     2/09/2012		   First release  
 *  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Software License Agreement
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by 
 *  the Free Software Foundation AND MODIFIED BY OpenPicus team.
 *  
 *  ***NOTE*** The exception to the GPL is included to allow you to distribute
 *  a combined work that includes OpenPicus code without being obliged to 
 *  provide the source code for proprietary components outside of the OpenPicus
 *  code. 
 *  OpenPicus software is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details. 
 * 
 * 
 * Warranty
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 **************************************************************************/

#include "taskFlyport.h"
#include "grovelib.h"
#include "rgb.h"

struct Interface *attachSensorToDigioBus(void *,int,int);


struct RGB
{
	const void *class;
	struct Interface *inter;
	BYTE led_number;
	BYTE *leds_start;
	BYTE *rgb_matrix;
};

static void soft_clock(struct RGB *self)
{
		vTaskSuspendAll();
		IOPut(self->inter->port->Pin1,ON);
		IOPut(self->inter->port->Pin1,OFF);
		xTaskResumeAll();
}

static void program(struct RGB *self,BYTE d)
{
	int i = 0;
	BYTE r;
	for(;  i < 8 ;i++)
	{ 
		r = d;
		r = r >> 7;
		IOPut(self->inter->port->Pin2,r);		
		soft_clock(self);
		d = d <<1;
	}
	
}

static void default_config(struct RGB *self)
{	
	int j,k;
	self->rgb_matrix = self->leds_start;	
	for(j = 0 ; j < self->led_number ; j++) 
	{
		for(k = 0 ; k < 3 ; k++)
		{
			*self->rgb_matrix = 0;
			self->rgb_matrix++;
		}
	}
	self->rgb_matrix = self->leds_start;		
}


static int set1_(void *self,...)
{	
	va_list ap;
	va_start(ap,self);
	const struct SensorClass **cp = self;
	if(self && *cp && (*cp)->set_)
		return (*cp)->set_(self,&ap);
	return -1;
}

static void *RGB_ctor (void * _self, va_list *app)
{
	struct RGB *self = _self;
	self->led_number = va_arg(*app, const BYTE);
	self->rgb_matrix =(BYTE *)calloc (3* self->led_number, sizeof (BYTE)); 
	if(!self->rgb_matrix)
		return NULL;
	self->leds_start = self->rgb_matrix;	
	default_config(self);
	self->inter = NULL;
	return self;

}	

static void RGB_dtor (void * _sensor)
{
	struct RGB *sensor = _sensor;
	sensor->rgb_matrix = sensor->leds_start; 
	free(sensor->rgb_matrix);
	if(sensor->inter)
	{
		free(sensor->inter->port);
		free(sensor->inter);
	}

}	


static int RGB_set(void * _self, va_list *app)
{
	struct RGB *self = _self;
	int state =  va_arg(*app, int);
	int y;
	BYTE data  = 0;
	BYTE d  = 0;
	for(y = 0;y <4;y++)
	{
		program(self,data);
	}	
	if(state == OFF)
	{
		for(y = 0;y < self->led_number;y++)
		{
			program(self,0xFF);
			program(self,0x00);
			program(self,0x00);
			program(self,0x00);
		}
	}		
	else if(state == ON)
	{
		for(y = 0;y < self->led_number;y++)
		{

			data = data | 0xC0;
			d = (~(*self->rgb_matrix) & (0xC0));
			d = d>>2;
			data = data | d;
			self->rgb_matrix++;
			
			d = (~(*self->rgb_matrix) & (0xC0));
			d = d>>4;
			data = data | d;
			self->rgb_matrix++;
			
			d = (~(*self->rgb_matrix) & (0xC0));
			d = d>>6;
			data = data | d;

			program(self,data);

			/*2° BYTE =  blue*/
			self->rgb_matrix-=2;
			program(self,*self->rgb_matrix);
			self->rgb_matrix++;

			/*3° BYTE =  green*/
			program(self,*self->rgb_matrix);
			self->rgb_matrix++;

			/*4° BYTE =  red*/
			program(self,*self->rgb_matrix);
			self->rgb_matrix++;

			}
		self->rgb_matrix = self->leds_start;
	}
	for(y = 0;y <4;y++)
	{
		program(self,0x00);
	}	
	return 0;
}	



static void *RGB_attach (void * _board,void *_sensor,int n)
{
	struct RGB *sensor = _sensor;
	//5 means RGB type object
	sensor->inter = attachSensorToDigioBus(_board,n,5);	
	if(!sensor->inter)
		return NULL;
	set1_(_sensor,ON);
	return sensor->inter;
}	

static int RGB_config (void * _self, va_list *app)
{
	struct RGB *self = _self;
	self->rgb_matrix = self->leds_start;
	BYTE numb = va_arg(*app, const BYTE);
	BYTE red = va_arg(*app, const BYTE);
	BYTE green = va_arg(*app, const BYTE);
	BYTE blue = va_arg(*app, const BYTE);

	self->rgb_matrix = self->rgb_matrix + ( 3*(numb-1));
	*self->rgb_matrix = blue;
	self->rgb_matrix++;
	*self->rgb_matrix = green;
	self->rgb_matrix++;
	*self->rgb_matrix = red;
	self->rgb_matrix = self->leds_start;
	return 0;
}

static const struct SensorClass _RGB =
{	
	sizeof(struct RGB),
	RGB_ctor,
	RGB_dtor,
	RGB_attach,
	RGB_config,
	RGB_set,
	0,
};

const void *Rgb = &_RGB;


