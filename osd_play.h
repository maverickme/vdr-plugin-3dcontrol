/*
 * osd_play.h: play plugin remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __OSD_PLAY_H
#define __OSD_PLAY_H

#include "control.h"

class cControlTVOSDPlayPlugin : public cControlTV {
private:
protected:
  virtual void DoSet3DMode(int mode);
public:
  cControlTVOSDPlayPlugin(void);
  virtual const char *Name(void) { return "osd_play"; }
  virtual const char *Description(void) { return "Control Play Plugin OSD"; }
  };

#endif
