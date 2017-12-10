/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs BT Mesh Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko BT Mesh C application.
 * The application starts unprovisioned Beaconing after boot
 ***************************************************************************************************
 * <b> (C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "boards.h"
#include "btMesh-configuration.h"
#include "board_features.h"
#include "graphics.h"
#include "glib.h"
#include "ADC.h"
#include <stdio.h>

#define CO_WARNING_LEVEL (25)

#include <native_gecko.h>
#include <gecko_configuration.h>
#include "gecko_bgapi_mesh_node_native.h"

#ifdef FEATURE_BOARD_DETECTED
#include "bspconfig.h"
#include "pti.h"
#endif

#ifdef FEATURE_SPI_FLASH
#include "em_usart.h"
#include "mx25flash_spi.h"
#endif /* FEATURE_SPI_FLASH */

#include "InitDevice.h"
#include "gpio.h"
#include "letimer.h"
#include "cmu.h"
#include "I2C.h"
#include "gpiointerrupt.h"
float temperature, temp_val;

volatile static int sensor_state1 = 0;
volatile static int sensor_state2 = 0;
void temp_CO_data()
{
	char array[30];
	char CO_data[5];
	sensor_state1 =  GPIO_PinInGet(IR1_port, IR1_pin);
	sensor_state2 = GPIO_PinInGet(IR2_port, IR2_pin);
	snprintf(array, sizeof(array), "%.2f", temperature);
	array[5] = ' ';
	array[6] = 'C';
    GRAPHICS_Clear();
    GRAPHICS_AppendString("Temperature \n");
    GRAPHICS_AppendString(array);
    GRAPHICS_Update();

    uint32_t ADC_data = ADC_DataSingleGet(ADC0);
    /*Clear FIFO*/
    ADC0->SINGLEFIFOCLEAR = ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR;
    snprintf(CO_data, sizeof(CO_data), "%ld", ADC_data);
    GRAPHICS_AppendString("CO Levels \n");
    GRAPHICS_AppendString(CO_data);
    GRAPHICS_Update();

    if(ADC_data >= CO_WARNING_LEVEL)
    {
    	gecko_external_signal(0x00000005);
    }
//    if(sensor_state && !old_sensor_state)
//    {
//		GRAPHICS_AppendString(" Link Same ");
//		GRAPHICS_Update();
//    }
//    else if(!sensor_state && old_sensor_state)
//	{
//		GRAPHICS_AppendString(" Link Broken ");
//		GRAPHICS_Update();
//	}
//    else
//    	GRAPHICS_AppendString(" Unknown ");
//    	GRAPHICS_Update();
//    old_sensor_state = sensor_state;
//    char IR_data[10];
//    snprintf(IR_data, sizeof(IR_data), "%d", sensor_state);
//    GRAPHICS_AppendString(" Pin Value = \n ");
//    GRAPHICS_AppendString(IR_data);
//    GRAPHICS_Update();
//    if(sensor_state1 == 0)
//    {
//    	GRAPHICS_AppendString("Broken S1");
//		GRAPHICS_Update();
//    }
//    else if(sensor_state1 == 1)
//    {
//    	GRAPHICS_AppendString("Constant S1");
//    	GRAPHICS_Update();
//    }
//    if(sensor_state2 == 0)
//   {
//		GRAPHICS_AppendString("Broken S2");
//		GRAPHICS_Update();
//   }
//   else if(sensor_state2 == 1)
//   {
//		GRAPHICS_AppendString("Constant S2");
//		GRAPHICS_Update();
//   }
}

// bluetooth stack heap
#define MAX_CONNECTIONS 2
// One for bgstack; one for general mesh operation;
// N for mesh GATT service advertisements
#define MAX_ADVERTISERS (2 + 4)
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS) + 8000];

#define TIMER_ID_FACTORY_RESET  77

// bluetooth stack configuration
extern const struct bg_gattdb_def bg_gattdb_data;

const gecko_configuration_t config =
{
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.max_advertisers = MAX_ADVERTISERS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100,
  .gattdb = &bg_gattdb_data,
};

