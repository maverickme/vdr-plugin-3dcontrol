/*
 * control.h
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

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
  int active;
  static int GetCurrent3DMode(void) { return Current3DMode; }
  static void SetCurrent3DMode(int mode) { Current3DMode = mode; }
protected:
  virtual void DoSet3DMode(int mode) {}
  virtual void DoInit(void) {}
  virtual void DoStop(void) {}
  virtual bool DoProcessArgs(int argc, char *argv[]) { return true; }
public:
  cControlTV();
  virtual ~cControlTV();
  static cList<cControlTV> Controllers;
  static void Set3DMode(int mode);
  virtual const char *Name(void) { return NULL; }
  virtual const char *Description(void) { return NULL; }
  virtual const char *CommandLineHelp(void) { return NULL; }
  virtual bool Active(void) { return active; }
  virtual void SetActive(const char *modul, bool value);
  static bool ProcessArgs(int argc, char *argv[]);
  static void Init(const char *ActiveModules);
  static void Stop(void);
  };

extern cControlTV ControlTV;

#endif
