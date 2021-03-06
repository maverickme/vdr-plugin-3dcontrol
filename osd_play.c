/*
 * osd_play.c: play plugin remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <vdr/plugin.h>
#include <play_service.h>

#include "osd_play.h"

cControlTVOSDPlayPlugin *osdplay = new cControlTVOSDPlayPlugin;

cControlTVOSDPlayPlugin::cControlTVOSDPlayPlugin(void)
{
  SetActive(Name(), 0);
}

void cControlTVOSDPlayPlugin::DoSet3DMode(int mode)
{
  int Play3dMode;
  switch (mode) {
    case SideBySide:
    case SideBySideLR:
         Play3dMode=1;
         break;
    case TopBottom:
         Play3dMode=2;
         break;
    default:
         Play3dMode=0;
         break;
  }

  cPlugin *p;
  Play_Osd3DModeService_v1_0_t r;
  r.Mode=Play3dMode;
  p = cPluginManager::GetPlugin("play");
  if (p)
     p->Service(PLAY_OSD_3DMODE_SERVICE, &r);
}
