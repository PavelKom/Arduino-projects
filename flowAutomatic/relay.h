#pragma once

#ifndef _relay_h
#define _relay_h

#include "Arduino.h"

class Relay
{
public:
  //Relay(){}
  Relay(uint8_t pin = 13, bool nOpen = true)
  {
    _pin = pin;
    _nOpen = nOpen;
  }
  void begin()
  {
    pinMode(_pin, OUTPUT);
  }
  void on(){
    if (_state != _nOpen)
    {
      _state = !_state;
      digitalWrite(_pin, _state);
    }
  }
  void off(){
    if (_state == _nOpen)
    {
      _state = !_state;
      digitalWrite(_pin, _state);
    }
  }
  /*
  bool getState(){
    return _state;
  }
  bool isNOpen(){
    return _nOpen;
  }
  bool isNormallyOpen(){
    return _nOpen;
  }
  */
protected:
  uint8_t _pin;
  bool _nOpen;
  bool _state;
};

#endif