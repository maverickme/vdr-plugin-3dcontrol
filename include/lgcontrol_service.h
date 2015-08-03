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
};

typedef struct
{
    int Command;
} LGRemote_Service_v1_0_t;

#endif
