#version 130

#ifdef GL_ES
precision mediump float;
#endif

// Uniforms
uniform sampler2D u_texUnit;
uniform int u_type;

// Varyings
in vec4 v_frontColor;
in vec2 v_texCoord0;

void main()
{
  if (u_type != 0)
  gl_FragColor = texture2D(u_texUnit, v_texCoord0) * v_frontColor;
  else
    gl_FragColor = texture2D(u_texUnit, v_texCoord0).rrra * v_frontColor;
}
