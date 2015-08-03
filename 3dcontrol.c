/*
 * 3dcontrol.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <stdio.h>
#include <ctype.h>
#include <getopt.h>

#include <vdr/config.h>
#include <vdr/plugin.h>
#include <vdr/menuitems.h>

#include "3dcontrol.h"
#include "control.h"

#define CHNUMWIDTH  (numdigits(Channels.MaxNumber()) + 1)

cControlTV *control = new cControlTV();

c3DControlConfig config;

// --- c3DControlConfig ---------------------------------------------------

c3DControlConfig::c3DControlConfig(void)
{
  LGDevice=NULL;
  hidemenu=1;
  no3DList=strdup("");
  force3DsbsList=strdup("");
  force3DtbList=strdup("");
  lg_rs232=0;
  osd_play=0;
  osd_softhddevice=1;
}

void c3DControlConfig::InitChannelSettings(void)
{
  for (const cChannel *ch = Channels.First(); ch; ch = Channels.Next(ch)) {
      if (strstr(config.no3DList,ch->GetChannelID().ToString())!=NULL)
         config.channel3DModes[ch] = Disable3D;
      else if (strstr(config.force3DsbsList,ch->GetChannelID().ToString())!=NULL)
         config.channel3DModes[ch] = force3DSbS;
      else if (strstr(config.force3DtbList,ch->GetChannelID().ToString())!=NULL)
         config.channel3DModes[ch] = force3DTB;
      else
         config.channel3DModes[ch] = Auto3D;
      }
}

Auto3DMode c3DControlConfig::GetChannelMode(const cChannel* channel)
{
/*  std::map<const cChannel*, Auto3DMode>::iterator i = config.channel3DModes.find(channel);
  if (i != config.channel3DModes.end())
     return i->second;
  else
     return Auto3D;*/
  return config.channel3DModes[channel];
}

void c3DControlConfig::SetChannelMode(const cChannel* channel, Auto3DMode mode, bool save)
{
  config.channel3DModes[channel] = mode;
  if (save) {
     std::string chList;
     std::string chListsbs;
     std::string chListtb;
     for (const cChannel *ch = Channels.First(); ch; ch = Channels.Next(ch)) {
         if (!ch->GroupSep()) {
            if (GetChannelMode(ch) == Disable3D)
                chList += std::string(chList.empty()?"":" ") + *ch->GetChannelID().ToString();
            if (GetChannelMode(ch) == force3DSbS)
                chListsbs += std::string(chListsbs.empty()?"":" ") + *ch->GetChannelID().ToString();
            if (GetChannelMode(ch) == force3DTB)
                chListtb += std::string(chListtb.empty()?"":" ") + *ch->GetChannelID().ToString();
            }
         }
     config.no3DList = strdup(chList.c_str());
     config.force3DsbsList = strdup(chListsbs.c_str());
     config.force3DtbList = strdup(chListtb.c_str());
     SaveConfig();
     }
}

void c3DControlConfig::SaveConfig(void)
{
  cPlugin *p = cPluginManager::GetPlugin(PLUGIN_NAME_I18N);
  if (p) {
     p->SetupStore("hidemenu",          config.hidemenu);
     p->SetupStore("no3DList",          config.no3DList);
     p->SetupStore("force3DsbsList",    config.force3DsbsList);
     p->SetupStore("force3DtbList",     config.force3DtbList);
     p->SetupStore("lg_rs232",          config.lg_rs232);
     p->SetupStore("osd_play",          config.osd_play);
     p->SetupStore("osd_softhddevice",  config.osd_softhddevice);
     Setup.Save();
     }
}

// --- cMenuChannelItem3D ----------------------------------------------

void cMenuChannelItem3D::Set(void)
{
  int rc=1;
  char *buffer = NULL;
  if (!channel->GroupSep()) {
     if (config.GetChannelMode(channel) == Auto3D )
        rc=asprintf(&buffer, "%d\t%s\t%s", channel->Number(), channel->Name(), tr("Auto 3D"));
     else if (config.GetChannelMode(channel) == force3DSbS )
        rc=asprintf(&buffer, "%d\t%s\t%s", channel->Number(), channel->Name(), tr("Force 3D SbS"));
     else if (config.GetChannelMode(channel) == force3DTB )
        rc=asprintf(&buffer, "%d\t%s\t%s", channel->Number(), channel->Name(), tr("Force 3D TB"));
     else
        rc=asprintf(&buffer, "%d\t%s\t%s", channel->Number(), channel->Name(), tr("No Auto 3D"));
    }
  else
     rc=asprintf(&buffer, "---\t%s ----------------------------------------------------------------", channel->Name());
  if (rc)
     rc=0;
  SetText(buffer, false);
}

int cMenuChannelItem3D::Compare(const cListObject &ListObject) const
{
  cMenuChannelItem3D *p = (cMenuChannelItem3D *)&ListObject;
  return channel->Number() - p->channel->Number();
}

