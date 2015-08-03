/*
 * lg_rs232.h: lg rs232 remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __LG_RS232_H
#define __LG_RS232_H

#include "control.h"

// --- cControlTVLGrs232 ---------------------------------------------------------

class cControlTVLGrs232 : public cControlTV {
private:
protected:
  virtual void DoSet3DMode(int mode);
public:
  cControlTVLGrs232(void);
  virtual const char *Name(void) { return "lg_rs232";}
  virtual const char *Description(void) { return "Control LG TV via RS232 (need lgcontrol)"; }
  };

#endif //__LG_RS232_H
