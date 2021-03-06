/*
 * osd_softhddevice.c: softhddevice plugin remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <vdr/plugin.h>
#include <softhddevice_service.h>
#include "osd_softhddevice.h"


cControlTVOSDSoftHdDevicePlugin *osdsofthddevice = new cControlTVOSDSoftHdDevicePlugin;

cControlTVOSDSoftHdDevicePlugin::cControlTVOSDSoftHdDevicePlugin(void)
{
  SetActive(Name(), 1);
}

void cControlTVOSDSoftHdDevicePlugin::DoSet3DMode(int mode)
{
  int SoftHdDevice3dMode;
  switch (mode) {
    case SideBySide:
    case SideBySideLR:
         SoftHdDevice3dMode=1;
         break;
    case TopBottom:
         SoftHdDevice3dMode=2;
         break;
    default:
         SoftHdDevice3dMode=0;
         break;
  }

  cPlugin *p;
  SoftHDDevice_Osd3DModeService_v1_0_t r;
  r.Mode=SoftHdDevice3dMode;
  p = cPluginManager::GetPlugin("softhddevice");
  if (p)
     p->Service(OSD_3DMODE_SERVICE, &r);
}
