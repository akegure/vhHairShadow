#include <string>
#include <fstream>
#include <stdio.h>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

class ikdGetShader
{
  //vertex, fragment�V�F�[�_�̓���g�p����ꍇ�̂ݑΉ�
private:
  GLuint getShader(GLenum shaderType, const char* shaderFileName);
public:
  GLuint getProgramHandle(const char* filename);
};