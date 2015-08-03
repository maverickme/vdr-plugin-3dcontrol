/*
 * lg_rs232.h: lg rs232 remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __LG_RS232_H
#define __LG_RS232_H

#define LGDEVICE                 "/dev/ttyUSB0"

class cControlTVLGrs232 : public cControlTV {
private:
  int SendCommand(char cmd1, char cmd2, int value, int value2=0, int value3=0, int value4=0);
  void InitSerial(const char *device);
  void CloseSerial(void);
protected:
  virtual void DoInit(void);
  virtual void DoStop(void);
  virtual void DoSet3DMode(int mode);
//  virtual const char *GetCommandLineParameters(void) { return *CLiP; }
public:
  };

#endif //__LG_RS232_H
