#include "Hooks.h"

#include "../Interfaces.h"
#include "../Utils/Util.h"

typedef INetChannel* (* CreateNetChannelFn)( CNetworkSystem*, int, void *, const char *, unsigned int, unsigned int );

// This will not work for splitscreen slots ( if somehow this code needs to be used for that )
INetChannel* Hooks::CreateNetChannel( CNetworkSystem *thisptr, int unk, void *ns_addr, const char *str, unsigned int uUnk, unsigned int uUnk2 ) {
    delete netChannelVMT;
    INetChannel *ret = networkSystemVMT->GetOriginalMethod<CreateNetChannelFn>( 28 )( thisptr, unk, ns_addr, str, uUnk, uUnk2 );

    MC_PRINTF( "Grabbing new NetChannel VMT - %p\n", (void*)ret );
    netChannelVMT = new VMT( ret );
    netChannelVMT->HookVM( Hooks::SendNetMessage, 66 );
    netChannelVMT->HookVM( Hooks::PostReceivedNetMessage, 84 );
    netChannelVMT->ApplyVMT();

    return ret;
}