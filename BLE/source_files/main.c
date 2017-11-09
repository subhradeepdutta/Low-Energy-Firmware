/********************************************************************************
* File ​Name 	  : main.c
* File Description: This file includes the main function which
* 					has an infinite loop where the system is put
*					to sleep indefinitely
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
********************************************************************************/

#define MIN_TX (-260)
#define MAX_TX (80)
#define TX_POWER_65_55 (-50)
#define TX_POWER_85_75 (50)
#define TX_POWER_45_35 (-200)
#define TX_POWER_75_65 (0)
#define TX_POWER_55_45 (-150)
#define ADVERTISING_MILISECONDS (500)
#define ADVERTISING_INTERVAL (1.6 * ADVERTISING_MILISECONDS)
#define CONNECTION_INTERVAL	75
#define CONNECTION_VALUE (CONNECTION_INTERVAL/1.25)
#define SLAVE_INTERVAL (375)
#define SLAVE_LATENCY ((SLAVE_INTERVAL/CONNECTION_INTERVAL) - 1)


/* Board headers */
#include "boards.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "aat.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#ifdef FEATURE_BOARD_DETECTED
#include "bspconfig.h"
#include "pti.h"
#endif

/* Device initialization header */
#include "InitDevice.h"

#ifdef FEATURE_SPI_FLASH
#include "em_usart.h"
#include "mx25flash_spi.h"
#endif /* FEATURE_SPI_FLASH */

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

#ifdef FEATURE_PTI_SUPPORT
static const RADIO_PTIInit_t ptiInit = RADIO_PTI_INIT;
#endif

/* Gecko configuration parameters (see gecko_configuration.h) */
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
  #ifdef FEATURE_PTI_SUPPORT
  .pti = &ptiInit,
  #endif
};


/* Flag for indicating DFU Reset must be performed */
uint8_t boot_to_dfu = 0;

//***********************************************************************************
// Include files
//***********************************************************************************

#include "em_device.h"
#include "em_chip.h"
#include "main.h"
#include "gpio.h"
#include "cmu.h"
#include "em_cmu.h"
#include "sleep.h"
#include "em_letimer.h"
#include "letimer.h"
#include "adc.h"
#include "timer.h"
#include "spi.h"
#include "I2C.h"
#include "infrastructure.h"

int sleep_block_counter[5]={0,0,0,0,0};


/************************************************************************************
Name: temperature_data
Description : Converts the temperature into bitstream and send it to the client
Inputs: N/A
Returns N/A
************************************************************************************/

void temperature_data()
{
	/* Stores the temperature data in the Health Thermometer format. */
	uint8_t HealthTempBuffer[5];
	/* The flag_value  is set as 0 for Celsius format. */
	uint8_t flag_value = 0x00;
	/* Stores the temperature data read from the SI7021 sensor */
	uint32_t temp;
	uint8_t *ptr = HealthTempBuffer; /* Pointer to the buffer for conversion to bitstream */
	/* Converts flag_value to bitstream and appends it to the buffer */
	UINT8_TO_BITSTREAM(ptr, flag_value);
    /* Convert the sensor data to the correct temperature format */
    temp = FLT_TO_UINT32(temperature, 0);
    /* Convert temperature data to bitstream and place it in the buffer */
    UINT32_TO_BITSTREAM(ptr, temp);
    gecko_cmd_gatt_server_send_characteristic_notification(ble_connection, gattdb_temperature_measurement, 5, HealthTempBuffer);
}


