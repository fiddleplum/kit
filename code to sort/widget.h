#pragma once

#include "Input.h"
#include "Box.h"

class Widget
{
public:
  virtual void setBounds (Box2i bounds) {}

  virtual void handleInput (Input input) {}

  virtual void tick (float timeInterval) {}

  virtual void render () {}
  
};

