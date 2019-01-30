#version 130

#ifdef GL_ES
precision mediump float;
#endif

// Attributes
attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_coord;

// Uniforms
uniform mat4 u_projModelMatrix;

// Varyings
smooth out vec4 v_frontColor;
out vec2 v_texCoord0;

void main ()
{
  gl_Position = u_projModelMatrix * a_position;

  v_texCoord0 = a_coord;
  v_frontColor = a_color;
  v_frontColor.g *= 1.15; /* Give green a bit of a boost */
}
