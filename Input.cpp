#include "Input.h"
#include "StringUtil.h"
#include <map>

std::map<int, std::string> keyToNameMap;

void setupKeyToNameMap ()
{
  keyToNameMap[Keyboard::A] = "a";
  keyToNameMap[Keyboard::B] = "b";
  keyToNameMap[Keyboard::C] = "c";
  keyToNameMap[Keyboard::D] = "d";
  keyToNameMap[Keyboard::E] = "e";
  keyToNameMap[Keyboard::F] = "f";
  keyToNameMap[Keyboard::G] = "g";
  keyToNameMap[Keyboard::H] = "h";
  keyToNameMap[Keyboard::I] = "i";
  keyToNameMap[Keyboard::J] = "j";
  keyToNameMap[Keyboard::K] = "k";
  keyToNameMap[Keyboard::L] = "l";
  keyToNameMap[Keyboard::M] = "m";
  keyToNameMap[Keyboard::N] = "n";
  keyToNameMap[Keyboard::O] = "o";
  keyToNameMap[Keyboard::P] = "p";
  keyToNameMap[Keyboard::Q] = "q";
  keyToNameMap[Keyboard::R] = "r";
  keyToNameMap[Keyboard::S] = "s";
  keyToNameMap[Keyboard::T] = "t";
  keyToNameMap[Keyboard::U] = "u";
  keyToNameMap[Keyboard::V] = "v";
  keyToNameMap[Keyboard::W] = "w";
  keyToNameMap[Keyboard::X] = "x";
  keyToNameMap[Keyboard::Y] = "y";
  keyToNameMap[Keyboard::Z] = "z";
  keyToNameMap[Keyboard::N0] = "0";
  keyToNameMap[Keyboard::N1] = "1";
  keyToNameMap[Keyboard::N2] = "2";
  keyToNameMap[Keyboard::N3] = "3";
  keyToNameMap[Keyboard::N4] = "4";
  keyToNameMap[Keyboard::N5] = "5";
  keyToNameMap[Keyboard::N6] = "6";
  keyToNameMap[Keyboard::N7] = "7";
  keyToNameMap[Keyboard::N8] = "8";
  keyToNameMap[Keyboard::N9] = "9";
  keyToNameMap[Keyboard::Up] = "up";
  keyToNameMap[Keyboard::Down] = "down";
  keyToNameMap[Keyboard::Left] = "left";
  keyToNameMap[Keyboard::Right] = "right";
  keyToNameMap[Keyboard::PageUp] = "page up";
  keyToNameMap[Keyboard::PageDown] = "page down";
  keyToNameMap[Keyboard::Home] = "home";
  keyToNameMap[Keyboard::End] = "end";
  keyToNameMap[Keyboard::Enter] = "enter";
  keyToNameMap[Keyboard::Backspace] = "backspace";
  keyToNameMap[Keyboard::Insert] = "insert";
  keyToNameMap[Keyboard::Delete] = "delete";
  keyToNameMap[Keyboard::Tab] = "tab";
  keyToNameMap[Keyboard::Space] = "space";
  keyToNameMap[Keyboard::Escape] = "escape";
  keyToNameMap[Keyboard::KP0] = "keypad 0";
  keyToNameMap[Keyboard::KP1] = "keypad 1";
  keyToNameMap[Keyboard::KP2] = "keypad 2";
  keyToNameMap[Keyboard::KP3] = "keypad 3";
  keyToNameMap[Keyboard::KP4] = "keypad 4";
  keyToNameMap[Keyboard::KP5] = "keypad 5";
  keyToNameMap[Keyboard::KP6] = "keypad 6";
  keyToNameMap[Keyboard::KP7] = "keypad 7";
  keyToNameMap[Keyboard::KP8] = "keypad 8";
  keyToNameMap[Keyboard::KP9] = "keypad 9";
  keyToNameMap[Keyboard::KPAdd] = "keypad plus";
  keyToNameMap[Keyboard::KPSubtract] = "keypad minus";
  keyToNameMap[Keyboard::KPMultiply] = "keypad multiply";
  keyToNameMap[Keyboard::KPDivide] = "keypad divide";
  keyToNameMap[Keyboard::KPDecimal] = "keypad decimal";
  keyToNameMap[Keyboard::KPEnter] = "kp enter";
  keyToNameMap[Keyboard::Grave] = "grave";
  keyToNameMap[Keyboard::Apostrophe] = "apostrophe";
  keyToNameMap[Keyboard::Semicolon] = "semicolon";
  keyToNameMap[Keyboard::Comma] = "comma";
  keyToNameMap[Keyboard::Period] = "period";
  keyToNameMap[Keyboard::Slash] = "slash";
  keyToNameMap[Keyboard::LBracket] = "left bracket";
  keyToNameMap[Keyboard::RBracket] = "right bracket";
  keyToNameMap[Keyboard::Backslash] = "backslash";
  keyToNameMap[Keyboard::Hyphen] = "hyphen";
  keyToNameMap[Keyboard::Equals] = "equals";
  keyToNameMap[Keyboard::Capslock] = "capslock";
  keyToNameMap[Keyboard::LShift] = "left shift";
  keyToNameMap[Keyboard::RShift] = "right shift";
  keyToNameMap[Keyboard::LControl] = "left control";
  keyToNameMap[Keyboard::RControl] = "right control";
  keyToNameMap[Keyboard::LAlt] = "left alt";
  keyToNameMap[Keyboard::RAlt] = "right alt";
  keyToNameMap[Keyboard::LSystem] = "left system";
  keyToNameMap[Keyboard::RSystem] = "right system";
  keyToNameMap[Keyboard::F1] = "f1";
  keyToNameMap[Keyboard::F2] = "f2";
  keyToNameMap[Keyboard::F3] = "f3";
  keyToNameMap[Keyboard::F4] = "f4";
  keyToNameMap[Keyboard::F5] = "f5";
  keyToNameMap[Keyboard::F6] = "f6";
  keyToNameMap[Keyboard::F7] = "f7";
  keyToNameMap[Keyboard::F8] = "f8";
  keyToNameMap[Keyboard::F9] = "f9";
  keyToNameMap[Keyboard::F10] = "f10";
  keyToNameMap[Keyboard::F11] = "f11";
  keyToNameMap[Keyboard::F12] = "f12";
  keyToNameMap[Keyboard::F13] = "f13";
  keyToNameMap[Keyboard::F14] = "f14";
  keyToNameMap[Keyboard::F14] = "f15";
  keyToNameMap[Keyboard::Pause] = "pause";
  keyToNameMap[Keyboard::Char] = "unicode";
}

std::string Input::getNameOfInput (int device, int axis)
{
  if (device == Device::Keyboard)
  {
    if (keyToNameMap.empty())
    {
      setupKeyToNameMap();
    }
    auto it = keyToNameMap.find(axis);
    if (it != keyToNameMap.end())
    {
      return it->second + " key";
    }
  }
  else if (device == Device::Mouse)
  {
    if (axis == Mouse::X)
    {
      return "mouse x axis";
    }
    else if (axis == Mouse::Y)
    {
      return "mouse y axis";
    }
    else if (axis == Mouse::Wheel)
    {
      return "mouse wheel";
    }
    else if (axis == Mouse::Left)
    {
      return "left mouse button";
    }
    else if (axis == Mouse::Middle)
    {
      return "middle mouse button";
    }
    else if (axis == Mouse::Right)
    {
      return "right mouse button";
    }
    else
    {
      return "mouse button " + toString(axis - Mouse::Button0 + 1);
    }
  }
  else if (device == Device::Controller0)
  {
    if (axis < Controller::Button0)
    {
      return "controller axis " + toString(axis - Controller::Axis0 + 1);
    }
    else
    {
      return "controller button " + toString(axis - Controller::Button0 + 1);
    }
  }

  return std::string();
}

