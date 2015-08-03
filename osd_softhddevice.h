/*
 * lg.h: lg rs232 remote control
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
  virtual void Set3DMode(int mode);
public:
  };


#endif //__LG_H
