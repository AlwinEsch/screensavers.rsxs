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

#include "camera.h"

#include <kodi/addon-instance/Screensaver.h>
#include <kodi/OpenGL/GL.h>
#include <kodi/OpenGL/Shader.h>

#include <vector>

#include <glm/gtc/type_ptr.hpp>


#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#define NUMOBJECTS 20
#define LATSIZE 12

struct sCoord
{
  float u,v;
};

struct sPosition
{
  float x,y,z;
};

struct sColor
{
  float r,g,b;
};

struct sLatticeSegmentEntry
{
  sPosition normal;
  sPosition vertex;
  sCoord coord;
  sColor color;
};

struct sLatticeSegment
{
  std::vector<std::vector<sLatticeSegmentEntry>> entries;
  sColor color;
  glm::mat4 matrix;
  unsigned int texture = 0;
};

typedef std::vector<sLatticeSegment> SEGMENT;

// Parameters edited in the dialog box
struct settings
{
  int dLongitude;
  int dLatitude;
  int dThick;
  int dDensity;
  int dDepth;
  int dFov;
  int dPathrand;
  int dSpeed;
  int dTexture;
  bool dSmooth;
  bool dFog;
};

class ATTRIBUTE_HIDDEN CScreensaverLattice
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceScreensaver,
    public kodi::gui::gl::CShaderProgram
{
public:
  CScreensaverLattice();

  bool Start() override;
  void Stop() override;
  void Render() override;

  void OnCompiledAndLinked() override;
  bool OnEnabled() override;

private:
  void setDefaults(int which);
  void initTextures();
  void reconfigure();
  void setMaterialAttribs(sLatticeSegment& segment);
  void makeLatticeObjects(std::vector<SEGMENT>& segments);
  void makeTorus(sLatticeSegment& segment, int longitude, int latitude, float centerradius, float thickradius);

  settings m_settings;
  unsigned int m_lattice[LATSIZE][LATSIZE][LATSIZE];
  int m_segments = 1;
  int m_globalxyz[3];
  GLuint m_texture_id[2] = {0};
  int m_lastBorder;
  float m_bPnt[10][6];  // Border points and direction vectors where camera can cross from cube to cube
  float m_path[7][6];

  GLint m_textureStyle;

  glm::mat4 m_projMat;
  glm::mat4 m_modelMat;
  glm::mat3 m_normalMat;

  bool m_useLighting;
  bool m_useSphere;
  float m_ambient[4];
  float m_diffuse[4];
  float m_specular[4];
  float m_position[4];
  float m_shininess;

  int m_fogUseLinear;
  float m_fogStart;
  float m_fogEnd;

  GLint m_uProjMatLoc = -1;
  GLint m_uModelViewMatLoc = -1;
  GLint m_uNormalMatLoc = -1;

  GLint m_uUseSphereLoc = -1;
  GLint m_uLightSource_ambientLoc = -1;
  GLint m_uLightSource_diffuseLoc = -1;
  GLint m_uLightSource_specularLoc = -1;
  GLint m_uLightSource_positionLoc = -1;
  GLint m_uLightSource_shininessLoc = -1;

  GLint m_uFog_useLoc = -1;
  GLint m_uFog_startLoc = -1;
  GLint m_uFog_endLoc = -1;

  // Smooth shading?
  GLint m_uUseFlatLoc = -1;
  GLint m_uTextureIdLoc = -1;
  GLint m_uMixTextureLoc = -1;
  GLint m_uTextureStyleLoc = -1;
  GLint m_uUseLightingLoc = -1;
  GLint m_uTextureUsedLoc = -1;

  GLint m_aNormalLoc = -1;
  GLint m_aVertexLoc = -1;
  GLint m_aCoordLoc = -1;
  GLint m_aColorLoc = -1;

  GLuint m_vertexVBO = 0;

  double m_lastTime;
  bool m_startOK = false;
  float m_frameTime = 0.0f;
  std::vector<SEGMENT> m_segmentList;

  CCamera m_camera;
};
