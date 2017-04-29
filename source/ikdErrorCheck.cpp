#include "ikdErrorCheck.h"

void ikdErrorCheck::shaderObjectError(GLuint shader, const char* str)
{
  if (0 == shader) {
    fprintf(stderr, "�V�F�[�_�����G���[�F%s", str);
    exit(1);
  }
}

void ikdErrorCheck::shaderCompileError(GLuint shader, const char* str)
{
  GLint result;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (GL_FALSE == result) {
    fprintf(stderr, "�V�F�[�_�R���p�C���G���[\nFile name�F%s", str);

    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char *log = (char *)malloc(logLen);
      GLsizei written;
      glGetShaderInfoLog(shader, logLen, &written, log);

      fprintf(stderr, "�V�F�[�_�@���O�F\n%s", log);
      free(log);
    }
  }
}

void ikdErrorCheck::programObjectError(GLuint program, const char* str)
{
  if (0 == program) {
    fprintf(stderr, "�v���O�����I�u�W�F�N�g�쐬�G���[�F%s", str);
    exit(1);
  }
}

void ikdErrorCheck::programLinkError(GLuint program, const char* str)
{
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (GL_FALSE == status) {
    fprintf(stderr, "�V�F�[�_�v���O�����̃����N���s\nFile name�F%s\n", str);

    GLint logLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char* log = (char*)malloc(logLen);
      GLsizei written;
      glGetProgramInfoLog(program, logLen, &written, log);
      fprintf(stderr, "�v���O�����@���O�F%s", log);
      free(log);
    }
  }
}


  void ikdErrorCheck::fileLoadError(const char* fileName, const std::string str)
  {
    if (str.length() == 0) {
      fprintf(stderr, "File load error�F%s", fileName);
    }
  }
