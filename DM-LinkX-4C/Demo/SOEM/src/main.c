
#include "soem.h"
#include "linkx.h"
#include <stdio.h>


char adapter_name[16][128];

static uint8 IOmap[4096];
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
        osal_usleep(10);
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

    ecx_config_map_group(&ctx, IOmap, 0);

    ecx_configdc(&ctx);
    while (ctx.ecaterror)
        printf_s("%s", ecx_elist2string(&ctx));

    ecx_config_init(&ctx);
    printf_s("%d slaves found and configured.\n", ctx.slavecount);
    fflush(stdout);
    if (ctx.slavecount==0)
        return 0;

    ecx_statecheck(&ctx, 0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 2);
    // ecx_send_processdata(&ctx);
    // ecx_receive_processdata(&ctx, EC_TIMEOUTRET);



    linkx_init(&linkx,1,&ctx);

    if (linkx_start(&linkx))
        osal_thread_create_rt(&thread_handle,8*1024*1024, pdo_thread_func, NULL);

    // bool result=false;
    // result=linkx_read_baudrate(&linkx,0);
    // printf_s("%s",linkx_get_error_string(&linkx));
    // fflush(stdout);
    // result=linkx_read_baudrate(&linkx,1);
    // result=linkx_read_baudrate(&linkx,2);
    // result=linkx_read_baudrate(&linkx,3);

    ec_free_adapters(head);
    return 0;
}
