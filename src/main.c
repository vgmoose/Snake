/*
|	-OOOO-O----OOO---OO----OO---OO-OOOOOOO-
|	OOOOO-OO---OO----OO----OO---OO-OOOOOOOO
|	O-----OO---OO----OO----OO---OO-OO-----O
|	O-----OOO--OO---OOOO---OO--OO--OO-----O
|	O-----OOO--OO---O--O---OO-OOO--OO------
|	OOO---OOOO-OO---O--O---OOOOO---OOOOO---
|	-OOO--OO-O-OO--OO--OO--OOOO----OOOOO---
|	--OOO-OO-OOOO--OO--OO--OOOOO---OOOOO---
|	---OO-OO--OOO--OOOOOO--OO-OOO--OO------
|	---OO-OO--OOO-OOOOOOOO-OO--OO--OO------
|	---OO-OO---OO-OO----OO-OO---OO-OO-----O
|	OOOO--OO---OO-OO----OO-OO---OO-OOOOOOOO
|	OOO--OOO----O-OO----OO-OO---OO-OOOOOOO
|	
|	For Wii U
|	v1.0
|	eliboa@gmail.com
|
*/

#include <vpad/input.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/proc.h>
#include "system/memory.h"
#include "main.h"
#include "snake.h"
#include "draw.h"

int Menu_Main()
{    
    memoryInitialize();
    VPADInit();

    int screen_buf0_size = 0;

	//Call the Screen initilzation function.
	OSScreenInit();
	
	//Grab the buffer size for each screen (TV and gamepad)
	screen_buf0_size = OSScreenGetBufferSizeEx(0);
	OSScreenGetBufferSizeEx(1);
    
	//Set the buffer area.
	OSScreenSetBufferEx(0, (void *)0xF4000000);
	OSScreenSetBufferEx(1, (void *)0xF4000000 + screen_buf0_size);
	
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
	
	//Clear both framebuffers.
	int ii;
	for (ii=0; ii < 2; ii++)
	{
		fillScreen(0,0,0,0);
		flipBuffers();
	}
	
	/****************************>             VPAD LOOP             <****************************/
	/* Enter the VPAD loop */
	int error;
	VPADStatus vpad_data;

	//Read initial vpad status
	VPADRead(0, &vpad_data, 1, &error);

	//Init Snake
	Snake s;                    
	s.frame_x = 840;
	s.frame_y = 480;
	s.x = 500;
	s.y = 300;
	s.w = 20;
	s.length = 6;
	s.direction = 'R';
	s.score=0;
	s.start=0;
	s.first = NULL;
	s.food_state = 0;
	s.food_x = 0;
	s.food_y = 0;
	s.seed = 1;
	s.pause = 0;
	s.debug = 0;

	initSnake(&s);

	while(1)
	{
		// Read the data from gamepad
		VPADRead(0, &vpad_data, 1, &error);
		// Init gamepad screen
		//fillScreen(255,0,0,0);
		fillScreen(0,0,0,0);

		if(s.start) {
			triggerSnake(&s, &vpad_data);
			displaySnake(&s);
		} else {
			displayStart(&s);
		}

		if(!s.start && (vpad_data.hold || vpad_data.trigger)) s.start=1;

		if (vpad_data.trigger & VPAD_BUTTON_A) {
			if(s.loose) {
				s.length=6;
				s.score=0;
				s.food_state = 0;
				initSnake(&s);
			}
		}	
		if ((vpad_data.trigger & VPAD_BUTTON_B) && s.start) {
			if(s.pause) s.pause = 0;
			else s.pause = 1;
		}
		/*
		if (vpad_data.trigger & VPAD_BUTTON_Y) {
			if(s.debug) s.debug = 0;
			else s.debug = 1;
		}		
		*/
		// Clear buffer
		flipBuffers();
		// Exit when Home button is pressed
		if(vpad_data.hold & VPAD_BUTTON_HOME) break;
        
    OSSleepTicks(OSMillisecondsToTicks(100));
	}

	// WARNING: DO NOT CHANGE THIS. YOU MUST CLEAR THE FRAMEBUFFERS AND 
	//  IMMEDIATELY CALL EXIT FROM THIS FUNCTION. RETURNING TO LOADER CAUSES FREEZE.

	for(ii=0;ii<2;ii++)
	{
		fillScreen(0,0,0,0);
		flipBuffers();
	}

  memoryRelease();
        
	return 0;
}


