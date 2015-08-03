#include <vdr/tools.h>

#ifndef __CONTROL_H
#define __CONTROL_H

enum _tv_3d_mode_
{
    Disable,
    SideBySide,
    SideBySideLR,
    TopBottom,
    CheckBoard,
    FrameSequential,
    Converted2d,
};

static int Current3DMode;

class cControlTV : public cListObject {
private:
  static cList<cControlTV> Controllers;
  static int GetCurrent3DMode(void) { return Current3DMode; }
  static void SetCurrent3DMode(int mode) { Current3DMode = mode; }
protected:
  virtual void DoSet3DMode(int mode) {}
  virtual void DoInit(void) {}
  virtual void DoStop(void) {}
public:
  cControlTV();
  virtual ~cControlTV();
  static void Set3DMode(int mode);
  static void Init(void);
  static void Stop(void);
  };

extern cControlTV ControlTV;

#endif
