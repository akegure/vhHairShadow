#include "AkgPlane.h"

void AkgPlane::Create(t_ObjectInfo* objectInfo)
{
    float vert[4][3] = {
    -0.5f,  0.0f, -0.5f,
     0.5f,  0.0f, -0.5f,
     0.5f,  0.0f,  0.5f,
    -0.5f,  0.0f,  0.5f};

  unsigned int index_base[6] = {0,3,2, 2,1,0};
  m_Index = std::vector<unsigned int>(index_base, index_base+6);

	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
  GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];

	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4*3 * sizeof(float), vert, GL_STATIC_DRAW);

  //�@���o�b�t�@�Ƀf�[�^��}��
  glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, 4*3 * sizeof(float), vert, GL_STATIC_DRAW);

  //�e�N�X�`���ʒu�o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4*2 * sizeof(float), vert, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

  //�C���f�b�N�X1�͖@���o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

  //�C���f�b�N�X2�̓e�N�X�`�����_�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
  

  objectInfo->handle = vaoHandle;
  objectInfo->index = &m_Index[0];
  objectInfo->indexSize = m_Index.size();
}