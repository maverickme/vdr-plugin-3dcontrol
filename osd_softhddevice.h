/*
 * osd_softhddevice.c: softhddevice plugin remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __OSD_SOFTHDDEVICE_H
#define __OSD_SOFTHDDEVICE_H

#include "control.h"

class cControlTVOSDSoftHdDevicePlugin : public cControlTV {
private:
protected:
  virtual void DoSet3DMode(int mode);
public:
  cControlTVOSDSoftHdDevicePlugin(void);
  virtual const char *Name(void) { return "osd_softhddevice"; }
  virtual const char *Description(void) { return "Control SoftHDDevice OSD"; }
  };

#endif
