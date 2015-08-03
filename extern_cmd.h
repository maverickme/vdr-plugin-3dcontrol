/*
 * extern_cmd.h: call extern script 
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __EXTERN_CMD_H
#define __EXTERN_CMD_H

#include "control.h"

class cControlTVCmdExtern : public cControlTV {
private:
  char *ExternalScript;
protected:
  virtual void DoSet3DMode(int mode);
  virtual bool DoProcessArgs(int argc, char *argv[]);
public:
  cControlTVCmdExtern(void);
  virtual const char *Name(void) { return "extern_cmd"; }
  virtual const char *Description(void) { return "Run external Commands"; }
  virtual const char *CommandLineHelp(void) { return "  -s SCRIPT,\t\t--script=SCRIPT"; }
  };

#endif
