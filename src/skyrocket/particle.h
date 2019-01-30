/*
 *  Copyright (C) 2005-2019 Team Kodi
 *  Copyright (C) 1999-2010 Terence M. Welsh
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

/*
 * Code is based on:
 *   https://github.com/reallyslickscreensavers/reallyslickscreensavers
 *   http://rss-glx.sourceforge.net/
 */

#pragma once

#include <kodi/AddonBase.h>
#include <kodi/OpenGL/GL.h>

#include <rsMath/rsMath.h>

#include "flare.h"
#include "smoke.h"
#include "shockwave.h"
#include "soundEngine.h"

#define PI 3.14159265359f
#define PIx2 6.28318530718f
#define D2R 0.0174532925f
#define R2D 57.2957795131f

// types of particles
#define ROCKET 0
#define FOUNTAIN 1
#define SPINNER 2
#define SMOKE 3
#define EXPLOSION 4
#define STAR 5
#define STREAMER 6
#define METEOR 7
#define POPPER 8
#define BEE 9
#define SUCKER 10
#define SHOCKWAVE 11
#define STRETCHER 12
#define BIGMAMA 13

#undef ATTRIBUTE_FORCEINLINE
#define ATTRIBUTE_FORCEINLINE

class CScreensaverSkyRocket;

class ATTRIBUTE_HIDDEN CParticle
{
public:
  // Constructor initializes particles to be stars because that's what most of them are
  CParticle(CScreensaverSkyRocket* base);
  ~CParticle(){};

  // A handy function for choosing an explosion's color
  void randomColor(rsVec& color);

  // Initialization functions for particle types other than stars
  void initStar();
  void initStreamer();
  void initMeteor();
  void initStarPopper();
  void initStreamerPopper();
  void initMeteorPopper();
  void initLittlePopper();
  void initBee();
  void initRocket();
  void initFountain();
  void initSpinner();
  void initSmoke(rsVec pos, rsVec speed);
  void initSucker(); // rare easter egg explosion which is immediately followed by...
  void initShockwave(); // a freakin' huge explosion
  void initStretcher(); // another rare explosion followed by...
  void initBigmama(); // this other massive bomb
  void initExplosion();

  // "pop" functions are used to spawn new particles during explosions
  void popSphere(int numParts, float v0, rsVec color);
  void popSplitSphere(int numParts, float v0, rsVec color1);
  void popMultiColorSphere(int numParts, float v0);
  void popRing(int numParts, float v0, rsVec color);
  void popStreamers(int numParts, float v0, rsVec color);
  void popMeteors(int numParts, float v0, rsVec color);
  void popStarPoppers(int numParts, float v0, rsVec color);
  void popStreamerPoppers(int numParts, float v0, rsVec color);
  void popMeteorPoppers(int numParts, float v0, rsVec color);
  void popLittlePoppers(int numParts, float v0);
  void popBees(int numParts, float v0, rsVec color);

  // Finds depth along camera's coordinate system's -z axis.
  // Can be used for sorting and culling.
  void findDepth();

  // Update a particle according to frameTime
  void update();

  // Draw a particle
  void draw();

  ATTRIBUTE_FORCEINLINE unsigned int& Type() { return type; }
  ATTRIBUTE_FORCEINLINE rsVec& XYZ() { return xyz; }
  ATTRIBUTE_FORCEINLINE rsVec& RGB() { return rgb; }
  ATTRIBUTE_FORCEINLINE rsVec& VelocityVector() { return vel; }
  ATTRIBUTE_FORCEINLINE float& TimeTotal() { return t; }
  ATTRIBUTE_FORCEINLINE float& TimeRemaining() { return tr; }
  ATTRIBUTE_FORCEINLINE float& LifeRemaining() { return life; }
  ATTRIBUTE_FORCEINLINE float Bright() const { return bright; }
  ATTRIBUTE_FORCEINLINE int& ExplosionType() { return explosiontype; };

  // Return a pointer to this particle
  CParticle* thisParticle(){return this;};

  // operators used by stl list sorting
  friend bool operator < (const CParticle &p1, const CParticle &p2){return(p2.depth < p1.depth);}
  friend bool operator > (const CParticle &p1, const CParticle &p2){return(p2.depth > p1.depth);}
  friend bool operator == (const CParticle &p1, const CParticle &p2){return(p1.depth == p2.depth);}
  friend bool operator != (const CParticle &p1, const CParticle &p2){return(p1.depth != p2.depth);}

private:
  unsigned int type; // choose type from #defines listed above
  flareType m_displayList; // which object to draw (uses flare and rocket models)
  rsVec xyz; // current position
  rsVec lastxyz; // position from previous frame
  rsVec vel; // velocity vector
  rsVec rgb; // particle's color
  float drag; // constant to represent air resistance
  float t; // total time that particle lives
  float tr; // time remaining
  float bright; // intensity at which particle shines
  float life; // life remaining (usually defined from 0.0 to 1.0)
  float size; // scale factor by which to multiply the display list
  // rocket variables
  float thrust; // constant to represent power of rocket
  float endthrust; // point in rockets life at which to stop thrusting
  float spin, tilt; // radial and pitch velocities to make rockets wobble when they go up
  rsVec tiltvec; // vector about which a rocket tilts
  int makeSmoke; // whether or not this particle produces smoke
  int smokeTimeIndex; // which smoke time to use
  float smokeTrailLength; // length that smoke particles must cover from one frame to the next.
      // smokeTrailLength is stored so that remaining length from previous frame can be covered
      // and no gaps are left in the smoke trail
  float sparkTrailLength; // same for sparks from streamers
  int explosiontype; // Type of explosion that a rocket will become when life runs out
  // sorting variable
  float depth;

  CScreensaverSkyRocket* m_base;
};
