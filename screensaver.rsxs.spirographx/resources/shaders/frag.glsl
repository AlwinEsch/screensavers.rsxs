#version 130

#ifdef GL_ES
precision mediump float;
#endif

smooth in vec4 v_frontColor;

void main()
{
  gl_FragColor = v_frontColor;
}
