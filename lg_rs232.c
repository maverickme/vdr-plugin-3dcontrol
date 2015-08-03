/*
 * lg_rs232.c: lg rs232 remote control
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <cstdlib>
#include <sys/time.h>

#include <vdr/tools.h>
#include "control.h"
#include "3dcontrol.h"
#include "lg_rs232.h"

#define TIMEOUT                  1.0
#define READ_STATUS              0xFF

#define CONVERTED_2D_3D_DEPTH    14

cControlTVLGrs232 *lgrs232 = new cControlTVLGrs232;

int fd = -1;
int baudrate = 9600;
int set_id = 0;

void cControlTVLGrs232::InitSerial(const char *device)
{
  ssize_t r = 0;
  if (fd != -1) return;
  dsyslog("3dservice: lg_rs232 open: %s\n", device);

  fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0) {
     dsyslog("3dservice: lg_rs232 Cannot open: %s\n", device);
     return;
     }
  tcflush(fd, TCIOFLUSH);

  struct termios tios;
  memset(&tios, 0, sizeof(tios));
  cfsetispeed(&tios, baudrate);
  cfsetospeed(&tios, baudrate);
  tios.c_cflag = CS8 | CLOCAL | CREAD;
  tios.c_iflag = IGNPAR | IGNBRK | IXANY;
  tios.c_lflag = ISIG;
  tios.c_cc[VMIN] = 0;
  tios.c_cc[VTIME] = 0;
  tcsetattr(fd, TCSANOW, &tios);

  tcflush(fd, TCIOFLUSH);
  r=write(fd, "\r", 1);
  r=r;
}

void cControlTVLGrs232::CloseSerial(void)
{
  if (fd != -1) close(fd);
}

/*
 * Send command to LG TV
 *
 * value can be READ_STATUS to read current value
 *
 * return:
 * -256 = timeout/unknown response
 * +n = OK
 * -n = NG
 *   -1 illegal code
 *   -2 not supported function
 *   -3 wait more time
 *   hmm.. really?
 */
int cControlTVLGrs232::SendCommand(char cmd1, char cmd2, int value, int value2, int value3, int value4)
{
  if (fd < 0) return -256;
  ssize_t r = 0;
  char cmd[30];
  int len;
  int extvalues=value2+value3+value4;
  if ( extvalues > 0 )
     len = sprintf(cmd, "%c%c %02x %02x %02x %02x %02x\r", cmd1, cmd2, set_id, value, value2, value3, value4);
  else {
     if (value >= 0x100)
        len = sprintf(cmd, "%c%c %02x %02x %02x\r", cmd1, cmd2, set_id, value>>8, value&255);
     else
        len = sprintf(cmd, "%c%c %02x %02x\r", cmd1, cmd2, set_id, value);
     }

  r=write(fd, cmd, len);
  r=r;
  tcdrain(fd);
  memset(cmd, 0, sizeof(cmd));
  char *p = cmd;

  struct timeval tv1; gettimeofday(&tv1, NULL);
  double t1 = tv1.tv_sec + (tv1.tv_usec / 1000000.0);

  while (true) {
    struct timeval tv2; gettimeofday(&tv2, NULL);
    double t2 = tv2.tv_sec + (tv2.tv_usec / 1000000.0);
    if (t2 > t1 + TIMEOUT) {
       dsyslog("3dservice: lg_rs232 Response timeout: %s\n", cmd);
       return -256;
       }
    int n = read(fd, p, 1);
    if (n < 0)
       usleep(1000);
    else if (n == 0)
       usleep(1000);
    else if (p - cmd < (int)sizeof(cmd)) {
       if (p == cmd && cmd[0] == ' ') { cmd[1] = cmd[0]; cmd[0] = cmd2; p++; }
       if (*p == 'x') {
          char cmd2r;
          int id;
          char ok_ng[2];
          int data;
          if (sscanf(cmd, "%c %02x %2c%x", &cmd2r, &id, ok_ng, &data) >= 4) {
             if (cmd2r == cmd2) {
                if (ok_ng[0] == 'O' && ok_ng[1] == 'K') {
                   if ((value == READ_STATUS) || (value == data)) {
                      return data;
                      }
                   }
                else if (ok_ng[0] == 'N' && ok_ng[1] == 'G') {
                   return -data;
                   }
                }
                dsyslog("3dservice: lg_rs232 Unknown response: %s\n", cmd);
                return -256;
             }
          }
          p++;
       }
  }
}

void cControlTVLGrs232::DoInit(void)
{
  if (!config.lg_rs232)
     return;
  if (config.LGDevice != NULL)
      InitSerial(config.LGDevice);
     else
      InitSerial(LGDEVICE);
}

void cControlTVLGrs232::DoStop(void)
{
  if (!config.lg_rs232)
     return;
  CloseSerial();
}

void cControlTVLGrs232::DoSet3DMode(int mode)
{
  if (!config.lg_rs232)
     return;
  dsyslog("3dservice: lg_rs232 mode:%i", mode);
  switch (mode) {
    case Disable:
         SendCommand('x', 't', 1, 1, 1);
         break;
    case SideBySide:
         SendCommand('x', 't', 0, 1, 0);
         break;
    case SideBySideLR:
         SendCommand('x', 't', 0, 1, 1);
         break;
    case TopBottom:
         SendCommand('x', 't', 0, 0);
         break;
    case CheckBoard:
         SendCommand('x', 't', 0, 2);
         break;
    case FrameSequential:
         SendCommand('x', 't', 0, 3);
         break;
    case Converted2d:
         SendCommand('x', 't', 3, 0, 0, CONVERTED_2D_3D_DEPTH);  // 2d -> 3d
         break;
    default:
         SendCommand('x', 't', 1, 1, 1);
         break;
  }
}

