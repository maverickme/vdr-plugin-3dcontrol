#include "control.h"

cList<cControlTV> cControlTV::Controllers;

cControlTV::cControlTV(void)
{
  Controllers.Add(this);
  Current3DMode=Disable;
}

cControlTV::~cControlTV()
{
  Controllers.Del(this, false);
}

void cControlTV::Set3DMode(int mode)
{
  if (GetCurrent3DMode()!=mode) {
     for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c))
         c->DoSet3DMode(mode);
     SetCurrent3DMode(mode);
  }
}

void cControlTV::Init(void)
{
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c))
      c->DoInit();
}

void cControlTV::Stop(void)
{
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c))
      c->DoStop();
}
