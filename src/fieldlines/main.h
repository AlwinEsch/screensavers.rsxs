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
 * Code taken from Terence M. Welsh (http://www.reallyslick.com) and
 * https://github.com/reallyslickscreensavers/reallyslickscreensavers
 * and reworked to GL 3.0.
 */

#pragma once

#include <kodi/addon-instance/Screensaver.h>
#include <kodi/OpenGL/GL.h>
#include <kodi/OpenGL/Shader.h>
#include <vector>

#include <glm/glm.hpp>

class CIon;

class ATTRIBUTE_HIDDEN CScreensaverFieldLines
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceScreensaver,
    public kodi::gui::gl::CShaderProgram
{
public:
  CScreensaverFieldLines() = default;

  bool Start() override;
  void Stop() override;
  void Render() override;

  void OnCompiledAndLinked() override;
  bool OnEnabled() override;

  inline float Speed() const { return m_speed; }
  inline float RenderWidth() const { return m_usedWidth; }
  inline float RenderHeight() const { return m_usedHeight; }
  inline float RenderDeep() const { return m_usedDeep; }

private:
  void drawfieldline(CIon& ion, float x, float y, float z);

  double m_lastTime;
  std::vector<CIon> m_ions;

  unsigned int m_vertexVBO = 0;

  glm::mat4 m_projMat;
  glm::mat4 m_modelMat;

  GLint m_hProj = -1;
  GLint m_hModel = -1;
  GLint m_hPos = -1;
  GLint m_hCol = -1;

  int m_ionsCnt = 6;
  int m_stepSize = 10;
  int m_maxSteps = 300;
  int m_width = 30;
  int m_speed = 10;
  bool m_constwidth = false;
  bool m_electric = false;
  float m_reduction = 3.0;

  float m_usedWidth = 1.0;
  float m_usedHeight = 1.0;
  float m_usedDeep = 200.0f;;

  bool m_startOK = false;
};
