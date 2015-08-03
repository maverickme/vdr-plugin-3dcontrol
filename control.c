#include "control.h"

cList<cControlTV> cControlTV::Controllers;

cControlTV::cControlTV(void)
{
  Current3dMode=Disable;
  Controllers.Add(this);
}

cControlTV::~cControlTV()
{
  Controllers.Del(this, false);
}

void cControlTV::MsgSet3DMode(int mode)
{
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c))
      c->Set3DMode(mode);
}

void cControlTV::MsgInit(void)
{
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c))
      c->Init();
}

void cControlTV::MsgStop(void)
{
  for (cControlTV *c = Controllers.First(); c; c = Controllers.Next(c))
      c->Stop();
}
