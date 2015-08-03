/*
 * lg_rs232.c: lg rs232 remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <vdr/plugin.h>
#include <lgcontrol_service.h>

#include "lg_rs232.h"

cControlTVLGrs232 *lgcontrol = new cControlTVLGrs232;

// --- cControlTVLGrs232 -----------------------------------------------------------

cControlTVLGrs232::cControlTVLGrs232(void)
{
  SetActive(Name(), 1);
}

void cControlTVLGrs232::DoSet3DMode(int mode)
{
  int LGMode;
  dsyslog("3dcontrol: lg_rs232 mode:%i", mode);
  switch (mode) {
    case Disable:
         LGMode=LG_Disable3D;
         break;
    case SideBySide:
         LGMode=LG_SideBySide3D;
         break;
    case SideBySideLR:
         LGMode=LG_SideBySide3DLR;
         break;
    case TopBottom:
         LGMode=LG_TopBottom3D;
         break;
    case CheckBoard:
         LGMode=LG_CheckBoard3D;
         break;
    case FrameSequential:
         LGMode=LG_FrameSequential3D;
         break;
    case Converted2d:
         LGMode=LG_Converted2D;
         break;
    default:
         LGMode=LG_Disable3D;
         break;
  }

  cPlugin *p;
  LGRemote_Service_v1_0_t r;
  r.Command=LGMode;
  p = cPluginManager::GetPlugin("lgcontrol");
  if (p)
     p->Service(LGREMOTE_SERVICE, &r);
}
