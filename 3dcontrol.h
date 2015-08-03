/*
 * 3dcontrol.h: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#ifndef __3DCONTROL_H
#define __3DCONTROL_H

#include <vector>
#include <map>
#include <string>

#ifdef IMAGEDETECTTEST
#include <Magick++.h>
#endif
#include <vdr/status.h>

#define MAXMODULES               64

static const char *VERSION        = "0.0.3";
static const char *DESCRIPTION    = "3D control plugin";
static const char *MAINMENUENTRY  = "3D Control";

typedef enum {
  Auto3D,
  Disable3D,
  force3DSbS,
  force3DTB
  } Auto3DMode;

// --- c3DControlConfig ---------------------------------------------------

class c3DControlConfig {
public:
  c3DControlConfig(void);

  // --- Main Plugin ---
  int hidemenu;

  std::map<const cChannel*, Auto3DMode> channel3DModes;
  char *no3DList;
  char *force3DsbsList;
  char *force3DtbList;

  char *activemodules;

  void InitChannelSettings(void);
  Auto3DMode GetChannelMode(const cChannel* channel);
  void SetChannelMode(const cChannel* channel, Auto3DMode mode, bool save = true);

  void SaveConfig(void);
  };

extern c3DControlConfig config;

// --- cMenuChannelItem3D ----------------------------------------------

class cMenuChannelItem3D : public cOsdItem {
private:
  const cChannel *channel;
public:
  cMenuChannelItem3D(const cChannel *Channel) : channel(Channel) { Set(); }
  void Set(void);
  virtual int Compare(const cListObject &ListObject) const;
  const cChannel *Channel(void) { return channel; }
  };

// --- cMenuChannels3D ---------------------------------------------------------

class cMenuChannels3D : public cOsdMenu {
private:
  void Set(void);
  const cChannel *GetChannel(int Index);
  cChannel mainGroup;
protected:
  eOSState Set3D(int index, Auto3DMode mode);
public:
  cMenuChannels3D(void);
  virtual eOSState ProcessKey(eKeys Key);
  };

// --- cMenuSetup3DControl ----------------------------------------------

class cMenuSetup3DControl : public cMenuSetupPage {
  private:
    int modactive[MAXMODULES];
  protected:
    virtual eOSState ProcessKey(eKeys Key);
    virtual void Store(void);
  public:
    cMenuSetup3DControl(void);
};

// --- cMenuMain3DControl ---------------------------------------------------

class cMenuMain3DControl : public cOsdMenu {
public:
  cMenuMain3DControl();
  ~cMenuMain3DControl();
  virtual eOSState ProcessKey(eKeys);
  };

// --- cMyStatusMonitor ---------------------------------------------------

class cMyStatusMonitor : public cStatus {
private:
  char *stristr(const char *String, const char *Pattern);
#ifdef IMAGEDETECTTEST
  int Is3DContent(void);
#endif
protected:
  virtual void Replaying(const cControl *Control, const char *Name, const char *FileName, bool On);
  virtual void ChannelSwitch(const cDevice* Device, int ChannelNumber, bool LiveView);
  };

// --- cPlugin3dcontrol ---------------------------------------------------

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
  virtual const char *MainMenuEntry(void) { return (config.hidemenu ? NULL : tr(MAINMENUENTRY)); }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  virtual bool Service(const char *Id, void *Data = NULL);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
  };
#endif
