#include "nrf24_mavlink.h"
#include "MY_NRF24.h"
#include "usb_device.h"

#define _ABS_(x) (((x)>0) ? x : -x)
#define NRF_VCP_DEBUG 1

ap_t       mav_data;
vel_target vel;


void update_mavlink(void)
{
  uint8_t myTxData[32];
  uint8_t AckPayload[32];
  mavlink_message_t msg;
  char printBuf[100];

  mavlink_msg_simple_pack(0,0,&msg,0);
  int len = mavlink_msg_to_send_buffer(myTxData, &msg);
  
  if(NRF24_write(myTxData, len)) 
  {
    NRF24_read(AckPayload, 32);
    mavlink_message_t msg_receive;
    mavlink_status_t mav_status;
    for(uint8_t i=0; i<32; i++) 
    {
      if(mavlink_parse_char(0, AckPayload[i], &msg_receive, &mav_status)) 
      {
        switch (msg_receive.msgid) 
        {
          case MAVLINK_MSG_ID_VELOCITY: 
          {
            mavlink_velocity_t packet;
            mavlink_msg_velocity_decode(&msg_receive, &packet);

//            vel.vel_x = packet.vel_x;
//            vel.vel_y = packet.vel_y;
//            vel.rad_z = packet.rad_z;
            
            sprintf(printBuf, "vel_x:%.2f, vel_y:%.2f, rad_z:%.2f\r\n", packet.vel_x, packet.vel_y, packet.rad_z);
            VCPSend((uint8_t *)printBuf, strlen(printBuf));
          
            break;
          }
          default:break;
          
        }
      }
    }
  }
}








