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

#include "stretchedParticle.h"
#include "main.h"

#include <glm/glm.hpp>
#include <rsMath/rsMath.h>

CStretchedParticle::CStretchedParticle(CScreensaverHyperspace* base)
  : m_base(base),
    m_pos(0.0f),
    m_lastPos(0.0f),
    m_drawPos(0.0f),
    m_screenPos(0.0f),
    m_lastScreenPos(0.0f),
    m_radius(0.03f),
    m_color(1.0f)
{
}

void CStretchedParticle::SetPosition(float x, float y, float z)
{
  m_pos.x = x;
  m_pos.y = y;
  m_pos.z = z;
}

void CStretchedParticle::SetColor(float r, float g, float b, bool withColorFull)
{
  m_color.r = r;
  m_color.g = g;
  m_color.b = b;
  if (withColorFull)
    m_color[rsRandi(3)] = 1.0f;
}

void CStretchedParticle::Draw(const glm::vec3& eyepoint)
{
  glm::vec3 win = glm::project(m_pos, m_base->ModelMatrix(), m_base->ProjectionMatrix(), m_base->ViewPort());  // in screen coordinates
  if(win.z > 0.0f)
  {
    m_screenPos.x = win.x;
    m_screenPos.y = win.y;
  }

  m_drawPos = (m_pos + m_lastPos) * 0.5f;
  m_lastPos = m_pos;

  glm::vec2 sd(m_screenPos - m_lastScreenPos); // screen difference
  glm::vec3 pd(eyepoint - m_drawPos); // position difference

  float n =  sqrtf(pd.x * pd.x + pd.y * pd.y + pd.z * pd.z);
  glm::mat4 bbMat(1.0f);
  bbMat[2][0] = pd.x / n;
  bbMat[2][1] = pd.y / n;
  bbMat[2][2] = pd.z / n;
  bbMat[0][0] = bbMat[2][2];
  bbMat[0][2] = -bbMat[2][0];
  bbMat[1][0] = bbMat[2][1] * bbMat[0][2] - bbMat[0][1] * bbMat[2][2];
  bbMat[1][0] = bbMat[2][2] * bbMat[0][0] - bbMat[0][2] * bbMat[2][0];
  bbMat[1][0] = bbMat[2][0] * bbMat[0][1] - bbMat[0][0] * bbMat[2][1];

  float stretch = 0.0015f * sqrtf(sd.x * sd.x + sd.y * sd.y) * n / m_radius;
  if(stretch < 1.0f)
    stretch = 1.0f;
  if(stretch > 0.5f / m_radius)
    stretch = 0.5f / m_radius;

  glm::mat4& modelMat = m_base->ModelMatrix();
  glm::mat4 modelMatOld = modelMat;

  modelMat = glm::translate(modelMat, m_drawPos) * bbMat;
  modelMat = glm::rotate(modelMat, glm::radians(57.2957795131f * atan2f(sd[1], sd[0]) + m_base->Unroll()), glm::vec3(0, 0, 1));
  modelMat = glm::scale(modelMat, glm::vec3(stretch, 1.0f, 1.0f));
  float darkener = stretch * 0.3f;
  if(darkener < 1.0f)
    darkener = 1.0f;

  sLight data[4];
  data[0].coord = sCoord(0.0f, 0.0f);
  data[1].coord = sCoord(1.0f, 0.0f);
  data[2].coord = sCoord(0.0f, 1.0f);
  data[3].coord = sCoord(1.0f, 1.0f);

  // draw colored aura
  data[0].vertex = sPosition(-m_radius, -m_radius, 0.0f);
  data[1].vertex = sPosition(m_radius, -m_radius, 0.0f);
  data[2].vertex = sPosition(-m_radius, m_radius, 0.0f);
  data[3].vertex = sPosition(m_radius, m_radius, 0.0f);
  m_base->Draw(sColor(m_color[0] / darkener, m_color[1] / darkener, m_color[2] / darkener), GL_TRIANGLE_STRIP, data, 4);

  // draw white center
  data[0].vertex = sPosition(-m_radius*0.3f, -m_radius*0.3f, 0.0f);
  data[1].vertex = sPosition(m_radius*0.3f, -m_radius*0.3f, 0.0f);
  data[2].vertex = sPosition(-m_radius*0.3f, m_radius*0.3f, 0.0f);
  data[3].vertex = sPosition(m_radius*0.3f, m_radius*0.3f, 0.0f);
  m_base->Draw(sColor(1.0f / darkener, 1.0f / darkener, 1.0f / darkener), GL_TRIANGLE_STRIP, data, 4);

  modelMat = modelMatOld;

  m_lastScreenPos = m_screenPos;
}
