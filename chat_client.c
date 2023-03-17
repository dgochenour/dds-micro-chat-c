#include <stdio.h>
#include <string.h>

#include "chat_client.h"
#include "nic_config.h"
#include "discovery_constants.h"

int CA_find_user_index(char *user)
{
    for (int i = 0; i < CA_number_of_users; i++)
    {
        if (!strcmp(user, CA_chat_users[i].username)) return i;
    }
    return -1;
}

void CA_configure_udp_transport(struct UDP_InterfaceFactoryProperty *udp_property) 
{
    *udp_property = UDP_INTERFACE_FACTORY_PROPERTY_DEFAULT;
    // For additional allowed interface(s), increase maximum and length, and
    // set interface below:
    REDA_StringSeq_set_maximum(&udp_property->allow_interface,2);
    REDA_StringSeq_set_length(&udp_property->allow_interface,2);
    *REDA_StringSeq_get_reference(&udp_property->allow_interface,0) = 
            DDS_String_dup(loopback_name); 
    *REDA_StringSeq_get_reference(&udp_property->allow_interface,1) = 
            DDS_String_dup(eth_nic_name); 
}

DDS_ReturnCode_t CA_configure_dp_qos(struct DDS_DomainParticipantQos *dp_qos, int myself_index)
{
    if(!RT_ComponentFactoryId_set_name(&(dp_qos->discovery.discovery.name), "dpse")) 
    {
        printf("ERROR: failed to set discovery plugin name\n");
        return DDS_RETCODE_ERROR;
    }
    if(!DDS_StringSeq_set_maximum(&(dp_qos->discovery.initial_peers), 1)) 
    {
        printf("ERROR: failed to set initial peers maximum\n");
        return DDS_RETCODE_ERROR;
    }
    if (!DDS_StringSeq_set_length(&(dp_qos->discovery.initial_peers), 1)) 
    {
        printf("ERROR: failed to set initial peers length\n");
        return DDS_RETCODE_ERROR;
    }
    *DDS_StringSeq_get_reference(&(dp_qos->discovery.initial_peers), 0) = 
            DDS_String_dup(peer);

    dp_qos->resource_limits.max_destination_ports = 8;
    dp_qos->resource_limits.max_receive_ports = 8;

    /* For remote_participant_allocation we are assuming that there won't be 
     * more remote remote DomainParticipants than there are users in the system, 
     * since each client uses a single DP
     */
    dp_qos->resource_limits.remote_participant_allocation = CA_number_of_users;
    
    /* Each chat client interacts with 2 Topics, each with its own type */ 
    dp_qos->resource_limits.local_topic_allocation = 2;
    dp_qos->resource_limits.local_type_allocation = 2;

    /* Each chat client has 2 DataWriters and 2 DataReaders: one each for the
     * "User Info Topic" Topic and for the "Message Topic" Topic. 
     */
    dp_qos->resource_limits.local_reader_allocation = CA_number_of_users * 2;
    dp_qos->resource_limits.local_writer_allocation = CA_number_of_users * 2;
    dp_qos->resource_limits.remote_reader_allocation = CA_number_of_users * 2;
    dp_qos->resource_limits.remote_writer_allocation = CA_number_of_users * 2;

    strcpy(dp_qos->participant_name.name, CA_chat_users[myself_index].username);

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t CA_register_types(DDS_DomainParticipant *dp) 
{
    DDS_ReturnCode_t retcode;
    retcode = DDS_DomainParticipant_register_type(
            dp,
            Chat_UserTypePlugin_get_default_type_name(),
            Chat_UserTypePlugin_get());
    if (retcode != DDS_RETCODE_OK) 
    {
        printf("ERROR: failed to register Chat_User type\n");
        return retcode;
    }
    retcode = DDS_DomainParticipant_register_type(
            dp,
            Chat_MessageTypePlugin_get_default_type_name(),
            Chat_MessageTypePlugin_get());
    if (retcode != DDS_RETCODE_OK) 
    {
        printf("ERROR: failed to register Chat_Message type\n");
        return retcode;
    }
    return retcode;
}

DDS_ReturnCode_t CA_configure_dw_qos(struct DDS_DataWriterQos *dw_qos, int myself_index, struct TopicInfo *topic_info)
{
    /* The RTPS Object ID will be unique, depending on which DataWriter we are
     * configuring. Additionally, we may want to set Topic-dependent QoS 
     */
    if (strcmp(topic_info->topic_name , Chat_k_MESSAGE_TOPIC_NAME) == 0) 
    {
        dw_qos->protocol.rtps_object_id = CA_chat_users[myself_index].rtps_obj_id_message_writer;
    }
    else if (strcmp(topic_info->topic_name , Chat_k_USER_TOPIC_NAME) == 0)
    {
        dw_qos->protocol.rtps_object_id = CA_chat_users[myself_index].rtps_obj_id_user_writer;        
    }
    else 
    {
        /* what Topic are we talking about anyway? */
        return DDS_RETCODE_ERROR;
    }

    dw_qos->reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;

    /* For max_remote_readers we are assuming that there won't be more remote
     * DataReaders than there are users in the system, since each client has 1
     */
    dw_qos->writer_resource_limits.max_remote_readers = CA_number_of_users;

    /* Allow for enough instances to have CA_number_of_users, all in different
     * groups 
     */
    dw_qos->resource_limits.max_instances = CA_number_of_users * 2;

    dw_qos->resource_limits.max_samples_per_instance = 8;
    dw_qos->resource_limits.max_samples = dw_qos->resource_limits.max_instances *
            dw_qos->resource_limits.max_samples_per_instance;
    dw_qos->history.depth = 8;
    dw_qos->protocol.rtps_reliable_writer.heartbeat_period.sec = 0;
    dw_qos->protocol.rtps_reliable_writer.heartbeat_period.nanosec = 250000000;

    return DDS_RETCODE_OK;

}

int main(int argc, char* argv[])
{     

    if (argc < 3) 
    {
        printf("Usage: chat_app <username> <group>\n");
        return -1;
    }

    char *myself_name = argv[1];
    int myself_group = atoi(argv[2]);
    int myself_index = CA_find_user_index(myself_name);
    
    if (myself_index == -1) 
    {
        printf("ERROR: You have specified a user that is not part of the peer list!\n");
        printf("ERROR: Peer list:\n");
        for (int i = 0; i < CA_number_of_users; i++)
        {
            printf("\t- %s\n", CA_chat_users[i].username);
        }
        return EXIT_FAILURE;
    }

    printf("INFO: Starting chat client\n");

    DDS_ReturnCode_t retcode;

    struct ApplicationInfo application;
    struct TopicInfo user_topic;
    struct TopicInfo message_topic;

    DDS_DomainParticipantFactory *dpf = 
        DDS_DomainParticipantFactory_get_instance();
    RT_Registry_T *registry = 
        DDS_DomainParticipantFactory_get_registry(dpf);

    if (!RT_Registry_register(
            registry, 
            DDSHST_WRITER_DEFAULT_HISTORY_NAME,
            WHSM_HistoryFactory_get_interface(), 
            NULL, 
            NULL))
    {
        printf("ERROR: failed to register wh\n");
    }
    if (!RT_Registry_register(
            registry, 
            DDSHST_READER_DEFAULT_HISTORY_NAME,
            RHSM_HistoryFactory_get_interface(), 
            NULL, 
            NULL))
    {
        printf("ERROR: failed to register rh\n");
    }

    if(!RT_Registry_unregister(
            registry, 
            NETIO_DEFAULT_UDP_NAME, 
            NULL, 
            NULL)) 
    {
        printf("ERROR: failed to unregister udp\n");
    }

    struct UDP_InterfaceFactoryProperty *udp_property = NULL;
    udp_property = (struct UDP_InterfaceFactoryProperty *)
            malloc(sizeof(struct UDP_InterfaceFactoryProperty));
    if (udp_property == NULL) {
        printf("ERROR: failed to allocate udp properties\n");
    }
    CA_configure_udp_transport(udp_property);

    if(!RT_Registry_register(
        registry, 
        NETIO_DEFAULT_UDP_NAME,
        UDP_InterfaceFactory_get_interface(),
        (struct RT_ComponentFactoryProperty*)udp_property, NULL))
    {
        printf("ERROR: failed to re-register udp\n");
    }     

    struct DPSE_DiscoveryPluginProperty discovery_plugin_properties =
            DPSE_DiscoveryPluginProperty_INITIALIZER;
    struct DDS_Duration_t my_lease = {5,0};
    struct DDS_Duration_t my_assert_period = {2,0};
    discovery_plugin_properties.participant_liveliness_lease_duration = my_lease;
    discovery_plugin_properties.participant_liveliness_assert_period = my_assert_period;
    if (!RT_Registry_register(
            registry,
            "dpse",
            DPSE_DiscoveryFactory_get_interface(),
            &discovery_plugin_properties._parent, 
            NULL))
    {
        printf("ERROR: failed to register dpse\n");
    }

    /* Before we create DDS entities, we'll set autoenable_created_entities to 
     * "false". By making this "false" until after all of the DDS entities
     * are created, we are assured that all DDS-realted memory allocation is 
     * complete before the DDS middleware is active on the network.
     */
    struct DDS_DomainParticipantFactoryQos dpf_qos = 
            DDS_DomainParticipantFactoryQos_INITIALIZER;
    DDS_DomainParticipantFactory_get_qos(dpf, &dpf_qos);
    dpf_qos.entity_factory.autoenable_created_entities = DDS_BOOLEAN_FALSE;
    DDS_DomainParticipantFactory_set_qos(dpf, &dpf_qos);

    struct DDS_DomainParticipantQos dp_qos = 
            DDS_DomainParticipantQos_INITIALIZER;
    CA_configure_dp_qos(&dp_qos, myself_index);

    /* create the local DomainParticipant */
    application.dp = DDS_DomainParticipantFactory_create_participant(
            dpf, 
            domain_id,
            &dp_qos, 
            NULL,
            DDS_STATUS_MASK_NONE);
    if(application.dp == NULL) {
        printf("ERROR: failed to create participant\n");
    }

    /* register types */
    retcode = CA_register_types(application.dp);
    if (retcode != DDS_RETCODE_OK) 
    {
        printf("ERROR: failed to register types\n");
    }

    /* create the Publisher */
    application.publisher = DDS_DomainParticipant_create_publisher(
            application.dp,
            &DDS_PUBLISHER_QOS_DEFAULT,
            NULL,
            DDS_STATUS_MASK_NONE);
    if(application.publisher == NULL) 
    {
        printf("ERROR: application.publisher == NULL\n");
    }

    /* create the Subscriber */
    application.subscriber = DDS_DomainParticipant_create_subscriber(
            application.dp,
            &DDS_SUBSCRIBER_QOS_DEFAULT,
            NULL,
            DDS_STATUS_MASK_NONE);
    if(application.subscriber == NULL) 
    {
        printf("ERROR: application.subscriber == NULL\n");
    }

    strcpy(user_topic.topic_name, Chat_k_USER_TOPIC_NAME);
    user_topic.topic = DDS_DomainParticipant_create_topic(
            application.dp,
            user_topic.topic_name, 
            Chat_UserTypePlugin_get_default_type_name(),
            &DDS_TOPIC_QOS_DEFAULT, 
            NULL,
            DDS_STATUS_MASK_NONE);
    if(user_topic.topic == NULL) 
    {
        printf("ERROR: user_topic.topic == NULL\n");
    }

    struct DDS_DataWriterQos dw_qos = DDS_DataWriterQos_INITIALIZER;
    CA_configure_dw_qos(&dw_qos, myself_index, &user_topic);

    strcpy(message_topic.topic_name, Chat_k_MESSAGE_TOPIC_NAME);
    message_topic.topic = DDS_DomainParticipant_create_topic(
            application.dp,
            message_topic.topic_name, 
            Chat_MessageTypePlugin_get_default_type_name(),
            &DDS_TOPIC_QOS_DEFAULT, 
            NULL,
            DDS_STATUS_MASK_NONE);
    if(message_topic.topic == NULL) 
    {
        printf("ERROR: message_topic.topic == NULL\n");
    }

    /* TODO: create user_topic.dw */

    /* TODO: create user_topic.dr */

    /* TODO: create message_topic.dw */

    /* TODO: create message_topic.dr */

    /* TODO: assert remote DPs*/

    /* TODO: assert remote DWs and DRs */

    /* now that all of the entities are created, we can enable them all */
    printf("INFO: Enabling DDS entities\n");
    retcode = DDS_Entity_enable(DDS_DomainParticipant_as_entity(application.dp));
    if(retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to enable entity\n");
    }

    /* TODO: create thread for user_topic.dw */

    /* TODO: create thread for user_topic.dr */

    /* TODO: create thread for message_topic.dr */

    /* TODO: loop to take user commands and send messages */

    return EXIT_SUCCESS;
}