/*
 * Flow System based on PX4FLOW by PX4 Development Team
 * Developed by Jason.C.T Mar.2016
 */
 
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

/* Init */
void communication_init(void);

/* Send */
void communication_system_state_send(void);
void communication_parameter_send(void);

/* Receive */
void communication_receive(void);
void communication_receive_forward(void);
void communication_receive_usb(void);
void handle_mavlink_message(mavlink_channel_t chan, mavlink_message_t* msg);

mavlink_status_t* mavlink_get_channel_status(uint8_t channel);
mavlink_message_t* mavlink_get_channel_buffer(uint8_t channel);

#endif /* COMMUNICATION_H_ */
