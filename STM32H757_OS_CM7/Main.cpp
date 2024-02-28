
#include "stm32h7xx.h"

#include <MemoryDefs.h>
#include <StartupSemafor.h>

#include <SharedData/SharedData.h>

#include <PWR.h>
#include <HSEM.h>

#include <stdio.h>
#include <stdint.h>

hwPWR PWRControl(hwPWR::eSupplyConfigurations::C3);
hwHSEMControl HSEMControl;

// this class receives notifications when CM4 have made something
class HSEMTest : hwHSEMControl
{
public:
	HSEMTest()
		: hwHSEMControl(1, &InterCommunication) // register shared object(s) to get notified from
	{
	}
	
	virtual void ModifiedNotified(hwHSEMBase *data) override
	{
		if (data == &InterCommunication)
		{
			Printf("CM7 Notification\n");
			if (InterCommunication.Take(0))
			{
				Printf("CM7 CounterCM4 = %ld\n", InterCommunication.CounterCM4);
				InterCommunication.Release(0, true);
			}
			else
				Printf("CM7 failed\n");
		}
	}
 
};

__RAMFUNC int main()
{
	// Wait until CM4 boots and waits
	Printf("Waiting for CM4 to start\n");
	WaitForCM4();
	
	// init HW
	Printf("HW Init CM7\n");
	hwHSEMControl::Init(); // powerup HSEM
	
	Printf("HW Init done CM7\n");
	
	// start CM4
	Printf("Releasing CM4\n");
	ReleaseCM4();
	Printf("CM4 Running\n");

	HSEMTest Test;
	
	uint32_t var = 3;
	
	Printf("Running %lu\n", var);
	while (1)
	{
		// access shared data
		if (InterCommunication.Take(1)) // take control
		{
			InterCommunication.CounterCM7++; // update var
			Printf("CM7 Send CounterCM7 = %ld\n", InterCommunication.CounterCM7);
			//			InterCommunication.Changed(); // we have changed something
			InterCommunication.Release(1,true); // release control
		}
		uint32_t timeout = 0xFFFF;
		while (--timeout > 0) ;
		var++;
	}
}

