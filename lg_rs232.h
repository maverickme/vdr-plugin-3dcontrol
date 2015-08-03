/*
 * lg.h: lg rs232 remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __LG_RS232_H
#define __LG_RS232_H

#define READ_STATUS              0xFF

#define LGMINVOL                 0
#define LGMAXVOL                 64

#define LGMINBACKLIGTH           0
#define LGMAXBACKLIGTH           64

#define CONVERTED_2D_3D_DEPTH    14

class cControlTVLGrs232 : public cControlTV {
private:
  int SendCommand(char cmd1, char cmd2, int value, int value2=0, int value3=0, int value4=0);
  void InitSerial(const char *device);
  void CloseSerial(void);
protected:
  virtual void Set3DMode(int mode);
public:
  };

#endif //__LG_RS232_H
