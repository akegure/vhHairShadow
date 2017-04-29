#include "AkgTextureGenerator.h"

void AkgTextureGenerator::GenerateTexture(t_TextureInfo* textureInfo, int textureNum)
{

	GLuint textureHandle = 0;
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureHandle);//2�����e�N�X�`����z��Ƃ��Ē�`
  
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�p�����[�^
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//�p�����[�^
  //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32F, textureInfo->size, textureInfo->size, 
    textureNum, 0, GL_RGBA, GL_FLOAT, NULL);//�e�N�X�`���̖{��
  //texture��fbo�̐ڑ��́CtextureArray�̏ꍇ�����_�����O�̒��O�ōs���D
  

	//�t���[���o�b�t�@�I�u�W�F�N�g�쐬
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//�t���[���o�b�t�@�I�u�W�F�N�g�𐶐�
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureInfo->size, textureInfo->size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);
     

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//���ʂ̃t���[���o�b�t�@�ɖ߂�
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);


  textureInfo->texture = textureHandle;
  textureInfo->fbo = fboHandle;

}
