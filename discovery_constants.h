/*
* (c) Copyright, Real-Time Innovations, 2023.  All rights reserved.
* RTI grants Licensee a license to use, modify, compile, and create derivative
* works of the software solely for use with RTI Connext DDS. Licensee may
* redistribute copies of the software provided that all such copies are subject
* to this license. The software is provided "as is", with no warranty of any
* type, including any warranty for fitness for any purpose. RTI is under no
* obligation to maintain or support the software. RTI shall not be liable for
* any incidental or consequential damages arising out of the use or inability
* to use the software.
*/

#ifndef DISCOVERY_CONSTANTS_H
#define DISCOVERY_CONSTANTS_H

/* chat_user captures the username (also used for DomainParticipant name) and 
 * the associated rtps obj IDs for each of the DP's 4 DataReaders and 
 * DataWriters
 */
struct CA_chat_user 
{
    char *username;
    const int rtps_obj_id_user_writer;
    const int rtps_obj_id_user_reader;
    const int rtps_obj_id_message_writer;
    const int rtps_obj_id_message_reader;
};

/* The chat_users array gives the developer a way to configure what remote 
 * DomainParticipants (names and rtps obj IDs) we *may* discover at runtime. 
 * Remember-- Connext Cert is limited to dpse discovery, so this a piori 
 * knowledge is required.
 * 
 * There is a multitude of ways you could approach this problem: this is one.
 */
struct CA_chat_user CA_chat_users[] =
{
    /*user          user    user    msg     msg */
    /*              DW      DR      DW      DR  */
    /*-------       ---     ---     ---     --- */
    { "don",        100,    110,    120,    130},
    { "reinier",    200,    210,    220,    230},   
    { "elena",      300,    310,    320,    330}, 
    { "tom",        400,    410,    420,    430},
    { "tron",       500,    510,    520,    530},
};

int CA_number_of_users = sizeof(CA_chat_users)/sizeof(struct CA_chat_user);

#endif