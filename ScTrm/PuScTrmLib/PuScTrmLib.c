// PuScTrmLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

__declspec(dllexport) unsigned int ScTrm_Open(void)
{
    TBS_RESULT result = 0;
    TBS_HCONTEXT hTBS = 0;

    TPM_DEVICE_INFO info = { TPM_VERSION_20, TPM_VERSION_20, 0, 0 };
    if ((result = Tbsi_GetDeviceInfo(sizeof(info), &info) != TBS_SUCCESS) ||
        (info.structVersion != 1) ||
        (info.tpmVersion != TPM_VERSION_20))
    {
        return (unsigned int)hTBS;
    }

    TBS_CONTEXT_PARAMS2 params = { TPM_VERSION_20, {{0, 0, 1}}};
    if ((((result = Tbsi_Context_Create((PCTBS_CONTEXT_PARAMS)&params, &hTBS)) != TBS_SUCCESS)) ||
        (hTBS == INVALID_HANDLE_VALUE) ||
        (hTBS == 0))
    {
        hTBS = 0;
        return (unsigned int)hTBS;
    }

    return (unsigned int)hTBS;
}

__declspec(dllexport) void ScTrm_Close(unsigned int context)
{
    TBS_HCONTEXT hTBS = (TBS_HCONTEXT)context;
    Tbsip_Context_Close(hTBS);
}

__declspec(dllexport) BOOL ScTrm_Cancel(unsigned int context)
{
    TBS_HCONTEXT hTBS = (TBS_HCONTEXT)context;
    return (Tbsip_Cancel_Commands(hTBS) == TBS_SUCCESS);
}

__declspec(dllexport) BOOL ScTrm_Execute(unsigned int context, unsigned char* pbCmd, unsigned int cbCmd, unsigned char* pbRsp, unsigned int cbRsp, unsigned int* pcbRsp)
{
    TBS_HCONTEXT hTBS = (TBS_HCONTEXT)context;
    *pcbRsp = cbRsp;
    return (Tbsip_Submit_Command(
        hTBS,
        TBS_COMMAND_LOCALITY_ZERO,
        TBS_COMMAND_PRIORITY_NORMAL,
        pbCmd,
        cbCmd,
        pbRsp,
        pcbRsp) == TBS_SUCCESS);
}
