
/*
* (c) Copyright, Real-Time Innovations, 2021.  All rights reserved.
* RTI grants Licensee a license to use, modify, compile, and create derivative
* works of the software solely for use with RTI Connext DDS. Licensee may
* redistribute copies of the software provided that all such copies are subject
* to this license. The software is provided "as is", with no warranty of any
* type, including any warranty for fitness for any purpose. RTI is under no
* obligation to maintain or support the software. RTI shall not be liable for
* any incidental or consequential damages arising out of the use or inability
* to use the software.
*/

#ifndef NIC_CONFIG_H
#define NIC_CONFIG_H

/* MacOS 10.15.x */
char *loopback_name = "lo0";
char *eth_nic_name = "en0";

/* Ubuntu 22.04 */
/* char *loopback_name = "lo";
 * char *eth_nic_name = "wlp0s20f3";
 */

/* Windows 10 */
/* char *loopback_name = "Loopback Pseudo-Interface 1";
 * char *eth_nic_name = "Wireless LAN adapter Wi-Fi";
 */

#endif