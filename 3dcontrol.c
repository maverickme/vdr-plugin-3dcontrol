/*
 * 3dcontrol.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <vdr/plugin.h>
#include <vdr/status.h>
#include "control.h"

static const char *VERSION        = "0.0.1";
static const char *DESCRIPTION    = "3D control plugin";
static const char *MAINMENUENTRY  = NULL;

cControlTV *control = new cControlTV();

// --- cMyStatusMonitor ---------------------------------------------------

class cMyStatusMonitor : public cStatus {
private:
  char *stristr(const char *String, const char *Pattern);
protected:
  virtual void Replaying(const cControl *Control, const char *Name, const char *FileName, bool On);
  virtual void ChannelSwitch(const cDevice* Device, int ChannelNumber, bool LiveView);
  };

char *cMyStatusMonitor::stristr(const char *String, const char *Pattern)
{
  char *pptr, *sptr, *start;
  for (start = (char *)String; *start; start++) {
      for ( ; (*start && (toupper(*start) != toupper(*Pattern))); start++)
                  ;
          if (!*start)
             return 0;
          pptr = (char *)Pattern;
          sptr = (char *)start;
          while (toupper(*sptr) == toupper(*pptr)) {
                sptr++;
                pptr++;
                if (!*pptr)
                   return (start);
          }
      }
  return 0;
}

void cMyStatusMonitor::Replaying(const cControl *Control, const char *Name, const char *FileName, bool On)
{
  if (On) {
     if (stristr(FileName, "3D")) {
        if (stristr(FileName, "OU") || stristr(FileName, "TB"))
           control->MsgSet3DMode(TopBottom);
        else
           control->MsgSet3DMode(SideBySide);
        }
     else
        control->MsgSet3DMode(Disable);
     }
  else
     control->MsgSet3DMode(Disable);
}

void cMyStatusMonitor::ChannelSwitch(const cDevice* Device, int ChannelNumber, bool LiveView)
{
  if (LiveView && ChannelNumber > 0) {
     cChannel *Channel = Channels.GetByNumber(cDevice::CurrentChannel());
     if (stristr(Channel->Name(), "3D")) {
        control->MsgSet3DMode(SideBySide);
        return;
        }
     }
  control->MsgSet3DMode(Disable);
}

class cPlugin3dcontrol : public cPlugin {
private:
  // Add any member variables or functions you may need here.
  cMyStatusMonitor     *statusMonitor;
public:
  cPlugin3dcontrol(void);
  virtual ~cPlugin3dcontrol();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return DESCRIPTION; }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual void MainThreadHook(void);
  virtual cString Active(void);
  virtual time_t WakeupTime(void);
  virtual const char *MainMenuEntry(void) { return MAINMENUENTRY; }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  virtual bool Service(const char *Id, void *Data = NULL);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
  };

cPlugin3dcontrol::cPlugin3dcontrol(void)
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
  statusMonitor = NULL;
}

cPlugin3dcontrol::~cPlugin3dcontrol()
{
  // Clean up after yourself!
  delete statusMonitor;
}

const char *cPlugin3dcontrol::CommandLineHelp(void)
{
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPlugin3dcontrol::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPlugin3dcontrol::Initialize(void)
{
  // Initialize any background activities the plugin shall perform.
  control->MsgInit();
  return true;
}

bool cPlugin3dcontrol::Start(void)
{
  // Start any background activities the plugin shall perform.
  statusMonitor=new cMyStatusMonitor;
  return true;
}

void cPlugin3dcontrol::Stop(void)
{
  // Stop any background activities the plugin is performing.
  control->MsgStop();
}

void cPlugin3dcontrol::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

void cPlugin3dcontrol::MainThreadHook(void)
{
  // Perform actions in the context of the main program thread.
  // WARNING: Use with great care - see PLUGINS.html!
}

cString cPlugin3dcontrol::Active(void)
{
  // Return a message string if shutdown should be postponed
  return NULL;
}

time_t cPlugin3dcontrol::WakeupTime(void)
{
  // Return custom wakeup time for shutdown script
  return 0;
}

cOsdObject *cPlugin3dcontrol::MainMenuAction(void)
{
  // Perform the action when selected from the main VDR menu.
  return NULL;
}

cMenuSetupPage *cPlugin3dcontrol::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return NULL;
}

bool cPlugin3dcontrol::SetupParse(const char *Name, const char *Value)
{
  // Parse your own setup parameters and store their values.
  return false;
}

bool cPlugin3dcontrol::Service(const char *Id, void *Data)
{
  // Handle custom service requests from other plugins
  return false;
}

const char **cPlugin3dcontrol::SVDRPHelpPages(void)
{
  // Return help text for SVDRP commands this plugin implements
  static const char *HelpPages[] = {
    "3DOF\n"
    "    TURN OFF 3D",
    "3DTB\n"
    "    TURN ON 3D : Top and Bottom",
    "3DSB\n"
    "    TURN ON 3D : Side by Side",
    "3DCB\n"
    "    TURN ON 3D : Check Board",
    "3DFS\n"
    "    TURN ON 3D : Frame Sequential",
    "2D3D\n"
    "    TURN ON 3D : 2D -> 3D ",
    NULL
    };
  return HelpPages;
}

cString cPlugin3dcontrol::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode)
{
  // Process SVDRP commands this plugin implements
  if (strcasecmp(Command, "3DOF") == 0) {
     control->MsgSet3DMode(Disable);
     return "TURN OFF 3D";
     }
  else if (strcasecmp(Command, "3DTB") == 0) {
     control->MsgSet3DMode(TopBottom);
     return "TURN ON 3D : Top and Bottom";
     }
  else if (strcasecmp(Command, "3DSB") == 0) {
     control->MsgSet3DMode(SideBySide);
     return "TURN ON 3D : Side by Side";
     }
  else if (strcasecmp(Command, "3DCB") == 0) {
     control->MsgSet3DMode(CheckBoard);
     return "TURN ON 3D : Check Board";
     }
  else if (strcasecmp(Command, "3DFS") == 0) {
     control->MsgSet3DMode(FrameSequential);
     return "TURN ON 3D : Frame Sequential";
     }
  else if (strcasecmp(Command, "2D3D") == 0) {
     control->MsgSet3DMode(Converted2d);
     return "TURN ON 3D : 2D -> 3D";
     }

  return NULL;
}

VDRPLUGINCREATOR(cPlugin3dcontrol); // Don't touch this!
