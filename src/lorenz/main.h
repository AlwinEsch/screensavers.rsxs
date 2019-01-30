/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  Copyright (C) 2002, 2009 Sören Sonnenburg <sonne@debian.org>
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
 */

/*
 * Code is based on:
 *   http://rss-glx.sourceforge.net/
 */

#pragma once

#include <kodi/addon-instance/Screensaver.h>
#include <kodi/OpenGL/GL.h>
#include <kodi/OpenGL/Shader.h>

#include <vector>

#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

#define num_satellites_default 25
#define num_points_default 100000
#define line_width_attractor_default 2
#define line_width_satellites_default 12
#define camera_speed_default 0.3
#define linear_cutoff_default 0.2
#define camera_angle_default 45

struct sPosition
{
  float x,y,z;
};

struct sColor
{
  float r,g,b,a;
};

struct sLatticeSegmentEntry
{
  sPosition normal;
  sPosition vertex;
  sColor color;
};

// Parameters edited in the dialog box
struct settings
{
  int line_width_satellites=line_width_satellites_default;
  int line_width_attractor=line_width_attractor_default;
  float camera_speed=camera_speed_default;
  float camera_angle=camera_angle_default;
  float linear_cutoff=linear_cutoff_default;

  int num_precomputed_points=num_points_default;
  int num_satellites=num_satellites_default;
};

class ATTRIBUTE_HIDDEN CScreensaverLorenz
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceScreensaver,
    public kodi::gui::gl::CShaderProgram
{
public:
  CScreensaverLorenz();

  // kodi::addon::CInstanceScreensaver
  bool Start() override;
  void Stop() override;
  void Render() override;

  // kodi::gui::gl::CShaderProgram
  void OnCompiledAndLinked() override;
  bool OnEnabled() override;

private:
  void display(void);
  void set_camera();
  void reshape(int w, int h);
  void init_satellites();
  void precompute_lorenz_array();
  void init_line_strip(void);
  void reduce_points(int cutoff);

  settings m_settings;

  // For hack check, Kodi currently also calls Render() if Start() returned false!
  bool m_startOK = false;

  float m_camera_angle_anim[2] = {5, 179};
  float m_camera_angle_anim_speed=0.1;

  int m_num_points;
  int m_num_points_max=-1;

  int m_width = 800;
  int m_height = 600;

  float* m_lorenz_coords;
  float* m_lorenz_path;
  float* m_satellite_times;
  float* m_satellite_speeds;

  glm::mat4 m_projMat;
  glm::mat4 m_modelMat;
  glm::mat3 m_normalMat;

  bool m_useLightning;
  bool m_useSphere;
  GLfloat m_position[4];
  GLfloat m_lightDir0[4];

  GLint m_projMatLoc = -1;
  GLint m_modelViewMatLoc = -1;
  GLint m_normalMatLoc = -1;

  GLint m_lightingLoc = -1;
  GLint m_uniformColorLoc = -1;
  GLint m_light0_ambientLoc = -1;
  GLint m_light0_diffuseLoc = -1;
  GLint m_light0_specularLoc = -1;
  GLint m_light0_positionLoc = -1;
  GLint m_light0_constantAttenuationLoc = -1;
  GLint m_light0_linearAttenuationLoc = -1;
  GLint m_light0_quadraticAttenuationLoc = -1;
  GLint m_light0_spotDirectionLoc = -1;
  GLint m_light0_spotExponentLoc = -1;
  GLint m_light0_spotCutoffAngleCosLoc = -1;
  GLint m_material_ambientLoc = -1;
  GLint m_material_diffuseLoc = -1;
  GLint m_material_specularLoc = -1;
  GLint m_material_emissionLoc = -1;
  GLint m_material_shininessLoc = -1;

  GLint m_hNormal = -1;
  GLint m_hVertex = -1;
  GLint m_hColor = -1;

  GLuint m_vertexVBO = 0;

  double m_lastTime;
  float m_frameTime = 0.0f;
  std::vector<sLatticeSegmentEntry> m_stripEntries;
};
