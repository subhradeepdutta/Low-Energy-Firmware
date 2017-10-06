/******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *****************************************************************************/

/********************************************************************************
* File ​Name 	  : sleep.c
* File Description: This file defines the function to put the system to sleep
* 					and uses a global shared variable to synchronize the lowest
* 					possible energy mode
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
* References: The function sleep(), blockSleepMode() and unblockSleepMode()
* 			  have been used from Silicon Labs libraries a copy of which was
* 			  made available in Lecture Slide #3
* Link		: https://github.com/ARMmbed/mbed-hal-silabs/blob/master/source/sleep.c
********************************************************************************/

#include "main.h"
#include "em_emu.h"
#include "sleep.h"


extern int sleep_block_counter[5];

void sleep(void) {
if (sleep_block_counter[0] > 0)
{
	return; // Blocked everything below EM0, so just return
}
else if (sleep_block_counter[1] > 0)
{
	EMU_EnterEM1(); // Blocked everything below EM1, enter EM1
}
else if (sleep_block_counter[2] > 0)
{
	EMU_EnterEM2(true); // Blocked everything below EM2, enter EM2
}
else if (sleep_block_counter[3] > 0)
{
	EMU_EnterEM3(true); // Blocked everything below EM3, enter EM3
}
else
{
	EMU_EnterEM3(true); // Nothing is blocked, enter EM4
}
return;
}

void blockSleepMode(sleepstate_enum minimumMode)
{
/*Disable interrupts*/
CORE_ATOMIC_IRQ_DISABLE();
sleep_block_counter[minimumMode]++;
/*Re-enable interrupts*/
CORE_ATOMIC_IRQ_ENABLE();
}

void unblockSleepMode(sleepstate_enum minimumMode)
{
CORE_ATOMIC_IRQ_DISABLE();
if(sleep_block_counter[minimumMode] > 0)
{
	sleep_block_counter[minimumMode]--;
}
CORE_ATOMIC_IRQ_ENABLE();
}
