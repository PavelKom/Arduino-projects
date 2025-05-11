#pragma once

#ifndef _sleepTime_h
#define _sleepTime_h

#include <Arduino.h>
#include <RtcDateTime.h>

class SleepTime
{
public:
  SleepTime(
    uint32_t start_hour, uint32_t start_minute, uint32_t start_second,
    uint32_t end_hour, uint32_t end_minute, uint32_t end_second){
      _start = (start_hour * 60 + start_minute ) * 60 + start_second;
      _end = (end_hour * 60 + end_minute ) * 60 + end_second;
      if (_end < _start){
        _offset = 86400;
      }
      _end += _offset;
  }
  SleepTime(
    uint32_t start_hour, uint32_t start_minute,
    uint32_t end_hour, uint32_t end_minute) : SleepTime(start_hour, start_minute, 0, end_hour, end_minute, 0) {}
  SleepTime(
    uint32_t start_hour, 
    uint32_t end_hour) : SleepTime(start_hour, 0, 0, end_hour, 0, 0) {}
  //SleepTime() : SleepTime(0, 0, 0, 0, 0, 0) {}
  bool isInside(const RtcDateTime& dt, uint32_t& tmp){
    tmp = (uint32_t(dt.Hour()) * 60 + uint32_t(dt.Minute())) * 60 + uint32_t(dt.Second());
    return (_start <= tmp) && (tmp <= _end);
  }
  bool isInside(const RtcDateTime& dt)
  {
    uint32_t tmp;
    return isInside(dt, tmp);
  }
  bool isInsideEx(const RtcDateTime& dt, RtcDateTime& next)
  {
    uint32_t tmp;
    return isInsideEx(dt, next, tmp);
  }
  bool isInsideEx(const RtcDateTime& dt, RtcDateTime& next, uint32_t& tmp)
  {
    if (isInside(dt, tmp))
    {
      next = RtcDateTime( dt.TotalSeconds() - tmp + _end);
      return true;
    }
    return false;
  }
protected:
    uint32_t _start;
    uint32_t _end;
    uint32_t _offset;
};

#endif
