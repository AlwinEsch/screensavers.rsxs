/*
 *  Copyright © 2005-2018 Team Kodi
 *  Copyright © 1999-2010 Terence M. Welsh
 *  Ported to Linux by Tugrul Galatali <tugrul@galatali.com>
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

#include "main.h"

#include <kodi/gui/General.h>
#include <glm/gtc/type_ptr.hpp>
#include <rsMath/rsMath.h>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#define NUMCONSTS 9
#define PIx2 6.28318530718f
#define DEG2RAD 0.0174532925f

namespace {
  sSettings m_settings;
}

class ATTRIBUTE_HIDDEN CWind
{
public:
  float **emitters;
  float **particles;
  int **linelist;
  int *lastparticle;
  int whichparticle;
  float c[NUMCONSTS];
  float ct[NUMCONSTS];
  float cv[NUMCONSTS];

  CWind();
  ~CWind();

  void update(CScreensaverSolarWinds* base);
};

CWind::CWind()
{
  int i;

  emitters = new float*[m_settings.dEmitters];
  for (i = 0; i < m_settings.dEmitters; i++)
  {
    emitters[i] = new float[3];
    emitters[i][0] = rsRandf(60.0f) - 30.0f;
    emitters[i][1] = rsRandf(60.0f) - 30.0f;
    emitters[i][2] = rsRandf(30.0f) - 15.0f;
  }

  particles = new float*[m_settings.dParticles];
  for (i = 0; i < m_settings.dParticles; i++)
  {
    particles[i] = new float[6];  // 3 for pos, 3 for color
    particles[i][2] = 100.0f;  // start particles behind viewer
  }

  whichparticle = 0;

  if (m_settings.dGeometry == 2)  // allocate memory for lines
  {
    linelist = new int*[m_settings.dParticles];
    for (i = 0; i < m_settings.dParticles; i++)
    {
      linelist[i] = new int[2];
      linelist[i][0] = -1;
      linelist[i][1] = -1;
    }
    lastparticle = new int[m_settings.dEmitters];
    for (i = 0; i < m_settings.dEmitters; i++)
      lastparticle[i] = i;
  }

  for (i = 0; i < NUMCONSTS; i++)
  {
    ct[i] = rsRandf(PIx2);
    cv[i] = rsRandf(0.00005f * float(m_settings.dWindspeed) * float(m_settings.dWindspeed))
      + 0.00001f * float(m_settings.dWindspeed) * float(m_settings.dWindspeed);
  }
}

CWind::~CWind()
{
  int i;

  for (i = 0; i < m_settings.dEmitters; i++)
    delete[] emitters[i];
  delete[] emitters;

  for (i = 0; i < m_settings.dParticles; i++)
    delete[] particles[i];
  delete[] particles;

  if (m_settings.dGeometry == 2)
  {
    for (i = 0; i < m_settings.dParticles; i++)
      delete[] linelist[i];
    delete[] linelist;
    delete[] lastparticle;
  }
}

void CWind::update(CScreensaverSolarWinds* base)
{
  int i;
  float x, y, z;
  float temp;
  static float evel = float(m_settings.dEmitterspeed) * 0.01f;
  static float pvel = float(m_settings.dParticlespeed) * 0.01f;
  static float pointsize = 0.04f * float(m_settings.dSize);
  static float linesize = 0.005f * float(m_settings.dSize);

  // update constants
  for (i = 0; i < NUMCONSTS; i++)
  {
    ct[i] += cv[i];
    if (ct[i] > PIx2)
      ct[i] -= PIx2;
    c[i] = cosf(ct[i]);
  }

  // calculate emissions
  for (i = 0; i < m_settings.dEmitters; i++)
  {
    emitters[i][2] += evel;  // emitter moves toward viewer
    if (emitters[i][2] > 15.0f)  // reset emitter
    {
      emitters[i][0] = rsRandf(60.0f) - 30.0f;
      emitters[i][1] = rsRandf(60.0f) - 30.0f;
      emitters[i][2] = -15.0f;
    }
    particles[whichparticle][0] = emitters[i][0];
    particles[whichparticle][1] = emitters[i][1];
    particles[whichparticle][2] = emitters[i][2];
    if (m_settings.dGeometry == 2)  // link particles to form lines
    {
      if (linelist[whichparticle][0] >= 0)
        linelist[linelist[whichparticle][0]][1] = -1;
      linelist[whichparticle][0] = -1;
      if (emitters[i][2] == -15.0f)
        linelist[whichparticle][1] = -1;
      else
        linelist[whichparticle][1] = lastparticle[i];
      linelist[lastparticle[i]][0] = whichparticle;
      lastparticle[i] = whichparticle;
    }
    whichparticle++;
    if (whichparticle >= m_settings.dParticles)
      whichparticle = 0;
  }

  // calculate particle positions and colors
  // first modify constants that affect colors
  c[6] *= 9.0f / float(m_settings.dParticlespeed);
  c[7] *= 9.0f / float(m_settings.dParticlespeed);
  c[8] *= 9.0f / float(m_settings.dParticlespeed);
  // then update each particle
  for (i = 0; i < m_settings.dParticles; i++)
  {
    // store old positions
    x = particles[i][0];
    y = particles[i][1];
    z = particles[i][2];
    // make new positions
    particles[i][0] = x + (c[0] * y + c[1] * z) * pvel;
    particles[i][1] = y + (c[2] * z + c[3] * x) * pvel;
    particles[i][2] = z + (c[4] * x + c[5] * y) * pvel;
    // calculate colors
    particles[i][3] = float(fabs((particles[i][0] - x) * c[6]));
    particles[i][4] = float(fabs((particles[i][1] - y) * c[7]));
    particles[i][5] = float(fabs((particles[i][2] - z) * c[8]));
    // clamp colors
    if (particles[i][3] > 1.0f)
      particles[i][3] = 1.0f;
    if (particles[i][4] > 1.0f)
      particles[i][4] = 1.0f;
    if (particles[i][5] > 1.0f)
      particles[i][5] = 1.0f;
  }

  sLight* light = base->m_light;

  // draw particles
  switch(m_settings.dGeometry)
  {
  case 0:  // lights
  case 1:  // points
    for (i = 0; i < m_settings.dParticles; i++)
    {
      light[0].color = light[1].color = light[2].color = light[3].color = sColor(particles[i][3], particles[i][4], particles[i][5], 1.0f);

      glm::mat4 modelMat = base->m_modelMat;

      base->m_modelMat = glm::translate(modelMat, glm::vec3(particles[i][0], particles[i][1], particles[i][2]));

      base->Enable();
      glBufferData(GL_ARRAY_BUFFER, sizeof(sLight)*4, base->m_light, GL_STATIC_DRAW);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      base->Disable();

      base->m_modelMat = modelMat;
    }
    break;
  case 2:  // lines
    for (i = 0; i < m_settings.dParticles; i++){
      temp = particles[i][2] + 40.0f;
      if (temp < 0.01f)
        temp = 0.01f;
      if (linelist[i][1] >= 0)
      {
        glLineWidth(linesize * temp);

        if (linelist[i][0] == -1)
          light[0].color = sColor(0.0f, 0.0f, 0.0f, 1.0f);
        else
          light[0].color = sColor(particles[i][3], particles[i][4], particles[i][5], 1.0f);
        light[0].vertex = sPosition(particles[i][0], particles[i][1], particles[i][2]);

        if (linelist[linelist[i][1]][1] == -1)
          light[1].color = sColor(0.0f, 0.0f, 0.0f, 1.0f);
        else
          light[1].color = sColor(particles[linelist[i][1]][3], particles[linelist[i][1]][4], particles[linelist[i][1]][5], 1.0f);
        light[1].vertex = sPosition(particles[linelist[i][1]][0], particles[linelist[i][1]][1], particles[linelist[i][1]][2]);

        base->Enable();
        glBufferData(GL_ARRAY_BUFFER, sizeof(sLight)*2, base->m_light, GL_STATIC_DRAW);
        glDrawArrays(GL_LINES, 0, 2);
        base->Disable();
      }
    }
  }
}

//------------------------------------------------------------------------------

CScreensaverSolarWinds::CScreensaverSolarWinds()
{
  int type = kodi::GetSettingInt("general.type");
  SetDefaults(type);
}

bool CScreensaverSolarWinds::Start()
{
  int i, j;
  float x, y, temp;

  if (!CreateShader("vert.glsl", "frag.glsl") || !CompileAndLink())
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to create and compile shader");
    return false;
  }

  // Initialize pseudorandom number generator
  srand((unsigned)time(nullptr));

  glGenBuffers(1, m_vertexVBO);

  m_projMat = glm::perspective(glm::radians(90.0f), (float)Width() / (float)Height(), 1.0f, 10000.0f);
  m_projMat = glm::translate(m_projMat, glm::vec3(0.0, 0.0, -15.0));

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  if (!m_settings.dGeometry)
    glBlendFunc(GL_ONE, GL_ONE);
  else
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Necessary for point and line smoothing (I don't know why)
  glEnable(GL_BLEND);

  if (!m_settings.dGeometry || m_settings.dGeometry == 1) // Init lights
  {
    for (i = 0; i < LIGHTSIZE; i++)
    {
      for (j=0; j<LIGHTSIZE; j++)
      {
        x = float(i - LIGHTSIZE / 2) / float(LIGHTSIZE / 2);
        y = float(j - LIGHTSIZE / 2) / float(LIGHTSIZE / 2);
        temp = 1.0f - float(sqrt((x * x) + (y * y)));
        if (temp > 1.0f)
          temp = 1.0f;
        if (temp < 0.0f)
          temp = 0.0f;
        m_lightTexture[i][j] = (unsigned char)(255.0f * temp);
      }
    }
    glGenTextures(1, &m_hwTexture);
    glBindTexture(GL_TEXTURE_2D, m_hwTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LIGHTSIZE, LIGHTSIZE, 0, GL_RED, GL_UNSIGNED_BYTE, m_lightTexture);

    temp = 0.02f * float(m_settings.dSize);

    m_light[0].vertex = sPosition(-temp, -temp, 0.0f);
    m_light[0].coord = sCoord(0.0f, 0.0f);

    m_light[1].vertex = sPosition(temp, -temp, 0.0f);
    m_light[1].coord = sCoord(1.0f, 0.0f);

    m_light[2].vertex = sPosition(-temp, temp, 0.0f);
    m_light[2].coord = sCoord(0.0f, 1.0f);

    m_light[3].vertex = sPosition(temp, temp, 0.0f);
    m_light[3].coord = sCoord(1.0f, 1.0f);
  }

#if !defined(HAS_GLES)
  //TODO: Bring in a way about in GLES 2.0 and above!
  if (m_settings.dGeometry == 2)  // init line smoothing
  {
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  }
#endif

  // Initialize surfaces
  m_winds = new CWind[m_settings.dWinds]();

  m_startOK = true;
  return true;
}

void CScreensaverSolarWinds::Stop()
{
  m_startOK = false;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, m_vertexVBO);
  memset(m_vertexVBO, 0, sizeof(m_vertexVBO));

  // Free memory
  delete[] m_winds;
}

void CScreensaverSolarWinds::Render()
{
  if (!m_startOK)
    return;

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO[0]);
  glVertexAttribPointer(m_hPos,  4, GL_FLOAT, 0, sizeof(sLight), BUFFER_OFFSET(offsetof(sLight, vertex)));
  glEnableVertexAttribArray(m_hPos);
  glVertexAttribPointer(m_hCol, 4, GL_FLOAT, 0, sizeof(sLight), BUFFER_OFFSET(offsetof(sLight, color)));
  glEnableVertexAttribArray(m_hCol);
  glVertexAttribPointer(m_hCoord, 2, GL_FLOAT, 0, sizeof(sLight), BUFFER_OFFSET(offsetof(sLight, coord)));
  glEnableVertexAttribArray(m_hCoord);

  int i;

  if (!m_settings.dBlur)
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }
  else
  {
    glm::mat4 projMat = m_projMat;
    m_projMat = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f);
    m_modelMat = glm::mat4(1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_blur[0].color = m_blur[1].color = m_blur[2].color = m_blur[3].color = sColor(0.0f, 0.0f, 0.0f, 0.5f - (float(m_settings.dBlur) * 0.0049f));
    m_blur[0].vertex = sPosition(0.0f, 0.0f, 0.0f);
    m_blur[1].vertex = sPosition(1.0f, 0.0f, 0.0f);
    m_blur[2].vertex = sPosition(0.0f, 1.0f, 0.0f);
    m_blur[3].vertex = sPosition(1.0f, 1.0f, 0.0f);

    Enable();
    glBufferData(GL_ARRAY_BUFFER, sizeof(sLight)*4, m_blur, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    Disable();

    if (m_settings.dGeometry == 0)
      glBlendFunc(GL_ONE, GL_ONE);
    else
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Necessary for point and line smoothing (I don't know why)
        // Maybe it's just my video card...

    m_projMat = projMat;
  }

  m_modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f));

  // You should need to draw twice if using blur, once to each buffer.
  // But wglSwapLayerBuffers appears to copy the back to the
  // front instead of just switching the pointers to them.  It turns
  // out that both NVidia and 3dfx prefer to use PFD_SWAP_COPY instead
  // of PFD_SWAP_EXCHANGE in the PIXELFORMATDESCRIPTOR.  I don't know why...
  // So this may not work right on other platforms or all video cards.

  // Update surfaces
  for (i = 0; i < m_settings.dWinds; i++)
    m_winds[i].update(this);

  glDisableVertexAttribArray(m_hPos);
  glDisableVertexAttribArray(m_hCol);
  glDisableVertexAttribArray(m_hCoord);
}

void CScreensaverSolarWinds::SetDefaults(int type)
{
  if (type == AUTOMATIC_MODE)
  {
    type = rsRandi(6) + 1;
  }

  switch (type)
  {
  case PRESET_REGULAR:  // Regular
    m_settings.dWinds = 1;
    m_settings.dEmitters = 30;
    m_settings.dParticles = 2000;
    m_settings.dGeometry = 0;
    m_settings.dSize = 50;
    m_settings.dWindspeed = 20;
    m_settings.dEmitterspeed = 15;
    m_settings.dParticlespeed = 10;
    m_settings.dBlur = 40;
    break;
  case PRESET_COSMIC_STRINGS:  // Cosmic Strings
    m_settings.dWinds = 1;
    m_settings.dEmitters = 50;
    m_settings.dParticles = 3000;
    m_settings.dGeometry = 2;
    m_settings.dSize = 20;
    m_settings.dWindspeed = 10;
    m_settings.dEmitterspeed = 10;
    m_settings.dParticlespeed = 10;
    m_settings.dBlur = 10;
    break;
  case PRESET_COLD_PRICKLIES:  // Cold Pricklies
    m_settings.dWinds = 1;
    m_settings.dEmitters = 300;
    m_settings.dParticles = 3000;
    m_settings.dGeometry = 2;
    m_settings.dSize = 5;
    m_settings.dWindspeed = 20;
    m_settings.dEmitterspeed = 100;
    m_settings.dParticlespeed = 15;
    m_settings.dBlur = 70;
    break;
  case PRESET_SPACE_FLURE:  // Space Fur
    m_settings.dWinds = 2;
    m_settings.dEmitters = 400;
    m_settings.dParticles = 1600;
    m_settings.dGeometry = 2;
    m_settings.dSize = 15;
    m_settings.dWindspeed = 20;
    m_settings.dEmitterspeed = 15;
    m_settings.dParticlespeed = 10;
    m_settings.dBlur = 0;
    break;
  case PRESET_JIGGLY:  // Jiggly
    m_settings.dWinds = 1;
    m_settings.dEmitters = 40;
    m_settings.dParticles = 1200;
    m_settings.dGeometry = 1;
    m_settings.dSize = 20;
    m_settings.dWindspeed = 100;
    m_settings.dEmitterspeed = 20;
    m_settings.dParticlespeed = 4;
    m_settings.dBlur = 50;
    break;
  case PRESET_UNDERTOW:  // Undertow
    m_settings.dWinds = 1;
    m_settings.dEmitters = 400;
    m_settings.dParticles = 1200;
    m_settings.dGeometry = 0;
    m_settings.dSize = 40;
    m_settings.dWindspeed = 20;
    m_settings.dEmitterspeed = 1;
    m_settings.dParticlespeed = 100;
    m_settings.dBlur = 50;
    break;
  case ADVANCED_MODE:
    m_settings.dWinds = kodi::GetSettingInt("advanced.winds");
    m_settings.dEmitters = kodi::GetSettingInt("advanced.emitters");
    m_settings.dParticles = kodi::GetSettingInt("advanced.particles");
    m_settings.dGeometry = kodi::GetSettingInt("advanced.geometry");
    m_settings.dSize = kodi::GetSettingInt("advanced.size");
    m_settings.dWindspeed = kodi::GetSettingInt("advanced.speed");
    m_settings.dEmitterspeed = kodi::GetSettingInt("advanced.emitterspeed");
    m_settings.dParticlespeed = kodi::GetSettingInt("advanced.particlespeed");
    m_settings.dBlur = kodi::GetSettingInt("advanced.blur");
  }

  if (type != ADVANCED_MODE)
  {
    int lastType = kodi::GetSettingInt("general.lastType");
    if (type != lastType)
    {
      kodi::SetSettingInt("general.lastType", type);
      kodi::SetSettingInt("advanced.winds", m_settings.dWinds);
      kodi::SetSettingInt("advanced.particles", m_settings.dParticles);
      kodi::SetSettingInt("advanced.geometry", m_settings.dGeometry);
      kodi::SetSettingInt("advanced.size", m_settings.dSize);
      kodi::SetSettingInt("advanced.speed", m_settings.dWindspeed);
      kodi::SetSettingInt("advanced.emitterspeed", m_settings.dEmitterspeed);
      kodi::SetSettingInt("advanced.particlespeed", m_settings.dParticlespeed);
      kodi::SetSettingInt("advanced.blur", m_settings.dBlur);
    }
  }
}

void CScreensaverSolarWinds::OnCompiledAndLinked()
{
  // Variables passed directly to the Vertex shader
  m_hProj = glGetUniformLocation(ProgramHandle(), "u_projectionMatrix");
  m_hModel = glGetUniformLocation(ProgramHandle(), "u_modelViewMatrix");
  m_hGeometry = glGetUniformLocation(ProgramHandle(), "u_geometry");
  m_hPos = glGetAttribLocation(ProgramHandle(), "a_position");
  m_hCol = glGetAttribLocation(ProgramHandle(), "a_color");
  m_hCoord = glGetAttribLocation(ProgramHandle(), "a_coord");

  // It's okay to do this only one time. Textures units never change.
  glUseProgram(ProgramHandle());
  glUseProgram(0);
}

bool CScreensaverSolarWinds::OnEnabled()
{
  // This is called after glUseProgram()
  glUniformMatrix4fv(m_hProj, 1, GL_FALSE, glm::value_ptr(m_projMat));
  glUniformMatrix4fv(m_hModel, 1, GL_FALSE, glm::value_ptr(m_modelMat));

  glUniform1i(m_hGeometry, m_settings.dGeometry);
  return true;
}

ADDONCREATOR(CScreensaverSolarWinds);
