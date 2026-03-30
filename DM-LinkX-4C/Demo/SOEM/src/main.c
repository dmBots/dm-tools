
#include "soem.h"
#include "linkx.h"
#include <stdio.h>


char adapter_name[16][128];

static uint8 IOmap[4*1024]={0};
static ecx_contextt ctx;

static linkx_t linkx;

void* thread_handle;

volatile bool thread_run=true;

void pdo_thread_func()
{
    while (thread_run)
    {
        linkx_send_pdos(&linkx);
        ecx_send_processdata(linkx.master);
        ecx_receive_processdata(linkx.master,EC_TIMEOUTRET);
        linkx_recv_pdos(&linkx);
        osal_usleep(40);
    }

}

int main()
{
    int expectedWKC;
    ec_adaptert *adapter = NULL;
    ec_adaptert *head = NULL;
    printf_s("SOEM (Simple Open EtherCAT Master)\n");
    fflush(stdout);
    printf_s("dm_linkx_soem_demo!\n");
    fflush(stdout);

    printf_s("\nAvailable adapters:\n");
    fflush(stdout);
    head = adapter = ec_find_adapters();
    for (int i=0; adapter != NULL; i++)
    {
        printf_s("   %d - %s  (%s)\n",i, adapter->name, adapter->desc);
        fflush(stdout);
        strcpy_s(adapter_name[i],128, adapter->name);
        adapter = adapter->next;
    }
    //ec_free_adapters(head);

    int adapter_index=-1;

    printf_s("\nSelect adapter index: ");
    fflush(stdout);
    scanf_s("%d", &adapter_index);

    if (adapter_index==-1)
        return 0;

    printf_s("\nInitializing adapter %s\n", adapter_name[adapter_index]);
    fflush(stdout);
    if (!ecx_init(&ctx, adapter_name[adapter_index]))
    {
        printf_s("Initialization failed!\n");
        return 0;
    }

    printf_s("Initialization successful!\n");
    fflush(stdout);
    ec_groupt *group = &ctx.grouplist[0];


    int slave_cnt=ecx_config_init(&ctx);
    // while (ctx.ecaterror)
    //     printf_s("%s", ecx_elist2string(&ctx));


    fflush(stdout);
    if (slave_cnt<=0)
    {
        printf_s("No slaves found!\n");
        return 0;
    }

    printf_s(" %d slaves found \n",slave_cnt);

    int io_size=ecx_config_map_group(&ctx, IOmap, 0);


    ecx_configdc(&ctx);
    ecx_dcsync0(&ctx,1,true, 50*1000, 10*1000);

    ecx_statecheck(&ctx, 0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 2);


    ecx_send_processdata(&ctx);
    ecx_receive_processdata(&ctx, EC_TIMEOUTRET);


    linkx_init(&linkx,1,&ctx);


    for (int i=0;i<LINKX_CAN_CHANNEL_NUM;i++)
    {
        linkx_switch_can_channel(&linkx,i,true);
    }

    if (linkx_start(&linkx))
    {
        osal_thread_create_rt(&thread_handle,4*1024*1024, pdo_thread_func, NULL);
        printf_s("pdo thread started!\n");
    }

    while (thread_run)
    {


    }


    ec_free_adapters(head);
    return 0;
}
