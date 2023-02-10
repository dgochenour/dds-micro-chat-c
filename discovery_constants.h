
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

#define GROUP_1000 1000

struct chat_user 
{
    char *username;
    int group;
    const int rtps_obj_id_user_writer;
    const int rtps_obj_id_user_reader;
    const int rtps_obj_id_message_writer;
    const int rtps_obj_id_message_reader;
};

struct chat_user chat_users[] =
{
    /*user          group           user    user    msg     msg */
    /*                              DW      DR      DW      DR  */
    /*-------       ----------      ---     ---     ---     --- */
    { "don",        GROUP_1000,     100,    110,    120,    130},
    { "reinier",    GROUP_1000,     200,    210,    220,    230},   
    { "elena",      GROUP_1000,     300,    310,    320,    330}, 
    { "tom",        GROUP_1000,     400,    410,    420,    430},
    { "tron",       GROUP_1000,     500,    510,    520,    530},
};

#endif