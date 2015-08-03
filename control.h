#include <vdr/tools.h>

#ifndef CONTROL_H
#define CONTROL_H

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

class cControlTV : public cListObject {
private:
  static cList<cControlTV> Controllers;
  int Current3dMode;
protected:
  virtual void Set3DMode(int mode) {}
  virtual void Init(void) {}
  virtual void Stop(void) {}
public:
  cControlTV();
  virtual ~cControlTV();
  static void MsgSet3DMode(int mode);
  static void MsgInit(void);
  static void MsgStop(void);
  };

extern cControlTV ControlTV;

#endif