int main(void)
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

	/* Initialize clocks */
	cmu_init();
	/* Initialize GPIO */
	gpio_init();

	/* Initialize LETIMER0*/
	le_timer_init(ON_period, TOTAL_period);
	/*Setup the ADC for conversion*/
	ADC_Initialize();

	LETIMER0->CMD = LETIMER_CMD_START;
	ADC_Start(ADC0, adcStartSingle);
	/*Initialize SPI*/
	SPI_init();

	/*Timer 0 Setup*/
	timer0_init();

	/*BMA 280 setup*/
	BMA280_init();
	BMA280_suspend();

	while (1)
	{

		/* Event pointer for handling events */
		struct gecko_cmd_packet* evt;

		/* gecko_wait_event is the bluetooth stack version of sleep() which allows the CPU to sleep in a low energy mode*/
		/* Check for stack event. */
		evt = gecko_wait_event();

		/* Once an event has occurred the CPU comes out of sleep and check for the corresponding switch case */
		switch (BGLIB_MSG_ID(evt->header))
		{
			/* This boot event is generated when the system boots up after reset.
			* Here the system is set to start advertising immediately after boot procedure. */
			case gecko_evt_system_boot_id:
			/* Set advertising parameters. 100ms advertisement interval. All channels used.
			*The first two parameters are minimum and maximum advertising interval, both in
			* units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
			gecko_cmd_le_gap_set_adv_parameters(ADVERTISING_INTERVAL, ADVERTISING_INTERVAL, 7);
			/* Start general advertising and enable connections. */
			gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
			break;

			case gecko_evt_gatt_server_characteristic_status_id:
			/* Check that the characteristic in question is temperature - its ID is defined
			* in gatt.xml as "temp_measurement". Also check that status_flags = 1, meaning that
			* the characteristic client configuration was changed (notifications or indications
			* enabled or disabled). */
			if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement) && (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01))
			{
				if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x02)
				{
				/* Indications have been turned ON - start the repeating timer. The 1st parameter '32768'
				 * tells the timer to run for 1 second (32.768 kHz oscillator), the 2nd parameter is
				 * the timer handle and the 3rd parameter '0' tells the timer to repeat continuously until
				 * stopped manually.*/
					gecko_cmd_hardware_set_soft_timer(32768, 0, 0);
				}
				else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x00)
				{
					gecko_cmd_hardware_set_soft_timer(0, 0, 0);
				}
			}
			break;

			/*A new connection was opened*/
			case gecko_evt_le_connection_opened_id:
			ble_connection = evt->data.evt_le_connection_opened.connection;
			gecko_cmd_le_connection_set_parameters(ble_connection,CONNECTION_VALUE, CONNECTION_VALUE, SLAVE_LATENCY, 0x300);
			break;

			/*We adjust the transmit power in the gecko based on the RSSI seen on the receiver end*/
			case gecko_evt_le_connection_rssi_id:
			if(evt->data.evt_le_connection_rssi.rssi > -35)
			{
				gecko_cmd_system_set_tx_power(MIN_TX);
			}
			else if((evt->data.evt_le_connection_rssi.rssi > -45) & (evt->data.evt_le_connection_rssi.rssi < -35))
			{
				gecko_cmd_system_set_tx_power(TX_POWER_45_35);
			}
			else if((evt->data.evt_le_connection_rssi.rssi > -55) & (evt->data.evt_le_connection_rssi.rssi < -45))
			{
				gecko_cmd_system_set_tx_power(TX_POWER_55_45);
			}
			else if((evt->data.evt_le_connection_rssi.rssi > -65) & (evt->data.evt_le_connection_rssi.rssi < -55))
			{
				gecko_cmd_system_set_tx_power(TX_POWER_65_55);
			}
			else if((evt->data.evt_le_connection_rssi.rssi > -75) & (evt->data.evt_le_connection_rssi.rssi < -65))
			{
				gecko_cmd_system_set_tx_power(TX_POWER_75_65);
			}
			else if((evt->data.evt_le_connection_rssi.rssi > -85) & (evt->data.evt_le_connection_rssi.rssi < -75))
			{
				gecko_cmd_system_set_tx_power(5);
			}
			else if(evt->data.evt_le_connection_rssi.rssi < -85)
			{
				gecko_cmd_system_set_tx_power(MAX_TX);
			}
			break;

			/*This event indicates external signals have been received.*/
			case gecko_evt_system_external_signal_id:
			switch(evt->data.evt_system_external_signal.extsignals)
			{
				case 1:
				temperature_data();
				break;
				default:
				break;
			}
			break;

			/*This event indicates that a connection was closed*/
			case gecko_evt_le_connection_closed_id:
			/*Reset the transmit power back to 0 so that the gecko can reconnect irrespective of its distance*/
			gecko_cmd_system_set_tx_power(0);
			/* Check if need to boot to dfu mode */
			if (boot_to_dfu)
			{
				/* Enter to DFU OTA mode */
				gecko_cmd_system_reset(2);
			}
			else
			{
				/* Restart advertising after client has disconnected */
				gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
			}
			break;

			/* Checks if the user-type OTA Control Characteristic was written.
			* If written, boots the device into Device Firmware Upgrade (DFU) mode. */
			case gecko_evt_gatt_server_user_write_request_id:
			if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control)
			{
				/* Set flag to enter to OTA mode */
				boot_to_dfu = 1;
				/* Send response to Write Request */
				gecko_cmd_gatt_server_send_user_write_response(
				evt->data.evt_gatt_server_user_write_request.connection,
				gattdb_ota_control,
				bg_err_success);

				/* Close connection to enter to DFU OTA mode */
				gecko_cmd_endpoint_close(evt->data.evt_gatt_server_user_write_request.connection);
			}
			break;
			default:
			break;
		}/* End switch case*/
	}/*End while loop*/
}


