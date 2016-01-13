/* 
 * File:   usb_callbacks.h
 * Author: aburns2
 *
 * Created on January 12, 2016, 9:36 PM
 */

#ifndef USB_CALLBACKS_H
#define	USB_CALLBACKS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "usb.h"
#include "usb_config.h"
#include "usb_ch9.h"
#include "usb_cdc.h"


    
 /* Callbacks. These function names are set in usb_config.h. */
void app_set_configuration_callback(uint8_t configuration);

uint16_t app_get_device_status_callback();

void app_endpoint_halt_callback(uint8_t endpoint, bool halted);


int8_t app_set_interface_callback(uint8_t interface, uint8_t alt_setting);


int8_t app_get_interface_callback(uint8_t interface);


void app_out_transaction_callback(uint8_t endpoint);


void app_in_transaction_complete_callback(uint8_t endpoint);


int8_t app_unknown_setup_request_callback(const struct setup_packet *setup);


int16_t app_unknown_get_descriptor_callback(const struct setup_packet *pkt, const void **descriptor);


void app_start_of_frame_callback(void);


void app_usb_reset_callback(void);


/* CDC Callbacks. See usb_cdc.h for documentation. */

int8_t app_send_encapsulated_command(uint8_t interface, uint16_t length);


int16_t app_get_encapsulated_response(uint8_t interface,
                                      uint16_t length, const void **report,
                                      usb_ep0_data_stage_callback *callback,
                                      void **context);


void app_set_comm_feature_callback(uint8_t interface,
                                     bool idle_setting,
                                     bool data_multiplexed_state);


void app_clear_comm_feature_callback(uint8_t interface,
                                       bool idle_setting,
                                       bool data_multiplexed_state);


int8_t app_get_comm_feature_callback(uint8_t interface,
                                     bool *idle_setting,
                                     bool *data_multiplexed_state);


void app_set_line_coding_callback(uint8_t interface,
                                    const struct cdc_line_coding *coding);


int8_t app_get_line_coding_callback(uint8_t interface,
                                    struct cdc_line_coding *coding);


int8_t app_set_control_line_state_callback(uint8_t interface,
                                           bool dtr, bool dts);


int8_t app_send_break_callback(uint8_t interface, uint16_t duration);



#ifdef	__cplusplus
}
#endif

#endif	/* USB_CALLBACKS_H */

