/*
 * control.c
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "control.h"

#define MAXPLUGINARGS            1024

cList<cControlTV> cControlTV::Controllers;

cControlTV::cControlTV(void)
{
  Controllers.Add(this);
  Current3DMode=Disable;
  active=0;
}

cControlTV::~cControlTV()
{
  Controllers.Del(this, false);
}

void cControlTV::Set3DMode(int mode)
{
  if (GetCurrent3DMode()!=mode) {
     for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c)) {
         if (c->Active())
            c->DoSet3DMode(mode);
         }
     SetCurrent3DMode(mode);
  }
}

void cControlTV::Init(const char *ActiveModules)
{
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c)) {
      if ( c->Name() != NULL ) {
         c->DoInit();
         if (strstr(ActiveModules, c->Name())!=NULL)
            c->SetActive(c->Name(), 1);
         }
   }
}

void cControlTV::Stop(void)
{
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c))
      c->DoStop();
}

void cControlTV::SetActive(const char *modul, bool value)
{
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c)) {
      if ( c->Name() == modul )
         c->active=value;
      }
}

bool cControlTV::ProcessArgs(int argc, char *argv[])
{
  int argc_t;
  char *argv_t[MAXPLUGINARGS];
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c)) {
      argc_t=argc;
      for (int idx=0; idx < argc_t; idx++)
           argv_t[idx]=argv[idx];
      c->DoProcessArgs(argc_t, argv_t);
      }
  return true;
}
