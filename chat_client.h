#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

/* headers from Connext Cert installation */
#include "rti_me_c.h"
#include "disc_dpse/disc_dpse_dpsediscovery.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"
#include "netio/netio_udp.h"

/* rtiddsgen generated headers */
#include "./generated/chat.h"
#include "./generated/chatPlugin.h"
#include "./generated/chatSupport.h"

/* user-configurable values */
int domain_id = 100;
char *peer = "127.0.0.1";

#endif
