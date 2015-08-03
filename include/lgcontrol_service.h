#ifndef __LGCONTROL_SERVICE_H
#define __LGCONTROL_SERVICE_H

#define LGREMOTE_SERVICE "LGRemoteService-v1.0"

enum _lg_commands_
{
    LG_Disable3D,
    LG_SideBySide3D,
    LG_SideBySide3DLR,
    LG_TopBottom3D,
    LG_CheckBoard3D,
    LG_FrameSequential3D,
    LG_Converted2D,
    LG_SetVolume,
    LG_GetVolume,
    LG_PowerOff,
    LG_PowerOn,
    LG_TogglePower,
    LG_GetCurrentPowerStatus,
    LG_ToggleMute,
    LG_IsMute,
    LG_ToggleRemoteLocked,
    LG_IsRemoteLocked,
};

typedef struct
{
    int Command;
    int Option;
    int rc;
} LGRemote_Service_v1_0_t;

#endif