// --- cMenuChannels3D ---------------------------------------------------------

cMenuChannels3D::cMenuChannels3D(void)
:cOsdMenu(tr("3D channels"), CHNUMWIDTH, 30)
{
  config.InitChannelSettings();
  // create a main channel group
  mainGroup.Parse(tr(":Main channels"));
  Set();
  SetHelp(tr("Button$Auto 3D"), tr("Button$Force 3D SbS"), tr("Button$Force 3D TB"), tr("Button$No Auto 3D"));
}

void cMenuChannels3D::Set(void)
{
  const cChannel *currentChannel = GetChannel(Current());
  Clear();
  if (Channels.First() && !Channels.First()->GroupSep()) // add a main group if there's none
    Add(new cMenuChannelItem3D(&mainGroup));
  for (const cChannel *channel = Channels.First(); channel; channel = Channels.Next(channel))
    if ( *channel->Name())
      Add(new cMenuChannelItem3D(channel), channel == currentChannel);
}

const cChannel *cMenuChannels3D::GetChannel(int Index)
{
  cMenuChannelItem3D *p = (cMenuChannelItem3D *)Get(Index);
  return p ? p->Channel() : NULL;
}

eOSState cMenuChannels3D::Set3D(int index, Auto3DMode mode)
{
  const cChannel* channel = GetChannel(index);
  if (!channel) return osContinue;
  std::vector<const cChannel*> channelList;
  bool singleUpdate = true;
  if (!channel->GroupSep())
    channelList.push_back(channel);
  else {
    singleUpdate = false;
    for (int c = index+1; c < Count()-1; c++) {
        const cChannel* ch = GetChannel(c);
        if (ch->GroupSep()) break;
           channelList.push_back(ch);
        }
    }
  for (std::vector<const cChannel*>::iterator i = channelList.begin(); i != channelList.end(); ++i) {
      std::vector<const cChannel*>::iterator next = i;
      next++;
      config.SetChannelMode(*i, mode, next==channelList.end());
      }
  if (!singleUpdate)
    Set();
  else
    RefreshCurrent();
  Display();
  return osContinue;
}

eOSState cMenuChannels3D::ProcessKey(eKeys Key)
{
  eOSState state = cOsdMenu::ProcessKey(Key);

  switch (state) {
    default:
      if (state == osUnknown) {
         switch (Key) {
           case kOk:
//                config.SaveConfig();
                state = osBack;
                break;
           case kRed:
                state = Set3D(Current(), Auto3D);
                break;
           case kGreen:
                state = Set3D(Current(), force3DSbS);
                break;
           case kYellow:
                state = Set3D(Current(), force3DTB);
                break;
           case kBlue:
                state = Set3D(Current(), Disable3D);
                break;
           default:
                break;
           }
         }
    }
  return state;
}

// --- cMenuSetup3DControl ---------------------------------------------------

cMenuSetup3DControl::cMenuSetup3DControl(void)
{
//  data = config;
  int current = Current();
  Clear();
  Add(new cMenuEditBoolItem(tr("Hide main menu entry"),          &config.hidemenu,               trVDR("no"),           trVDR("yes")));
  Add(new cMenuEditBoolItem(tr("Control LG-TV via RS232"),       &config.lg_rs232,               trVDR("disable"),      trVDR("enable")));
  Add(new cMenuEditBoolItem(tr("Control play plugin OSD mode"),  &config.osd_play,               trVDR("disable"),      trVDR("enable")));
  Add(new cMenuEditBoolItem(tr("Control softhddevice OSD mode"), &config.osd_softhddevice,       trVDR("disable"),      trVDR("enable")));
  Add(new cOsdItem(tr("Channelmapping >>"), osUser1));
  SetCurrent(Get(current));
  Display();
}

void cMenuSetup3DControl::Store(void)
{
//    config = data;
    config.SaveConfig();
}

eOSState cMenuSetup3DControl::ProcessKey(eKeys Key)
{
  eOSState state = cOsdMenu::ProcessKey(Key);

  if (state == osUser1 && Key == kOk)
     return AddSubMenu(new cMenuChannels3D());

  if (state == osUnknown) {
     switch (Key) {
       case kOk: Store();
                 state = osBack;
                 break;
       default: break;
       }
     }
  return state;
}

// --- cMenuMain3DControl -----------------------------------------------------------

cMenuMain3DControl::cMenuMain3DControl()
:cOsdMenu(tr("3D-Control"))
{
  SetHasHotkeys();

  Add(new cOsdItem(hk(tr("3D Disable")), osUser1));
  Add(new cOsdItem(hk(tr("3D Side by Side")), osUser2));
  Add(new cOsdItem(hk(tr("3D Side by Side LR")), osUser3));
  Add(new cOsdItem(hk(tr("3D Top Bottom")), osUser4));
  Add(new cOsdItem(hk(tr("3D Check Board")), osUser5));
  Add(new cOsdItem(hk(tr("3D Frame Sequential")), osUser6));
  Add(new cOsdItem(hk(tr("2D -> 3D")), osUser7));
}

