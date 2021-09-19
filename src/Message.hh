#ifndef mtac_message_hh
#define mtac_message_hh

typedef unsigned int Message;

const Message MESSAGE_CONNECTION_REQUEST = 0;
const Message MESSAGE_CONNECTION_RESPONSE = 1;
const Message MESSAGE_TURN   = 2;
const Message MESSAGE_UPDATE = 3;

const unsigned int PROTOCOL_ID = 0xD9182CCF;

#endif