static void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);
static uint8 conn_handle = 0xFF;      /* handle of the last opened LE connection */
int main()
{
#ifdef FEATURE_SPI_FLASH
  /* Put the SPI flash into Deep Power Down mode for those radio boards where it is available */
  MX25_init();
  MX25_DP();
  /* We must disable SPI communication */
  USART_Reset(USART1);

#endif /* FEATURE_SPI_FLASH */
  enter_DefaultMode_from_RESET();
  gecko_init(&config);

#ifdef FEATURE_PTI_SUPPORT
  APP_ConfigEnablePti();
#endif // FEATURE_PTI_SUPPORT

 GRAPHICS_Init();
 cmu_init();
 gpio_init();
 GPIOINT_Init();
 le_timer_init();
 I2Cinit();
 ADC_Initialize();
 ADC_Start(ADC0, adcStartSingle);
 I2C_Enable(I2C0, true);


 LETIMER0->CMD = LETIMER_CMD_START;

 while (1) {
    struct gecko_cmd_packet *evt = gecko_wait_event();
    handle_gecko_event(BGLIB_MSG_ID(evt->header), evt);
  }
}

void initiate_factory_reset(void)
{
	GRAPHICS_Clear();
	GRAPHICS_AppendString("\n***\nFACTORY RESET\n***");
	GRAPHICS_Update();

  /* if connection is open then close it before rebooting */
  if (conn_handle != 0xFF) {
    gecko_cmd_endpoint_close(conn_handle);
  }

  /* perform a factory reset by erasing PS storage. This removes all the keys and other settings
     that have been configured for this node */
  gecko_cmd_flash_ps_erase_all();
  // reboot after a small delay
  gecko_cmd_hardware_set_soft_timer(2 * 32768, TIMER_ID_FACTORY_RESET, 1);
}

static void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
  switch (evt_id) {
    case gecko_evt_dfu_boot_id:
      //gecko_cmd_le_gap_set_adv_parameters(1000*adv_interval_ms/625, 1000*adv_interval_ms/625, 7);
      gecko_cmd_le_gap_set_mode(2, 2);
      break;
    case gecko_evt_system_boot_id:
    	if (GPIO_PinInGet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN) == 0 || GPIO_PinInGet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN) == 0)
    	{
    	        initiate_factory_reset();
    	}
      // Initialize Mesh stack in Node operation mode, wait for initialized event
      gecko_cmd_mesh_node_init();
      GRAPHICS_Clear();
      GRAPHICS_AppendString("Booting up!\n");
      GRAPHICS_Update();
      break;
    case gecko_evt_mesh_node_initialized_id:
      // The Node is now initialized, start unprovisioned Beaconing using both Bearer
      gecko_cmd_mesh_node_start_unprov_beaconing(0x3);
      GRAPHICS_AppendString("Initialized\n");
      GRAPHICS_Update();
      break;
    case gecko_evt_mesh_node_provisioning_started_id:
    	GRAPHICS_AppendString("Provisioning\n");
    	GRAPHICS_Update();
    	break;

    case gecko_evt_system_external_signal_id:
	switch(evt->data.evt_system_external_signal.extsignals)
	{
		case 1:
			temp_val= I2C_read(READTEMP_HOLD_MASTER_MODE);
			temperature = ((175.72 * temp_val)/65536) - 46.85;
			temp_CO_data();
			break;
		case 2:
			GRAPHICS_AppendString("Broken S1");
			GRAPHICS_Update();
			break;
		case 4:
			GRAPHICS_AppendString("Broken S2");
			GRAPHICS_Update();
			break;
		case 5:
			GRAPHICS_AppendString("CO Warning");
			GRAPHICS_Update();
			break;
		default:
		break;
	}
	break;
    case gecko_evt_mesh_node_provisioned_id:
    	GRAPHICS_AppendString("Provisioned\n");
		GRAPHICS_Update();
		break;
    default:
      break;
  }
}