cMenuMain3DControl::~cMenuMain3DControl()
{
}

eOSState cMenuMain3DControl::ProcessKey(eKeys key)
{
  eOSState state = cOsdMenu::ProcessKey(key);

  switch (state) {
    case osUser1:
         control->Set3DMode(Disable);
         return osEnd;
    case osUser2:
         control->Set3DMode(SideBySide);
         return osEnd;
    case osUser3:
         control->Set3DMode(SideBySideLR);
         return osEnd;
    case osUser4:
         control->Set3DMode(TopBottom);
         return osEnd;
    case osUser5:
         control->Set3DMode(CheckBoard);
         return osEnd;
    case osUser6:
         control->Set3DMode(FrameSequential);
         return osEnd;
    case osUser7:
         control->Set3DMode(Converted2d);
         return osEnd;
    default:
         break;
    }

  return state;
}

// --- cMyStatusMonitor ---------------------------------------------------

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
        if (stristr(FileName, "OU"))
           control->Set3DMode(TopBottom);
        else if (stristr(FileName, "TB"))
           control->Set3DMode(TopBottom);
        else
           control->Set3DMode(SideBySide);
        }
     else
        control->Set3DMode(Disable);
     }
  else
     control->Set3DMode(Disable);
}

void cMyStatusMonitor::ChannelSwitch(const cDevice* Device, int ChannelNumber, bool LiveView)
{
  cChannel *Channel = Channels.GetByNumber(cDevice::CurrentChannel());
  if (LiveView && ChannelNumber > 0) {
     switch (config.GetChannelMode(Channel)) {
       case Disable3D:
            control->Set3DMode(Disable);
            return;
       case force3DSbS:
            control->Set3DMode(SideBySide);
            return;
       case force3DTB:
            control->Set3DMode(TopBottom);
            return;
       default:
            if (stristr(Channel->Name(), "3D")) {
               control->Set3DMode(SideBySide);
               return;
               }
            break;
       }
     }
  control->Set3DMode(Disable);
}

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
  return "  -d DEV,   --device=DEV   set the device to use (default is )";
}

bool cPlugin3dcontrol::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  static struct option long_options[] = {
       { "device",   required_argument, NULL, 'd' },
       { NULL,       no_argument,       NULL,  0  }
     };

  int c;
  while ((c = getopt_long(argc, argv, "d:", long_options, NULL)) != -1) {
        switch (c) {
          case 'd': config.LGDevice=optarg;
                    break;
          default:  return false;
          }
        }
  return true;
}

bool cPlugin3dcontrol::Initialize(void)
{
  // Initialize any background activities the plugin shall perform.
  control->Init();
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
  control->Stop();
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
  return new cMenuMain3DControl();
}

cMenuSetupPage *cPlugin3dcontrol::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return new cMenuSetup3DControl();
}

bool cPlugin3dcontrol::SetupParse(const char *Name, const char *Value)
{
  // Parse your own setup parameters and store their values.
  if      (!strcasecmp(Name, "hidemenu"))         config.hidemenu         = atoi(Value);
  else if (!strcasecmp(Name, "no3DList"))         config.no3DList         = strdup(Value ? Value : "");
  else if (!strcasecmp(Name, "force3DsbsList"))   config.force3DsbsList   = strdup(Value ? Value : "");
  else if (!strcasecmp(Name, "force3DtbList"))    config.force3DtbList    = strdup(Value ? Value : "");
  else if (!strcasecmp(Name, "lg_rs232"))         config.lg_rs232         = atoi(Value);
  else if (!strcasecmp(Name, "osd_play"))         config.osd_play         = atoi(Value);
  else if (!strcasecmp(Name, "osd_softhddevice")) config.osd_softhddevice = atoi(Value);
  else
     return false;
  return true;
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
     control->Set3DMode(Disable);
     return "TURN OFF 3D";
     }
  else if (strcasecmp(Command, "3DTB") == 0) {
     control->Set3DMode(TopBottom);
     return "TURN ON 3D : Top and Bottom";
     }
  else if (strcasecmp(Command, "3DSB") == 0) {
     control->Set3DMode(SideBySide);
     return "TURN ON 3D : Side by Side";
     }
  else if (strcasecmp(Command, "3DCB") == 0) {
     control->Set3DMode(CheckBoard);
     return "TURN ON 3D : Check Board";
     }
  else if (strcasecmp(Command, "3DFS") == 0) {
     control->Set3DMode(FrameSequential);
     return "TURN ON 3D : Frame Sequential";
     }
  else if (strcasecmp(Command, "2D3D") == 0) {
     control->Set3DMode(Converted2d);
     return "TURN ON 3D : 2D -> 3D";
     }

  return NULL;
}

VDRPLUGINCREATOR(cPlugin3dcontrol); // Don't touch this!
