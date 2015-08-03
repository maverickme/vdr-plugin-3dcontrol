/*
 * extern_cmd.c: call extern script
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <getopt.h>
#include <vdr/plugin.h>
#include "extern_cmd.h"

cControlTVCmdExtern *cmdextern = new cControlTVCmdExtern;

cControlTVCmdExtern::cControlTVCmdExtern(void)
{
  ExternalScript=NULL;
  SetActive(Name(), 0);
}

void cControlTVCmdExtern::DoSet3DMode(int mode)
{
  if (ExternalScript == NULL)
     return;
  cString cmd;
  cmd = cString::sprintf("%s %i", ExternalScript, mode);
  dsyslog("3dcontrol: extern_cmd - run: %s mode: %i",ExternalScript, mode);
  SystemExec(cmd);
}

bool cControlTVCmdExtern::DoProcessArgs(int argc, char *argv[])
{
  static struct option long_options[] = {
       { "script",   no_argument, NULL, 's' },
       { NULL,       no_argument,       NULL,  0  }
     };

  int c;
  while ((c = getopt_long(argc, argv, "s:", long_options, NULL)) != -1) {
        switch (c) {
          case 's': ExternalScript=optarg;
                    SetActive(Name(), 1);
                    break;
          }
        }
  return true;
}
