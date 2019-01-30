#pragma once
/*
 *  Copyright (C) 2005-2019 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Kodi; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#if defined(TARGET_DARWIN)
#include <mach/mach_time.h>
#include <CoreVideo/CVHostTime.h>
#elif defined(__WINDOWS__)
#include <time.h>
#else
#include <sys/time.h>
#endif

inline double GetTimeMs()
{
#if defined(__APPLE__)
  return (double) (CVGetCurrentHostTime() / (double)(CVGetHostClockFrequency() * 0.001));
#elif defined(__WINDOWS__)
  LARGE_INTEGER tickPerSecond;
  LARGE_INTEGER tick;
  if (QueryPerformanceFrequency(&tickPerSecond))
  {
    QueryPerformanceCounter(&tick);
    return (double) (tick.QuadPart / (tickPerSecond.QuadPart / 1000.));
  }
  return -1;
#else
  timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return (double)time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0;
#endif
}
