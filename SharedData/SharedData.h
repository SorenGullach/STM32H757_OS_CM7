#pragma once

#include <HSEM.h>

// make a struct to be placed in shared memory, and can be accessed from both MPU's
struct _InterCommunication : 
	hwHSEM<1> // use HSEM x as semafor
{
	uint8_t CounterCM4;		
	uint32_t CounterCM7;		
};

extern _InterCommunication InterCommunication;