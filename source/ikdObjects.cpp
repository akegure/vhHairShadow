#include "ikdObjects.h"


GLuint ikdObjects::ikdTriangle()
{
	float positionData[] = { -0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f,  0.8f, 0.0f};

	float colorData[] = { 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f};

	//create vertex buffer object
	GLuint vboHandle[2];
	glGenBuffers(2, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint colorBufferHandle = vboHandle[1];

	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

	//�F�o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X1�͐F�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

void ikdObjects::ikdGetVertexBox(float boxVert[], float boxNormal[], float boxTexCoord[])
{
	int i, j, k;
	int index;

	float vertices[8][3] = {-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f};

	int surfaces[12][3] = {
		0,3,2, 2,1,0,
		0,1,5, 5,4,0,
		0,4,7, 7,3,0,
		1,2,6, 6,5,1,
		5,6,7, 7,4,5,
		2,3,7, 7,6,2};

	float texCoords[4][2] = {0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f};

	int texSurfaces[2][3] = {0,3,2, 2,1,0};

	//���_�z��
	index = 0;
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 3; j++) {
			int ver = surfaces[i][j];
			memcpy(boxVert+index, vertices[ver], sizeof(float)*3);
			index += 3;
		}
	}

	//�e�N�X�`�����W
	index = 0;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 2; j++) {
			for (k = 0; k < 3; k++) {
				int ver = texSurfaces[j][k];
				memcpy(boxTexCoord+index, texCoords[ver], sizeof(float)*2);
				index += 2;
			}
		}
	}


	//�@�����
	index = 0;
	for (i = 0; i < 12; i++) {
		glm::vec3 p1 = glm::vec3(boxVert[i*9+0], boxVert[i*9+1], boxVert[i*9+2]);
		glm::vec3 p2 = glm::vec3(boxVert[i*9+3], boxVert[i*9+4], boxVert[i*9+5]);
		glm::vec3 p3 = glm::vec3(boxVert[i*9+6], boxVert[i*9+7], boxVert[i*9+8]);

		glm::vec3 e1 = p2 - p1;
		glm::vec3 e2 = p3 - p2;

		glm::vec3 normal = glm::normalize(glm::cross(e1, e2));

		for (j = 0; j < 3; j++) {
			memcpy(boxNormal+index, &normal[0], sizeof(float)*3);
			index += 3;
		}
	}

}

GLuint ikdObjects::ikdBox(const float width, const float height, const float depth)
{
	float boxVert[3*3*12];
	float boxNormal[3*3*12];
	float boxTexCoord[2*3*12];



	//���_�̎擾
	ikdGetVertexBox(boxVert, boxNormal, boxTexCoord);

	//�T�C�Y�̐ݒ�
	for (int i = 0; i < 3*12; i++) {
		boxVert[i*3] *= width;
		boxVert[i*3+1] *= height;
		boxVert[i*3+2] *= depth;
	}

	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle	  = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];

	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*12 * sizeof(float), boxVert, GL_STATIC_DRAW);

	//�@�����o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*12 * sizeof(float), boxNormal, GL_STATIC_DRAW);

	//�e�N�X�`���ʒu�o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*12 * sizeof(float), boxTexCoord, GL_STATIC_DRAW);

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

	//�C���f�b�N�X1�͖@�����o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X2�̓e�N�X�`�����_�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdBox(ikdDataSet* pdataSet)
{
	const int pointsNum = 12*2*3;
	float boxPoints[8*3];
	pdataSet->getBoundingPos(boxPoints);

	int boxLines[12*2] = {
		0,1, 1,2, 2,3, 3,0,
		4,5, 5,6, 6,7, 7,4,
		3,4, 0,5, 2,7, 1,6};

	float boxVertices[pointsNum];

	int index = 0;
	for (int i = 0; i < 12*2; i++) {
		memcpy(boxVertices+index, &boxPoints[boxLines[i]*3], sizeof(float)*3);
		index += 3;
	}


	//create vertex buffer object
	GLuint vboHandle[1];
	glGenBuffers(1, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	

	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pointsNum * sizeof(float), boxVertices, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);	

	return vaoHandle;
}

void ikdObjects::ikdGetVertexPlane(float planeVert[], float planeNormal[], float planeTexCoord[])
{
	int i, j;
	int index;

	float vertices[4][3] = {-0.5, 0.0, -0.5,
		0.5, 0.0, -0.5,
		0.5, 0.0,  0.5,
		-0.5, 0.0, 0.5};

	int surfaces[2][3] = {0,1,2, 2,3,0};

	float texCoords[4][2] = {1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f};

	index = 0;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) {
			int ver = surfaces[i][j];
			memcpy(planeVert+index, vertices[ver], sizeof(float)*3);
			index += 3;
		}
	}


	index = 0;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) { 
			int ver = surfaces[i][j];
			memcpy(planeTexCoord+index, texCoords[ver], sizeof(float)*2);
			index += 2;
		}
	}

	//�@�����̐ݒ�
	float normal[] = {0.0, 1.0, 0.0};

	index = 0;
	for (i = 0; i < 6; i++) {
		memcpy(planeNormal+index, normal, sizeof(float)*3);
		index += 3;
	}

}

GLuint ikdObjects::ikdPlane(const float width, const float depth)
{

	float planeVert[3*3*2];
	float planeNormal[3*3*2];
	float planeTexCoord[2*3*2];

	ikdGetVertexPlane(planeVert, planeNormal, planeTexCoord);

	for (int i = 0; i < 6; i++) {
		planeVert[i*3] *= width;
		planeVert[i*3+2] *= depth;
	}



	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle   = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];


	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*2 * sizeof(float), planeVert, GL_STATIC_DRAW);

	//�@�����o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*2 * sizeof(float), planeNormal, GL_STATIC_DRAW);

	//�e�N�X�`���ʒu�o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*2 * sizeof(float), planeTexCoord, GL_STATIC_DRAW);


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

	//�C���f�b�N�X1�͖@�����o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X2�̓e�N�X�`�����_�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdScalablePlane(void)
{
	
	float planeVert[3*3*2];
	float planeNormal[3*3*2];
	float planeTexCoord[2*3*2];
	float planeTangent[3*3*2];
	float planeMoveVec[2*3*3] = {
		-0.5, 0.0, -0.5,
		0.5, 0.0, -0.5,
		0.5, 0.0, 0.5,

		0.5, 0.0, 0.5,
		-0.5, 0.0, 0.5,
		-0.5, 0.0, -0.5
		};

	ikdGetVertexPlane(planeVert, planeNormal, planeTexCoord);

	float startPos[] = {0.0, 0.0, 0.0};//�����_�͂Ƃ肠�������ׂ�0�Ƃ���
	for (int i = 0, index = 0; i < 2*3; i++, index += 3) {
		memcpy(planeTangent+index, startPos, sizeof(float) * 3);
		memcpy(planeVert+index, startPos, sizeof(float)*3);
	}	

	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle   = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle	= vboHandle[3];
	GLuint moveVecBufferHandle	= vboHandle[4];

	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*2 * sizeof(float), planeVert, GL_STATIC_DRAW);

	//�@�����o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*2 * sizeof(float), planeNormal, GL_STATIC_DRAW);

	
	//�e�N�X�`���ʒu�o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*2 * sizeof(float), planeTexCoord, GL_STATIC_DRAW);
	
	//�^���W�F���g�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*3 * sizeof(float), planeTangent, GL_STATIC_DRAW);

	//�g�傷�����
	glBindBuffer(GL_ARRAY_BUFFER, moveVecBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*3 * sizeof(float), planeMoveVec, GL_STATIC_DRAW);
	

	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);


	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X1�͖@�����o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	
	//�C���f�b�N�X2�̓e�N�X�`�����_�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	
	//�C���f�b�N�X3�̓^���W�F���g�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X4�͕������ߗp�̃o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, moveVecBufferHandle);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	

	return vaoHandle;
}

GLuint ikdObjects::ikdLoadObject(const char* fileName)
{
	int vertexNum = 0, normalNum = 0, texCoordNum = 0, faceNum = 0;
	char buf[200];


	//�t�@�C������̃f�[�^�̃��[�h/////////////////////////////////////////////////
	FILE *fp = fopen(fileName, "r");

	if (fp == NULL) {
		fprintf(stderr, "%s ��ǂݍ��߂܂���\n", fileName);

	}

	while (fgets(buf, 200, fp) != NULL) {
		switch(buf[0]) {
		case 'v':
			if (buf[1] == 't') texCoordNum++;
			else if (buf[1] == 'n') normalNum++;
			else vertexNum++;
			break;
		case 'f':
			faceNum++;
			break;
		default:
			break;
		}
	}

	float * vertices = (float *) malloc(sizeof(float)*vertexNum*3);
	float * normals = (float *)malloc(sizeof(float)*normalNum*3);
	float * texCoords = (float *)malloc(sizeof(float)*texCoordNum*2);
	//int * vertFaces = (int *)malloc(sizeof(int)*faceNum*4);
	int * vertFaces = (int *)malloc(sizeof(int)*faceNum*3);
	int * normalFaces = (int *)malloc(sizeof(int)*faceNum*4);
	int * texFaces = (int *)malloc(sizeof(int)*faceNum*4);

	rewind(fp);


	fprintf(stderr, "vertexNum: %d, faceNum: %d\n", vertexNum, faceNum);


	int vertindex = 0, normalindex = 0, texindex = 0, faceindex = 0;
	while (fgets(buf, 50, fp) != NULL) {

		switch(buf[0]) {
		case 'v':
			if (buf[1] == 't') {
				sscanf(buf, "vt %f %f", &texCoords[texindex + 0], &texCoords[texindex + 1]);
				texindex += 2;
			}
			else if (buf[1] == 'n') {
				sscanf(buf, "vn %f %f %f", &normals[normalindex + 0], &normals[normalindex + 1], &normals[normalindex + 2]);
				normalindex += 3;
			}
			else {
				sscanf(buf, "v %f %f %f", &vertices[vertindex + 0], &vertices[vertindex + 1], &vertices[vertindex + 2]);
				vertindex += 3;
			}
			break;
		case 'f':
			/*
			�e�N�X�`���A�@���͏ȗ��\
			�E���_/�e�N�X�`��/�@���S
			�E���_//�@��(//)2
			�E���_/�e�N�X�`���i/��j3
			�E���_(/����)�P

			�����̒��_���w�肳��Ă�����A���p�`��\���B
			*/
			{

				/*
				//�v�f�����J�E���g
				int valNum = 0;
				for (int i = 0; i < 200 || buf[i] != '\n'; i++) {
				if (buf[i] == ' ') valNum++;//�X�y�[�X��ɂ��v�f���������A����ł����B
				}
				*/

				if (strchr(buf, '/') == NULL) {
					//'/'��������΁A���_�̂݁B
					/*sscanf(buf, "f %d %d %d %d",
					&vertFaces[faceindex + 0],
					&vertFaces[faceindex + 1],
					&vertFaces[faceindex + 2],
					&vertFaces[faceindex + 3]);*/
					sscanf(buf, "f %d %d %d",
						&vertFaces[faceindex + 0],
						&vertFaces[faceindex + 1],
						&vertFaces[faceindex + 2]);

				} else if (strstr(buf, "//") != NULL) {
					//"//"������΁A���_�A�@��
					sscanf(buf, "f %d//%d %d//%d %d//%d %d//%d",
						&vertFaces[faceindex + 0], &normalFaces[faceindex + 0],
						&vertFaces[faceindex + 1], &normalFaces[faceindex + 1],
						&vertFaces[faceindex + 2], &normalFaces[faceindex + 2],
						&vertFaces[faceindex + 3], &normalFaces[faceindex + 3]);


				} else {
					//'/'�����邪�A"//"�͂Ȃ��@'/'�̐��łǂ���̃p�^�[�����𔻒f�B
					char typecheck[100];//�����_���������ׂ̈ɁA�ꉞ100�������̃o�b�t�@
					int slashNum = 0;
					sscanf(buf+2, "%s", typecheck);//�ǂݎ������s�́A�ŏ��̗v�f�����o���B
					for (int i = 0; i < 100; i++) {
						if (typecheck[i] == '/') slashNum++;//�P�v�f�����Ɋւ��āA'/'���������邩�J�E���g
					}

					if (slashNum == 1) {
						//���_�A�e�N�X�`��
						sscanf(buf, "f %d/%d %d/%d %d/%d %d/%d",
							&vertFaces[faceindex + 0], &texFaces[faceindex + 0],
							&vertFaces[faceindex + 1], &texFaces[faceindex + 1],
							&vertFaces[faceindex + 2], &texFaces[faceindex + 2],
							&vertFaces[faceindex + 3], &texFaces[faceindex + 3]);

					} else if (slashNum == 2) {
						//���_�A�e�N�X�`���A�@��

						sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
							&vertFaces[faceindex + 0], &texFaces[faceindex + 0], &normalFaces[faceindex + 0],
							&vertFaces[faceindex + 1], &texFaces[faceindex + 1], &normalFaces[faceindex + 1],
							&vertFaces[faceindex + 2], &texFaces[faceindex + 2], &normalFaces[faceindex + 2],
							&vertFaces[faceindex + 3], &texFaces[faceindex + 3], &normalFaces[faceindex + 3]);
					}



				}

				//faceindex += 4;
				faceindex += 3;


			}
			break;
		default:
			break;
		}
	}
	fclose(fp);
	//�f�[�^�̃��[�h�I��------------------------------------------------------------------------------------


	//���ۂ̒��_�f�[�^�쐬
	//float * objVertices = (float *)malloc(sizeof(float) * faceNum * 4 * 3);//�l�_��1��
	float * objVertices = (float *)malloc(sizeof(float) * faceNum * 3 * 3);//�O�_��1��
	float * objNormals = (float *)malloc(sizeof(float) * faceNum * 3 * 3);
	float * objTexCoords = (float *)malloc(sizeof(float) * faceNum * 4 * 2);
	float * objTangents = (float *)malloc(sizeof(float) * faceNum * 3 * 3);


	//���_�z��
	/*int index = 0;
	for (int i = 0; i < faceNum; i++) {
	for (int j = 0; j < 4; j++) {
	int ver = vertFaces[i*4+j];
	memcpy(objVertices+index, &vertices[ver], sizeof(float)*3);
	index += 3;
	}
	}*/


	int index = 0;
	for (int i = 0; i < faceNum; i++) {//faceNum -> f�̍s��
		for (int j = 0; j < 3; j++) {
			int ver = (vertFaces[i*3+j] - 1) * 3;//��_�ɂ��O�_�����Ă���B
			memcpy(objVertices+index, &vertices[ver], sizeof(float)*3);
			index += 3;
		}
	}


	//�@�����W
	index = 0;
	for (int i = 0; i < faceNum; i++) {//�ʂ̐������������������� �񐔂���������΂���
		glm::vec3 p1 = glm::vec3(objVertices[i*9+0], objVertices[i*9+1], objVertices[i*9+2]);
		glm::vec3 p2 = glm::vec3(objVertices[i*9+3], objVertices[i*9+4], objVertices[i*9+5]);
		glm::vec3 p3 = glm::vec3(objVertices[i*9+6], objVertices[i*9+7], objVertices[i*9+8]);

		glm::vec3 e1 = p2 - p1;
		glm::vec3 e2 = p3 - p2;

		glm::vec3 normal = glm::normalize(glm::cross(e1, e2));

		for (int j = 0; j < 3; j++) {
			memcpy(objNormals+index, &normal[0], sizeof(float)*3);
			index += 3;
		}
	}
	/*
	//�e�N�X�`�����W
	index = 0;
	for (int i = 0; i < faceNum; i++) {
	for (int j = 0; j < 4; j++) {
	int ver = texFaces[i*4 + j];
	memcpy(objTexCoords+index, &texCoords[ver], sizeof(float)*2);
	index += 2;
	}
	}
	*/

	

	//create vertex buffer object
	//GLuint vboHandle[3];
	//glGenBuffers(3, vboHandle);
	GLuint vboHandle[4];
	glGenBuffers(4, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle   = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle  = vboHandle[3];


	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, faceNum * 3 * 3 * sizeof(float), objVertices, GL_STATIC_DRAW);//�l�_�Ŗ�


	//�@�����o�b�t�@�Ƀf�[�^��}��
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, faceNum * 3 * 3 * sizeof(float), objNormals, GL_STATIC_DRAW);

	//�^���W�F���g�o�b�t�@�Ƀf�[�^����
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, faceNum * 3 * 3 * sizeof(float), objTangents, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);


	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X1�͖@�����o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X2�̓e�N�X�`�����_�o�b�t�@
	//glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	
	//�C���f�b�N�X3�̓^���W�F���g���o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);


	free(vertices);
	free(normals);
	free(texCoords);
	free(vertFaces);
	free(texFaces);
	free(normalFaces);
	free(objVertices);
	free(objNormals);
	free(objTexCoords);
	free(objTangents);

	return vaoHandle;
}

GLuint ikdObjects::ikdLoad_cyHairFile(ikdDataSet* pdataSet, float thickness)
{
	
	//�ϗ\��p�����[�^
	const int partitions = 10;//��{�̔��������ɋ�؂邩
	const int surfaces = 12;//�e�Z�O�����g�����ʂō�邩
	const float radius = thickness;//���}�`�̔��a

	//dataSet�Ɋ܂߂�\������
	const int meshedDataNum = partitions*surfaces*3*3*(pdataSet->getHairFileHeader().hair_count);//���b�V����������̑S�f�[�^��(�e���_��xyz�܂�)
	const int meshedVertexNum = partitions*surfaces*3*(pdataSet->getHairFileHeader().hair_count);//���_��
	const int meshedDataNum_onehair = partitions*surfaces*3*3;
	const int meshedVertexNum_onehair = partitions*surfaces*3;



	
	const float* pointsArray = pdataSet->getHairFilePointsArray();
	float * meshedHair = new float[meshedDataNum];//���ׂĂ̔���partitions�p�[�g, surfaces/2��(��ʂɓ񖇂̎O�p�`), 3���_, xyz�̃f�[�^
	float * meshedHairNormal = new float[meshedDataNum];//����`����, �S���_�ɑ΂��Ė@����ݒ肷��
	float * meshedHairTangent = new float[meshedDataNum];//���̗���̕����̃x�N�g��

	int nowHairStartArrayNum = 0, nowHairEndArrayNum;

	const unsigned short* segmentsArray = pdataSet->getHairFileSegmentsArray();
	int defaultSegs = 0;
	if (segmentsArray) {
		nowHairEndArrayNum = segmentsArray[0];//���ꂩ��`�����̎n�_�ƏI�_
	} else {//segmentsArray��null�̂Ƃ�
		defaultSegs = pdataSet->getHairFileHeader().d_segments;
		nowHairEndArrayNum = defaultSegs;
	}

	//�O�����āA��{�ƂȂ�Z�p�`������Ă���///////////////////////////////////////////////////////////////////////
	glm::vec4 oneHexPoint = glm::vec4(0.0, radius, 0.0, 1.0);//�Z�p�`���\������_�͂��̓_����ɍ��B
	glm::mat4 hexRotate = glm::rotate<float>(glm::mat4(1.0f), 360 / 6, glm::vec3(0.0, 0.0, 1.0));//�e�_���w�肷�邽�߂̉�]�s��
	float originalHexVertices[6*3];//���_�ɂ���Z�p�`
	for (int i = 0, index = 0; i < 6; i++, index+=3) {
		oneHexPoint = hexRotate * oneHexPoint;//��]�Ɋւ��ĐώZ���Ă����B
		memcpy(originalHexVertices+index, &oneHexPoint[0], sizeof(float)*3);
	}

	//----------------------------------------------------------------------------------------------------------------

	
	
	for (unsigned int nowHair = 0; nowHair < pdataSet->getHairFileHeader().hair_count; nowHair++) {//����{��{�ɂ���
		if (segmentsArray) {
			nowHairEndArrayNum = nowHairStartArrayNum + segmentsArray[nowHair];//���ݒ��ڂ��Ă���т̍Ō�̒��_
		} else {
			nowHairEndArrayNum = nowHairStartArrayNum + defaultSegs;
		}
		float * hexCylinderVert = new float[7*3*(partitions+1)];//��{�̔���partitions�񕪊����čl����B�ǂ�Ȗ����ɂȂ��Ă��A�Ō�Ɉꖇ�ǉ�����B

		//�Z�p�`��`�����W�����̔z��ɂ���B/////////////////////////////////////////////////////
		int pickupInterval;
		if (segmentsArray){
			pickupInterval = segmentsArray[nowHair] / partitions;//�Ԉ����čl���邽�߁A���Ԋu�œ_�𒊏o����B 
		} else {
			pickupInterval = defaultSegs / partitions;
		}
		float * pickedPointsArray = new float[3*(partitions+1)];

		for (int i = 0; i < (partitions + 1); i++) {
			memcpy(&pickedPointsArray[i*3], &pointsArray[nowHairStartArrayNum*3 + i*3*pickupInterval], sizeof(float)*3);
		}		
		//--------------------------------------------------------------------------------------//

		//�Z�p�`��`��//////////////////////////////////////////////////////////////////////////////////////
		glm::vec3 prevHexApper = glm::vec3(0.0, 1.0, 0.0);//��{�`�̏����
		for (int segmentFace = 0; segmentFace < (partitions + 1); segmentFace++) {//�e�Z�p�`�Ɋւ���
			glm::vec3 p1, p2;
			if (segmentFace == partitions) {
				//�Ō�̈ꖇ�ɂȂ�����
				p1 = glm::vec3(pointsArray[nowHairEndArrayNum*3], 
					pointsArray[nowHairEndArrayNum*3 + 1], 
					pointsArray[nowHairEndArrayNum*3 + 2]);

				p2 = glm::vec3(pointsArray[nowHairEndArrayNum*3-3], 
					pointsArray[nowHairEndArrayNum*3-3+1], 
					pointsArray[nowHairEndArrayNum*3-3+2]);				
			} else {		
				p1 = glm::vec3(pickedPointsArray[segmentFace * 3], 
					pickedPointsArray[segmentFace * 3 + 1], 
					pickedPointsArray[segmentFace * 3 + 2]);//�Z�p������镔���̐擪

				p2 = glm::vec3(pickedPointsArray[segmentFace * 3 + 3], 
					pickedPointsArray[segmentFace * 3 + 4], 
					pickedPointsArray[segmentFace * 3 + 5]);//�Z�p������镔���̏I�_
			}

			//���W�ϊ��̂��߂̃x�N�g���ݒ�////////////////////////////////////////////////////////////////////////////////
			glm::vec3 hexApper;
			if (segmentFace == 0) {
				hexApper = glm::vec3(0.0, 1.0, 0.0);//�Z�p�`��apper y
			} else {
				hexApper = prevHexApper;
			}
			
			float hexVertices[6*3];//�����_�ł̘Z�p�`�̒��_��ۑ����邽�߂̔z��B

			glm::mat4 hexTranslate = glm::translate<float>(glm::mat4(1.0f), glm::vec3(p1));//���̃Z�O�����g�ւ̈ړ��s��

			glm::vec3 segNormal;
			if (segmentFace == partitions) {
				segNormal = glm::normalize(p1 - p2);//���킹�����@�� z'
				segNormal = glm::vec3(-segNormal[0],-segNormal[1],-segNormal[2]);
			} else {
				segNormal = glm::normalize(p2 - p1);//���킹�����@�� z'
			}
			
			glm::vec3 crossVec = glm::normalize(glm::cross(hexApper, segNormal));//x'

			glm::vec3 comvertedHexApper = glm::cross(segNormal, crossVec);//y'

			//���ϊ��s��@�C�Ӎ��W�n�ɑ΂��Đ�������]������悤�ȍs��
			glm::mat4 hexCoordComvert4(glm::vec4(crossVec[0], comvertedHexApper[0], segNormal[0], 0),
				glm::vec4(crossVec[1], comvertedHexApper[1], segNormal[1], 0),
				glm::vec4(crossVec[2], comvertedHexApper[2], segNormal[2], 0),
				glm::vec4(0, 0, 0, 1));

			hexCoordComvert4 = glm::transpose(hexCoordComvert4);
			//���W�ϊ��̂��߂̃x�N�g���ݒ�----------------------------------------------------------------------------------

			//���_�Z�̍��W�̌v�Z		
			for (int i = 0; i < 6; i++) {
				glm::vec4 changePoint = glm::vec4(originalHexVertices[i*3],originalHexVertices[i*3+1],originalHexVertices[i*3+2], 1.0);
				changePoint = hexTranslate * hexCoordComvert4 * changePoint;//�ώZ����Ă͍���B

				memcpy(&hexVertices[i*3], &changePoint[0], sizeof(float) * 3);//����̘Z���_
			}

			//��ō�������_6��z��ɂ܂Ƃ߂�B
			int index = 0;
			for (int i = 0; i < 7; i++) {//hex[]�ɁA�e�ʂ��\�����钸�_����͂���B
				if (i == 6) {//��������Ƃ��́A0�ƑS���������W������B
					memcpy(hexCylinderVert + segmentFace * 7 * 3 + index, &hexVertices[0], sizeof(float) * 3);
				} else {
					memcpy(hexCylinderVert + segmentFace * 7 * 3 + index, &hexVertices[i * 3], sizeof(float) * 3);
				}
				index += 3;
			}
			prevHexApper = comvertedHexApper;
		}//�e�Z�p�`�Ɋւ���---------------------------------------------------------------------------------------------

		delete[] pickedPointsArray;




		//���̎��_�ŁAhexCylinderVert�ɔ���{�̘Z�p�`�̒��_�����ׂĕۑ�����Ă���B
		//�f�[�^�𒊏o���āA���b�V�����\�����钸�_���w�肷��/////////////////////////////////////////////////////////////////
		float * hexCylinder = new float[meshedDataNum_onehair];//hexcylindervert����A���݂̎O�p�`��`���̂ɕK�v�Ȓ��_���̗p����
	
		for (int i = 0, index = 0; i < partitions; i++) {//�e�Z�O�����g�ɂ��čs��
			//�㕔���̎O�p�Z��
			for (int j = i*7; j < i*7+6; j++) {
				memcpy(&hexCylinder[index + 0], &hexCylinderVert[j*3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 3], &hexCylinderVert[(j+1) * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 6], &hexCylinderVert[(j+7) * 3], sizeof(float)*3);
				index += 9;
			}

			//�������̎O�p�Z��
			for (int j = (i+1)*7; j < (i+1)*7+6; j++) {
				memcpy(&hexCylinder[index + 0], &hexCylinderVert[j*3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 3], &hexCylinderVert[(j-6)*3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 6], &hexCylinderVert[(j+1)*3], sizeof(float)*3);
				index += 9;
			}
		}

		memcpy(&meshedHair[meshedDataNum_onehair*nowHair], hexCylinder, sizeof(float)*meshedDataNum_onehair);
		//-------------------------------------------------------------------------------------------------------------------


		//hexCylindervert�̍��W�𗘗p���āA�e�ʂ̖@����ݒ肷��B
		int normalIndex = 0;
		for (int i = 0; i < partitions; i++) {//�e�Z�p���Ɋւ���
			float hexNormals[6*3];
			int index = 0;
			for (int j = i*7; j < i*7+6; j++) {//�e�ʂɊւ��Ė@���x�N�g�����쐬
				glm::vec3 p1, p2, p3;
				memcpy(&p1[0], &hexCylinderVert[j*3], sizeof(float)*3);
				memcpy(&p2[0], &hexCylinderVert[(j+1)*3], sizeof(float)*3);
				memcpy(&p3[0], &hexCylinderVert[(j+7)*3], sizeof(float)*3);

				glm::vec3 e1 = p2 - p1;
				glm::vec3 e2 = p3 - p2;

				glm::vec3 normal = glm::normalize(glm::cross(e1, e2));//���݂̖ʂ̖@�����o�͂����

				memcpy(hexNormals+index, &normal[0], sizeof(float)*3);

				index += 3;
			}

			for (int j = 0; j < 2; j++) {//�쐬�����@���x�N�g���𑗂�z��ɃR�s�[
				for (int k = 0; k < 6; k++) {
					memcpy(&meshedHairNormal[meshedDataNum_onehair*nowHair+normalIndex],   &hexNormals[k*3], sizeof(float)*3);
					memcpy(&meshedHairNormal[meshedDataNum_onehair*nowHair+normalIndex+3], &hexNormals[k*3], sizeof(float)*3);
					memcpy(&meshedHairNormal[meshedDataNum_onehair*nowHair+normalIndex+6], &hexNormals[k*3], sizeof(float)*3);

					normalIndex += 9;
				}
			}

		
		}
		
		
		//�e���_�ɂ�����tangent�x�N�g�����v�Z����
		int tangentIndex =0;
		for (int i = 0; i < partitions; i++) {
			glm::vec3 tangent;	
			int index = 0;

			glm::vec3 p1, p2;
			memcpy(&p1[0], &hexCylinderVert[i*7*3], sizeof(float)*3);
			memcpy(&p2[0], &hexCylinderVert[(i+1)*7*3], sizeof(float)*3);

			tangent = p2 - p1;

			for (int j = 0; j < 2; j++) {//�Z�p���̏㉺
				for (int k = 0; k < 6; k++) {//�e������O�p�`��6�`����Ă���
					memcpy(&meshedHairTangent[meshedDataNum_onehair*nowHair+tangentIndex],   &tangent[0], sizeof(float)*3);
					memcpy(&meshedHairTangent[meshedDataNum_onehair*nowHair+tangentIndex+3], &tangent[0], sizeof(float)*3);
					memcpy(&meshedHairTangent[meshedDataNum_onehair*nowHair+tangentIndex+6], &tangent[0], sizeof(float)*3);

					tangentIndex += 9;
				}
			}
		}
		

		//-------------------------------------------------------------------------------------------------------------------

		delete[] hexCylinderVert;
		delete[] hexCylinder;


		if (segmentsArray) {
			nowHairStartArrayNum += segmentsArray[nowHair]+1;
		} else {
			nowHairStartArrayNum += defaultSegs + 1;
		}





	}//����{��{�ɂ���-------------------------------------------------------------------------------------------
	
	/*
	glm::vec3 test = glm::vec3(1.0, 1.0, 1.0);
	int index = 0;
	for (int i = 0; i < meshedVertexNum; i++) {
		memcpy(meshedHairTangent+index, &test[0], sizeof(float)*3);
		index += 3;
	}
	*/



	//�o�E���f�B���O�{�b�N�X����邽�߂̍��W��ۑ��B
	float min[3], max[3];
	for (int i = 0; i < 3; i++) {
		memset(&min[i], FLT_MAX, sizeof(float));
		memset(&max[i], FLT_MIN, sizeof(float));
	}

	//�o�E���f�B���O�{�b�N�X�̌v�Z�p���W���擾���Ă���
	for (int i = 0; i < meshedVertexNum; i++) {
		for (int j = 0; j < 3; j++) {
			if (meshedHair[i*3+j] > max[j]) max[j] = meshedHair[i*3+j];
			if (meshedHair[i*3+j] < min[j]) min[j] = meshedHair[i*3+j];
		}
	}
	pdataSet->setBoundingPos(min, max);


	//create vertex buffer object
	GLuint vboHandle[4];
	glGenBuffers(4, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];



	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHair, GL_STATIC_DRAW);

	//�@���o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairNormal, GL_STATIC_DRAW);
	
	//tangent�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), NULL, GL_STATIC_DRAW);

	//tangent�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairTangent, GL_STATIC_DRAW);
	
	delete[] meshedHair;
	delete[] meshedHairNormal;
	delete[] meshedHairTangent;
	
	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X1�͖@���o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X2�̓e�N�X�`�����W�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X3��tangent�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdScalable_cyHairFile(ikdDataSet* pdataSet)
{


  //�ϗ\��p�����[�^
    const int partitions = 10;//��{�̔��������ɋ�؂邩
	const int surfaces = 6*2;//�e�Z�O�����g�����ʂō�邩
	const float radius = 0.1;//���}�`�̔��a

	//dataSet�Ɋ܂߂�\������
	const int meshedDataNum = partitions*surfaces*3*3*(pdataSet->getHairFileHeader().hair_count);//���b�V����������̑S�f�[�^��(�e���_��xyz�܂�)
	const int meshedVertexNum = partitions*surfaces*3*(pdataSet->getHairFileHeader().hair_count);//���_��
	const int meshedDataNum_onehair = partitions*surfaces*3*3;
	const int meshedVertexNum_onehair = partitions*surfaces*3;



	const unsigned short* segmentsArray = pdataSet->getHairFileSegmentsArray();
	const float* pointsArray = pdataSet->getHairFilePointsArray();
	float * meshedHair = new float[meshedDataNum];//���ׂĂ̔���partitions�p�[�g, surfaces/2��(��ʂɓ񖇂̎O�p�`), 3���_, xyz�̃f�[�^
	float * meshedHairNormal = new float[meshedDataNum];//����`����, �S���_�ɑ΂��Ė@����ݒ肷��
	float * meshedHairTangent = new float[meshedDataNum];//���̗���̕����̃x�N�g��
	int nowHairStartArrayNum = 0, nowHairEndArrayNum;//���ꂩ��`�����̎n�_�ƏI�_
	int defaultSegs = 0;
	if (segmentsArray) {
		nowHairEndArrayNum = segmentsArray[0];//���ꂩ��`�����̎n�_�ƏI�_
	} else {//segmentsArray��null�̂Ƃ�
		defaultSegs = pdataSet->getHairFileHeader().d_segments;
		nowHairEndArrayNum = defaultSegs;
	} 



	//�O�����āA��{�ƂȂ�Z�p�`������Ă���///////////////////////////////////////////////////////////////////////
	glm::vec4 oneHexPoint = glm::vec4(0.0, radius, 0.0, 1.0);//�Z�p�`���\������_�͂��̓_����ɍ��B
	glm::mat4 hexRotate = glm::rotate<float>(glm::mat4(1.0f), 360 / 6, glm::vec3(0.0, 0.0, 1.0));//�e�_���w�肷�邽�߂̉�]�s��
	float originalHexVertices[6*3];//���_�ɂ���Z�p�`
	for (int i = 0, index = 0; i < 6; i++, index+=3) {
		oneHexPoint = hexRotate * oneHexPoint;//��]�Ɋւ��ĐώZ���Ă����B
		memcpy(originalHexVertices+index, &oneHexPoint[0], sizeof(float)*3);
	}



	for (unsigned int nowHair = 0; nowHair < pdataSet->getHairFileHeader().hair_count; nowHair++) {//����{��{�ɂ���
      if (segmentsArray) {
		  nowHairEndArrayNum = nowHairStartArrayNum + segmentsArray[nowHair];//���ݒ��ڂ��Ă���т̍Ō�̒��_
	  } else {
		  nowHairEndArrayNum = nowHairStartArrayNum + defaultSegs;
	  }
        //�z�u�����Z�p�`�̒��_�ۑ��p
		float * hexCylinderVert = new float[7*3*(partitions+1)];

		int pickupInterval = 0;
        //�Z�p�`��`�����_��I�яo��
		if (segmentsArray) {
	        pickupInterval = segmentsArray[nowHair] / partitions;//�Ԉ����čl���邽�߁A���Ԋu�œ_�𒊏o����B 
		} else {
			pickupInterval = defaultSegs / partitions;
		}
		float * pickedPointsArray = new float[3*(partitions+1)];
		for (int i = 0, interval = 0; i < (partitions + 1); i++, interval += 3) {
			if (i == partitions) memcpy(pickedPointsArray + interval, &pointsArray[nowHairEndArrayNum * 3], sizeof(float) * 3);
			else memcpy(pickedPointsArray + interval, &pointsArray[nowHairStartArrayNum * 3 + i*3*pickupInterval], sizeof(float)*3);
		}		



        //�x�N�g�������p
        glm::vec3 prevHexApper = glm::vec3(0.0, 1.0, 0.0);//��{�`�̏����

		//�Z�p�`��`��
		for (int segmentFace = 0; segmentFace < (partitions + 1); segmentFace++) {//�e�Z�p�`�Ɋւ���
			glm::vec3 p1, p2;
            p1 = glm::vec3(
						pickedPointsArray[segmentFace * 3], 
						pickedPointsArray[segmentFace * 3 + 1], 
						pickedPointsArray[segmentFace * 3 + 2]);
			if (segmentFace == partitions) {
				//�Ō�̈ꖇ�ɂȂ�����
				p2 = glm::vec3(
						pickedPointsArray[(segmentFace - 1) * 3], 
						pickedPointsArray[(segmentFace - 1) * 3 + 1], 
						pickedPointsArray[(segmentFace - 1) * 3 + 2]);				
			} else {		
				p2 = glm::vec3(
						pickedPointsArray[(segmentFace + 1) * 3], 
						pickedPointsArray[(segmentFace + 1) * 3 + 1], 
						pickedPointsArray[(segmentFace + 1) * 3 + 2]);//�Z�p������镔���̏I�_
			}

			//���W�ϊ��̂��߂̃x�N�g���ݒ�////////////////////////////////////////////////////////////////////////////////
			glm::vec3 hexApper;
			if (segmentFace == 0) hexApper = glm::vec3(0.0, 1.0, 0.0);//�Z�p�`��apper
			else hexApper = prevHexApper;
			
			

            //�Z�p�`�̈ړ�
			glm::mat4 hexTranslate = glm::translate<float>(glm::mat4(1.0f), glm::vec3(p1));

			glm::vec3 segNormal;
			if (segmentFace == partitions) {
				segNormal = glm::normalize(p1 - p2);//���킹�����@�� z'
                //segNormal = -segNormal;
			} else {
				segNormal = glm::normalize(p2 - p1);//���킹�����@�� z'
			}
			
			glm::vec3 crossVec = glm::normalize(glm::cross(hexApper, segNormal));//x'

			glm::vec3 comvertedHexApper = glm::normalize(glm::cross(segNormal, crossVec));//y'

			//���ϊ��s��@�C�Ӎ��W�n���킹�ĉ�]������悤�ȍs��
			glm::mat4 hexCoordComvert4(
				glm::vec4(crossVec[0], comvertedHexApper[0], segNormal[0], 0),
				glm::vec4(crossVec[1], comvertedHexApper[1], segNormal[1], 0),
				glm::vec4(crossVec[2], comvertedHexApper[2], segNormal[2], 0),
				glm::vec4(0, 0, 0, 1));

			hexCoordComvert4 = glm::transpose(hexCoordComvert4);
			//���W�ϊ��̂��߂̃x�N�g���ݒ�----------------------------------------------------------------------------------



            float hexVertices[6*3];//�Z�p�`�ꖇ�̍��W������

			//�Z�p�`�ꖇ�̌v�Z	
			for (int i = 0, index = 0; i < 6; i++, index += 3) {
				glm::vec4 changePoint = glm::vec4(originalHexVertices[i*3],originalHexVertices[i*3+1],originalHexVertices[i*3+2], 1.0);
				changePoint = hexTranslate * hexCoordComvert4 * changePoint;

				memcpy(hexVertices + index, &changePoint[0], sizeof(float) * 3);//�Z�p�`�ꖇ�̒��_
			}

			//�Z�p�`�𔯈�{�̍��W������z��ɂ܂Ƃ߂�
			for (int i = 0, index = 0; i < 7; i++, index += 3) {//�e�ʂ��\�����钸�_����͂���B
				if (i == 6) 
					memcpy(hexCylinderVert + segmentFace * 7 * 3 + index, &hexVertices[0], sizeof(float) * 3);
				else 
					memcpy(hexCylinderVert + segmentFace * 7 * 3 + index, &hexVertices[i * 3], sizeof(float) * 3);
			}

           

			prevHexApper = comvertedHexApper;
		}//�e�Z�p�`�Ɋւ���---------------------------------------------------------------------------------------------

		

		//hexcylindervert�̒��_����ʂ�`��
		float * hexCylinder = new float[meshedDataNum_onehair];
		for (int row = 0, index = 0; row < partitions; row++) {//�e�Z�O�����g�ɂ��čs��
			int nwRArNum = row * 7;//nowRowArrayNum  
            int nxRArNum = (row + 1) * 7;//nextRowArrayNum

			for (int column = 0; column < 6; column++) {
				memcpy(&hexCylinder[index + 0], &hexCylinderVert[(nwRArNum + column)     * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 3], &hexCylinderVert[(nwRArNum + column + 1) * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 6], &hexCylinderVert[(nxRArNum + column)     * 3], sizeof(float)*3);

				memcpy(&hexCylinder[index + 9], &hexCylinderVert[(nxRArNum + column)     * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index +12], &hexCylinderVert[(nwRArNum + column + 1) * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index +15], &hexCylinderVert[(nxRArNum + column + 1) * 3], sizeof(float)*3);
				
                index += 18;
			}	
		}

		memcpy(&meshedHair[meshedDataNum_onehair * nowHair], hexCylinder, sizeof(float) * meshedDataNum_onehair);
		//-------------------------------------------------------------------------------------------------------------------





        //�@���̍쐬
        //�P���Ɏ����Ɨׂō�����@��
        float simpleNormalVec[partitions+1][(surfaces / 2)+1][3];
        for (int row = 0; row < partitions+1; row++) {
          for (int column = 0; column < surfaces / 2+1; column++) {
            if (column == surfaces / 2) {
              memcpy(simpleNormalVec[row][column], simpleNormalVec[row][0], sizeof(float) * 3);
            } else {
              int nwRArNum = row * (surfaces / 2 + 1);
              int nxRArNum = (row + 1) * (surfaces / 2 + 1);//nextRowArrayNum
              int prRArNum = (row - 1) * (surfaces / 2 + 1);
			  glm::vec3 p1, p2, p3;

			  memcpy(&p1[0], &hexCylinderVert[(nwRArNum + column)     * 3], sizeof(float)*3);
			  memcpy(&p2[0], &hexCylinderVert[(nwRArNum + column + 1) * 3], sizeof(float)*3);
			  if (row == partitions)
                memcpy(&p3[0], &hexCylinderVert[(prRArNum + column)     * 3], sizeof(float)*3);
              else 
                memcpy(&p3[0], &hexCylinderVert[(nxRArNum + column)     * 3], sizeof(float)*3);

			  glm::vec3 e1 = p2 - p1;
			  glm::vec3 e2 = p3 - p1;
              glm::vec3 normal;
              if (row == partitions) {
                normal = glm::normalize(glm::cross(e2, e1));
              } else {
  			    normal = glm::normalize(glm::cross(e1, e2));
              }
  			  memcpy(simpleNormalVec[row][column], &normal[0], sizeof(float) * 3);
            }
          }
        }

        //�����̉E���g���Ė@����������̂�, ���ƍ���
        float left_right_margeNormalVec[partitions+1][(surfaces / 2)+1][3];
        for (int row = 0; row < partitions+1; row++) {
          for (int column = 0; column < surfaces / 2 + 1; column++) {
            if (column == surfaces / 2) {
              memcpy(left_right_margeNormalVec[row][column], left_right_margeNormalVec[row][0], sizeof(float) * 3);   
            } else {
              glm::vec3 p1, p2;
              glm::vec3 normal;

              memcpy(&p1[0], simpleNormalVec[row][column], sizeof(float) * 3);

              if (column == 0) {
                memcpy(&p2[0], simpleNormalVec[row][surfaces / 2 - 1], sizeof(float) * 3);
              } else {
                memcpy(&p2[0], simpleNormalVec[row][column-1], sizeof(float) * 3);
              }

              normal = glm::normalize(p1 + p2);

              memcpy(left_right_margeNormalVec[row][column], &normal[0], sizeof(float) * 3);
            }
          }
        }

        //�㉺������
        float normalVec[partitions+1][(surfaces / 2) + 1][3];
        for (int row = 0; row < partitions + 1; row++) {
          for (int column = 0; column < surfaces / 2 + 1; column++) {
            if (column == surfaces / 2) {
              memcpy(normalVec[row][column], normalVec[row][0], sizeof(float) * 3);
            } else {
              glm::vec3 p1, p2;
              glm::vec3 normal;


              if (row == 0) {
                memcpy(normalVec[row][column], left_right_margeNormalVec[row][column], sizeof(float) * 3); 
              } else {
                memcpy(&p1[0], left_right_margeNormalVec[row][column], sizeof(float) * 3);
                memcpy(&p2[0], left_right_margeNormalVec[row-1][column], sizeof(float) * 3);

                normal = glm::normalize(p1+p2);
                memcpy(normalVec[row][column], &normal[0], sizeof(float)*3);
              }
            }
          }
        }

      
        
        int nowHairIndex = meshedDataNum_onehair * nowHair;
        for (int row = 0; row < partitions; row++) {
          const int nowRowIndex = row * surfaces * 3 * 3; 
          for (int column = 0, index = 0; column < surfaces / 2; column++, index += 18) {
            
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index + 0], &normalVec[row][column], sizeof(float) * 3);
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index + 3], &normalVec[row][column+1], sizeof(float) * 3);
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index + 6], &normalVec[row+1][column], sizeof(float) * 3);

            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index + 9], &normalVec[row+1][column], sizeof(float) * 3);
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index +12], &normalVec[row][column+1], sizeof(float) * 3);
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index +15], &normalVec[row+1][column+1], sizeof(float) * 3);
          }
        }
        
		//-------------------------------------------------------------------------------------------------------------------
    
        //tangent vector
        float tangentVec[partitions+1][3];
		for (int row = 0; row < partitions+1; row++) {
			glm::vec3 tangent;//local
            int nwRArNum = row * 3;
            int nxRArNum = (row + 1) * 3;
			
            if (row == partitions) {
                memcpy(tangentVec[row], tangentVec[partitions - 1], sizeof(float) * 3);
            } else {
		        glm::vec3 p1, p2;
		        memcpy(&p1[0], &pickedPointsArray[nwRArNum], sizeof(float) * 3);
		        memcpy(&p2[0], &pickedPointsArray[nxRArNum], sizeof(float) * 3);

		        tangent = glm::normalize(p2 - p1);


                memcpy(tangentVec[row], &tangent[0], sizeof(float) * 3);
            }
		}

        for (int row = 0; row < partitions; row++) {
            int nowRowIndex = row * surfaces * 3 * 3;
            for (int column = 0, index = 0; column < surfaces / 2; column++, index += 18) {
                //��̎O�p�`
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index + 0], &tangentVec[row], sizeof(float) * 3);
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index + 3], &tangentVec[row], sizeof(float) * 3);
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index + 6], &tangentVec[row+1], sizeof(float) * 3);

                //���̎O�p�`
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index + 9], &tangentVec[row+1], sizeof(float) * 3);
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index +12], &tangentVec[row], sizeof(float) * 3);
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index +15], &tangentVec[row+1], sizeof(float) * 3);
            }
        }
		//-------------------------------------------------------------------------------------------------------------------
        delete[] pickedPointsArray;
		delete[] hexCylinderVert;
		delete[] hexCylinder;

		if (segmentsArray) {
			nowHairStartArrayNum += segmentsArray[nowHair] + 1;
		} else {
			nowHairStartArrayNum += defaultSegs + 1;
		}
	}//����{��{�ɂ���-------------------------------------------------------------------------------------------
      


	//�o�E���f�B���O�{�b�N�X����邽�߂̍��W��ۑ��B
	float min[3], max[3];
	for (int i = 0; i < 3; i++) {
		memset(&min[i], FLT_MAX, sizeof(float));
		memset(&max[i], FLT_MIN, sizeof(float));
	}

	//�o�E���f�B���O�{�b�N�X�̌v�Z�p���W���擾���Ă���
	for (int i = 0; i < meshedVertexNum; i++) {
		for (int j = 0; j < 3; j++) {
			if (meshedHair[i*3+j] > max[j]) max[j] = meshedHair[i*3+j];
			if (meshedHair[i*3+j] < min[j]) min[j] = meshedHair[i*3+j];
		}
	}
	pdataSet->setBoundingPos(min, max);


	//create vertex buffer object
	GLuint vboHandle[4];
	glGenBuffers(4, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];



	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHair, GL_STATIC_DRAW);

	//�@���o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairNormal, GL_STATIC_DRAW);
	
	//texture�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), NULL, GL_STATIC_DRAW);

    /*
    float* testColor = new float[meshedVertexNum];
    for (int i = 0; i < meshedVertexNum; i++) {
      if (i % 36 >= 0 && i % 36 <= 5) {
        testColor[i] = 0.0;
      } else if (i % 36 >= 6 && i % 36 <= 11) {
        testColor[i] = 1.0;
      } else if (i % 36 >= 12 && i % 36 <= 17) {
        testColor[i] = 2.0;
      } else if (i % 36 >= 18 && i % 36 <= 23) {
        testColor[i] = 3.0;
      } else if (i % 36 >= 24 && i % 36 <= 29) {
        testColor[i] = 4.0;
      } else if (i % 36 >= 30 && i % 36 <= 35) {
        testColor[i] = 5.0;
      }
    }

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, meshedVertexNum * sizeof(float), testColor,GL_STATIC_DRAW);
    */

	//tangent�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairTangent, GL_STATIC_DRAW);
	
	delete[] meshedHair;
	delete[] meshedHairNormal;
	delete[] meshedHairTangent;
    //delete[] testColor;

	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X1�͖@���o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    
	//�C���f�b�N�X2�̓e�N�X�`�����W�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    /*
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	*/
    
    //�C���f�b�N�X3��tangent�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);



	return vaoHandle;
    
}

GLuint ikdObjects::ikdRibbon_cyHairFile(ikdDataSet* pdataSet)
{


  //�ϗ\��p�����[�^
    const int partitions = 10;//��{�̔��������ɋ�؂邩
	const int surfaces = 2;//�e�Z�O�����g�����ʂō�邩�@�O�p�񖇂ō���Ă݂�
	

	//dataSet�Ɋ܂߂�\������
	const int meshedDataNum				= (partitions + 1) * 2 * 3 * (pdataSet->getHairFileHeader().hair_count);//���b�V����������̑S�f�[�^��(�e���_��xyz�܂�)
	const int meshedDataNum_onehair		= (partitions + 1) * 2 * 3;
	const int meshedVertexNum			= (partitions + 1) * 2 * (pdataSet->getHairFileHeader().hair_count);//���_�͋��L����
	const int meshedVertexNum_onehair	= (partitions + 1) * 2;
	const int meshedIndexNum			= partitions * 2 * 3 * (pdataSet->getHairFileHeader().hair_count);
	const int meshedIndexNum_onehair	= partitions * 2 * 3;


	const unsigned short* segmentsArray = pdataSet->getHairFileSegmentsArray();
	const float* pointsArray = pdataSet->getHairFilePointsArray();
	float * meshedHair = new float[meshedDataNum];//���ׂĂ̔���partitions�p�[�g, surfaces/2��(��ʂɓ񖇂̎O�p�`), 3���_, xyz�̃f�[�^
	unsigned int * meshedHairIndex = new unsigned int[meshedIndexNum];
	float * meshedHairNormal = new float[meshedDataNum];//����`����, �S���_�ɑ΂��Ė@����ݒ肷��
	float * meshedHairTangent = new float[meshedDataNum];//���̗���̕����̃x�N�g��
	int nowHairStartArrayNum = 0, nowHairEndArrayNum;//���ꂩ��`�����̎n�_�ƏI�_
	int defaultSegs = 0;
	if (segmentsArray) {
		nowHairEndArrayNum = segmentsArray[0];//���ꂩ��`�����̎n�_�ƏI�_
	} else {//segmentsArray��null�̂Ƃ�
		defaultSegs = pdataSet->getHairFileHeader().d_segments;
		nowHairEndArrayNum = defaultSegs;
	} 

	//���ɂȂ钸�_���
	glm::vec4 originRibbonVert = glm::vec4(0.0, 0.0, 0.0, 1.0);

	//index�̍쐬
	//�C���f�b�N�X�̌����쐬 ����Ɏ�������č��W�̎w����s���B
	unsigned int singleHairIndex[meshedIndexNum_onehair];
	for (int i = 0; i < partitions; i++) {
		if (i == 0) {
			singleHairIndex[0] = 0;
			singleHairIndex[1] = 1;
			singleHairIndex[2] = 2;
			singleHairIndex[3] = 2;
			singleHairIndex[4] = 1;
			singleHairIndex[5] = 3;
		} else {
			int nowStart = i * 6;
			int prevStart = (i - 1) * 6;
			for (int j = 0; j < 6; j++) {
				singleHairIndex[nowStart + j]	= singleHairIndex[prevStart + j] + 2;
			}
		}
	}





	for (unsigned int nowHair = 0; nowHair < pdataSet->getHairFileHeader().hair_count; nowHair++) {//����{��{�ɂ���
		  if (segmentsArray) {
			  nowHairEndArrayNum = nowHairStartArrayNum + segmentsArray[nowHair];//���ݒ��ڂ��Ă���т̍Ō�̒��_
		  } else {
			  nowHairEndArrayNum = nowHairStartArrayNum + defaultSegs;
		  }
        //����{�̒��_��ۑ�
		float * ribbonVert = new float[meshedDataNum_onehair];

		int pickupInterval = 0;
        
		//�߂̈ʒu
		if (segmentsArray) {
	        pickupInterval = segmentsArray[nowHair] / partitions;//�Ԉ����čl���邽�߁A���Ԋu�œ_�𒊏o����B 
		} else {
			pickupInterval = defaultSegs / partitions;
		}
		float * pickedPointsArray = new float[(partitions+1) * 3];
		for (int i = 0, interval = 0; i < (partitions + 1); i++, interval += 3) {
			if (i == partitions) memcpy(pickedPointsArray + interval, &pointsArray[nowHairEndArrayNum * 3], sizeof(float) * 3);
			else memcpy(pickedPointsArray + interval, &pointsArray[nowHairStartArrayNum * 3 + i*3*pickupInterval], sizeof(float)*3);
		}		



        //�x�N�g�������p
        glm::vec3 prevRibbonApper = glm::vec3(0.0, 1.0, 0.0);//��{�`�̏����

		//2���_�̈ʒu�����߂�
		for (int segmentFace = 0; segmentFace < (partitions + 1); segmentFace++) {//�e2���_�Ɋւ���
			glm::vec3 p1, p2;
            p1 = glm::vec3(
						pickedPointsArray[segmentFace * 3], 
						pickedPointsArray[segmentFace * 3 + 1], 
						pickedPointsArray[segmentFace * 3 + 2]);
			if (segmentFace == partitions) {
				p2 = glm::vec3(
						pickedPointsArray[(segmentFace - 1) * 3], 
						pickedPointsArray[(segmentFace - 1) * 3 + 1], 
						pickedPointsArray[(segmentFace - 1) * 3 + 2]);				
			} else {		
				p2 = glm::vec3(
						pickedPointsArray[(segmentFace + 1) * 3], 
						pickedPointsArray[(segmentFace + 1) * 3 + 1], 
						pickedPointsArray[(segmentFace + 1) * 3 + 2]);
			}

			//���W�ϊ��̊��ϊ��x�N�g��////////////////////////////////////////////////////////////////////////////////
			glm::vec3 ribbonApper;
			if (segmentFace == 0) ribbonApper = glm::vec3(0.0, 1.0, 0.0);
			else ribbonApper = prevRibbonApper;
			
            //2���_�̈ړ�
			glm::mat4 ribbonTranslate = glm::translate<float>(glm::mat4(1.0f), glm::vec3(p1));

			glm::vec3 segNormal;
			if (segmentFace == partitions) {
				segNormal = glm::normalize(p1 - p2);//�ϊ����z'
			} else {
				segNormal = glm::normalize(p2 - p1);//�ϊ����z'
			}
			
			glm::vec3 crossVec = glm::normalize(glm::cross(ribbonApper, segNormal));//�ϊ����x'

			glm::vec3 comvertedRibbonApper = glm::normalize(glm::cross(segNormal, crossVec));//�ϊ����y'

			//���ϊ��s��@�C�Ӎ��W�n���킹�ĉ�]������悤�ȍs��
			glm::mat4 ribbonCoordComvert(
				glm::vec4(crossVec[0], comvertedRibbonApper[0], segNormal[0], 0),
				glm::vec4(crossVec[1], comvertedRibbonApper[1], segNormal[1], 0),
				glm::vec4(crossVec[2], comvertedRibbonApper[2], segNormal[2], 0),
				glm::vec4(0, 0, 0, 1));

			ribbonCoordComvert = glm::transpose(ribbonCoordComvert);
			//���W�ϊ��̂��߂̃x�N�g���ݒ�----------------------------------------------------------------------------------

            float nowRibbonVertices[2*3];//�ϊ����2���_�̍��W

			//2���_�̌v�Z
			for (int i = 0, index = 0; i < 2; i++, index += 3) {
				glm::vec4 changePoint = glm::vec4(originRibbonVert[0], originRibbonVert[1], originRibbonVert[2], 1.0);
				changePoint = ribbonTranslate * ribbonCoordComvert * changePoint;
					
				memcpy(nowRibbonVertices + index, &changePoint[0], sizeof(float) * 3);
			}

			//2���_���𔯈�{�̍��W������z��ɂ܂Ƃ߂�
			for (int i = 0, index = 0; i < 2; i++, index += 3) {//�e�ʂ��\�����钸�_����͂���B
				memcpy(ribbonVert + segmentFace * 2 * 3 + index, &nowRibbonVertices[i * 3], sizeof(float) * 3);
			}

			prevRibbonApper = comvertedRibbonApper;
		}//�e2���_�Ɋւ���--------------------------------------------------------------------------------------------

		memcpy(&meshedHair[meshedDataNum_onehair * nowHair], ribbonVert, sizeof(float) * meshedDataNum_onehair);

		//index�̐ݒ�
		int nowHairStart = meshedIndexNum_onehair * nowHair;
		int nowHairIndexOffset = meshedVertexNum_onehair * nowHair;
		for (int i = 0; i < meshedIndexNum_onehair; i++) {
			meshedHairIndex[nowHairStart + i] = singleHairIndex[i] + nowHairIndexOffset;
		}

        //tangent vector
		//����{���̃^���W�F���g������
		
		float tangentVec[partitions+1][3];
		for (int row = 0; row < partitions+1; row++) {
			glm::vec3 tangent;
            int nwRArNum = row * 3;
            int nxRArNum = (row + 1) * 3;
			
            if (row == partitions) {
                memcpy(tangentVec[row], tangentVec[partitions - 1], sizeof(float) * 3);
            } else {
		        glm::vec3 p1, p2;
		        memcpy(&p1[0], &pickedPointsArray[nwRArNum], sizeof(float) * 3);
		        memcpy(&p2[0], &pickedPointsArray[nxRArNum], sizeof(float) * 3);

		        tangent = glm::normalize(p2 - p1);

                memcpy(tangentVec[row], &tangent[0], sizeof(float) * 3);
            }
		}

		

		//meshedHairTangent�ɑ΂��āA�f�[�^�����Ă���
		nowHairStart = meshedDataNum_onehair * nowHair;
		for (int row = 0; row < partitions + 1; row++) {
			int nowPartStart = row * 2;
			memcpy(&meshedHairTangent[nowHairStart + nowPartStart * 3]		, tangentVec[row], sizeof(float) * 3);
			memcpy(&meshedHairTangent[nowHairStart + (nowPartStart + 1) * 3]	, tangentVec[row], sizeof(float) * 3);
		}
		//-------------------------------------------------------------------------------------------------------------------

		
        delete[] pickedPointsArray;
		delete[] ribbonVert;

		if (segmentsArray) {
			nowHairStartArrayNum += segmentsArray[nowHair] + 1;
		} else {
			nowHairStartArrayNum += defaultSegs + 1;
		}
	}//����{��{�ɂ���-------------------------------------------------------------------------------------------
      

	pdataSet->setIndex(meshedIndexNum, meshedHairIndex);//index�z��̃R�s�[����

	float * meshedHairExpandDirection = new float[meshedVertexNum];
	for (int i = 0; i < meshedVertexNum; i++) {
		if (i % 2 == 0) {
			meshedHairExpandDirection[i] = -1.0;
		} else if (i % 2 == 1) {
			meshedHairExpandDirection[i] = 1.0;
		}
	}

	//�o�E���f�B���O�{�b�N�X����邽�߂̍��W��ۑ��B
	float min[3], max[3];
	for (int i = 0; i < 3; i++) {
		memset(&min[i], FLT_MAX, sizeof(float));
		memset(&max[i], FLT_MIN, sizeof(float));
	}

	//�o�E���f�B���O�{�b�N�X�̌v�Z�p���W���擾���Ă���
	for (int i = 0; i < meshedVertexNum; i++) {
		for (int j = 0; j < 3; j++) {
			if (meshedHair[i*3+j] > max[j]) max[j] = meshedHair[i*3+j];
			if (meshedHair[i*3+j] < min[j]) min[j] = meshedHair[i*3+j];
		}
	}
	pdataSet->setBoundingPos(min, max);


	//create vertex buffer object
	GLuint vboHandle[5];
	glGenBuffers(5, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];
	GLuint expandDirectionHandle = vboHandle[4];


	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHair, GL_STATIC_DRAW);

	//�@���o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairNormal, GL_STATIC_DRAW);
	
	//texture�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), NULL, GL_STATIC_DRAW);

	//tangent�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairTangent, GL_STATIC_DRAW);

	//expandDirection�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, expandDirectionHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedVertexNum * sizeof(float), meshedHairExpandDirection, GL_STATIC_DRAW);
	
	delete[] meshedHair;
	delete[] meshedHairNormal;
	delete[] meshedHairTangent;
	delete[] meshedHairExpandDirection;
    //delete[] testColor;

	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X1�͖@���o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    
	//�C���f�b�N�X2�̓e�N�X�`�����W�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    /*
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	*/
    
    //�C���f�b�N�X3��tangent�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X4��expand direction�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, expandDirectionHandle);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);



	return vaoHandle;
    
}

GLuint ikdObjects::ikdLoad_fromMyHairFile(char* fileName, ikdDataSet* pdataSet)
{
	char buf[100];
	FILE* fp = fopen(fileName, "r"); 
	if (fp == NULL) {
		printf("file open error.\n");
		exit(1);
	}
	int meshedDataNum;
	int vindex = 0, nindex = 0;
	
	fscanf(fp, "%d", &meshedDataNum);
	float *meshedHair = new float[meshedDataNum];
	float *meshedHairNormal = new float[meshedDataNum];
	while (fgets(buf, 50, fp) != NULL) {
		switch(buf[0]) {
		case 'v':
			sscanf(buf, "v %f %f %f", &meshedHair[vindex], &meshedHair[vindex+1], &meshedHair[vindex+2]);
			vindex += 3;
			break;
		case 'n':
			sscanf(buf, "n %f %f %f", &meshedHairNormal[nindex], &meshedHairNormal[nindex+1], &meshedHairNormal[nindex+2]);
			nindex += 3;
			break;
		default:
			
			break;
		}
	}
	
	fclose(fp);


	float min[3], max[3];//�o�E���f�B���O�{�b�N�X����邽�߂̍��W��ۑ��B
	for (int i = 0; i < 3; i++) {
		memset(&min[i], FLT_MAX, sizeof(float));
		memset(&max[i], FLT_MIN, sizeof(float));
	}

	
	int meshedVertexNum = meshedDataNum / 3;
	//�o�E���f�B���O�{�b�N�X�̌v�Z�p���W���擾���Ă���
	for (int i = 0; i < meshedVertexNum; i++) {
		for (int j = 0; j < 3; j++) {
			if (meshedHair[i*3+j] > max[j]) max[j] = meshedHair[i*3+j];
			if (meshedHair[i*3+j] < min[j]) min[j] = meshedHair[i*3+j];
		}
	}
	pdataSet->setBoundingPos(min, max);


	//create vertex buffer object
	GLuint vboHandle[2];
	glGenBuffers(2, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];



	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHair, GL_STATIC_DRAW);

	//�@���o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairNormal, GL_STATIC_DRAW);

	delete[] meshedHair;
	delete[] meshedHairNormal;
	
	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X1�͖@���o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdOSMLastPosition(ikdDataSet* pDataSet)
{
	//opacity map�̍ŏI�n�_�����[�U�����I�ɐݒ�ł���悤�A���ʂ�\������

	float planeVert[3*3*2];
	float planeTangent[2*3*3] = {
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0
	};
	float planeMoveVec[2*3] = {
		0, 1, 3,
		3, 1, 2
	};


	float startPos[] = {0.0, 0.0, 0.0};//�����_�͂Ƃ肠�������ׂ�0�Ƃ���
	for (int i = 0, index = 0; i < 2*3; i++, index += 3) {
		memcpy(planeVert+index, startPos, sizeof(float)*3);
	}	

	//create vertex buffer object
	GLuint vboHandle[5];
	glGenBuffers(5, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];
	GLuint expandDirectionHandle = vboHandle[4];


	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 3 * sizeof(float), planeVert, GL_STATIC_DRAW);

	//�@���o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
	
	//texture�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);

	//tangent�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 3 * sizeof(float), planeTangent, GL_STATIC_DRAW);

	//expandDirection�o�b�t�@�Ƀf�[�^�𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, expandDirectionHandle);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(float), planeMoveVec, GL_STATIC_DRAW);
	
	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X1�͖@���o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    
	//�C���f�b�N�X2�̓e�N�X�`�����W�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    
    //�C���f�b�N�X3��tangent�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//�C���f�b�N�X4��expand direction�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, expandDirectionHandle);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);



	return vaoHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_ArrayDepth(ikdDataSet* pDataSet,const int textureNum)
{
	GLfloat border[] = {1.0f, 0.0f, 0.0f, 1.0f};

	GLuint depthTex = 0;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, depthTex);//2�����e�N�X�`����z��Ƃ��Ē�`
  
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�p�����[�^
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), textureNum, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);//�e�N�X�`���̖{��
	
	pDataSet->setTexHandle(depthTex);

	//�t���[���o�b�t�@�I�u�W�F�N�g�쐬
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//�t���[���o�b�t�@�I�u�W�F�N�g�𐶐�
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo�L����

	GLenum drawBuffers[] = {GL_DEPTH_ATTACHMENT};
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//���ʂ̃t���[���o�b�t�@�ɖ߂�
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return fboHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_ArrayColor(ikdDataSet* pDataSet,const int textureNum)
{
	GLfloat border[] = {1.0f, 1.0f, 1.0f, 1.0f};

	GLuint colorTex = 0;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, colorTex);//2�����e�N�X�`����z��Ƃ��Ē�`
  
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�p�����[�^
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32F, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 
    textureNum, 0, GL_RGBA, GL_FLOAT, NULL);//�e�N�X�`���̖{��

  pDataSet->setTexHandle(colorTex);

	//�t���[���o�b�t�@�I�u�W�F�N�g�쐬
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//�t���[���o�b�t�@�I�u�W�F�N�g�𐶐�
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo�L����

	
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight());

  
	//�[�x�o�b�t�@��FBO�Ƀo�C���h����
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);
     

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//���ʂ̃t���[���o�b�t�@�ɖ߂�
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return fboHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_SingleDepth(ikdDataSet* pDataSet)
{

	GLfloat border[]={1.0f, 0.0f, 0.0f, 0.0f};

	//�e�N�X�`�����쐬
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);//�e�N�X�`���̖{��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�p�����[�^
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
 
	//�g���e�N�X�`���̔ԍ�
	//glActiveTexture(GL_TEXTURE0+activeTextureNum);
	//glActiveTexture(GL_TEXTURE10);
	//glBindTexture(GL_TEXTURE_2D, depthTex);

	//�t���[���o�b�t�@�I�u�W�F�N�g�쐬
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//�t���[���o�b�t�@�I�u�W�F�N�g�𐶐�
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo�L����

	//fbo�Ƀe�N�X�`�����o�C���h
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTex, 0);

	//GLenum drawBuffers[] = {GL_NONE};
	//glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//���ʂ̃t���[���o�b�t�@�ɖ߂�
	glBindTexture(GL_TEXTURE_2D, 0);


	pDataSet->setTexHandle(depthTex);

	return fboHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_SingleDepth_NoCompare(ikdDataSet* pDataSet)
{

	GLfloat border[]={1.0f, 0.0f, 0.0f, 0.0f};

	//�e�N�X�`�����쐬
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);//�e�N�X�`���̖{��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�p�����[�^
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//�t���[���o�b�t�@�I�u�W�F�N�g�쐬
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//�t���[���o�b�t�@�I�u�W�F�N�g�𐶐�
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo�L����

	//fbo�Ƀe�N�X�`�����o�C���h
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//���ʂ̃t���[���o�b�t�@�ɖ߂�
	glBindTexture(GL_TEXTURE_2D, 0);


	pDataSet->setTexHandle(depthTex);

	return fboHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_SingleColor(ikdDataSet *pDataSet)
{

	GLfloat border[]={1.0f, 1.0f, 1.0f, 1.0f};

	//�e�N�X�`�����쐬
	GLuint colorTex;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 0,
		GL_RGBA, GL_FLOAT, NULL);//�e�N�X�`���̖{��

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�p�����[�^
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//�t���[���o�b�t�@�I�u�W�F�N�g�쐬
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//�t���[���o�b�t�@�I�u�W�F�N�g�𐶐�
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo�L����

	//fbo�Ƀe�N�X�`�����o�C���h
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, colorTex, 0);


	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight());

	//�[�x�o�b�t�@��FBO�Ƀo�C���h����
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);

	pDataSet->setTexHandle(colorTex);

	/*
	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//���ʂ̃t���[���o�b�t�@�ɖ߂�

	glBindTexture(GL_TEXTURE_2D, 0);

	

	return fboHandle;
}

void ikdObjects::ikdChangeFramebufferTexture_SingleColor(int newWidth, int newHeight, ikdDataSet* pDataSet, 
		int deleteTexNum, int setFboNum)
{
	
	//�Â��e�N�X�`�����폜
	pDataSet->deleteTexture(deleteTexNum);


	//�V�����e�N�X�`�����쐬
	GLuint colorTex;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newWidth, newHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);//�e�N�X�`���̖{��

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�p�����[�^
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//�����̃t���[���o�b�t�@�I�u�W�F�N�g�Ăяo��
	GLuint fboHandle = pDataSet->getFboHandle(setFboNum);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo�L����

	//fbo�Ƀe�N�X�`�����o�C���h
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, colorTex, 0);


	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			newWidth, newHeight);

	//�[�x�o�b�t�@��FBO�Ƀo�C���h����
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);

	pDataSet->setTexHandle(colorTex);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//���ʂ̃t���[���o�b�t�@�ɖ߂�

	glBindTexture(GL_TEXTURE_2D, 0);

}

GLuint ikdObjects::ikdMakeFramebufferTexture_SingleColor_calculatedShadow(ikdDataSet *pDataSet)
{
	//�ŏI�I�Ɍv�Z������̉e��ۑ����邽�߂̃e�N�X�`��
	//�ЂƂ܂����ʂ̃e�N�X�`���Ɠ����p�����[�^�Őݒ肵�Ă��邪, 
	//������x�i�񂾂�, �X�N���[����ɓ\��t����悤�ɂ���


	GLfloat border[]={1.0f, 1.0f, 1.0f, 1.0f};

	//�e�N�X�`�����쐬
	GLuint colorTex;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 0,
		GL_RGBA, GL_FLOAT, NULL);//�e�N�X�`���̖{��

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�p�����[�^
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//�t���[���o�b�t�@�I�u�W�F�N�g�쐬
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//�t���[���o�b�t�@�I�u�W�F�N�g�𐶐�
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo�L����

	//fbo�Ƀe�N�X�`�����o�C���h
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, colorTex, 0);


	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight());

	//�[�x�o�b�t�@��FBO�Ƀo�C���h����
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);

	pDataSet->setTexHandle(colorTex);

	/*
	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//���ʂ̃t���[���o�b�t�@�ɖ߂�

	glBindTexture(GL_TEXTURE_2D, 0);

	

	return fboHandle;
}

void ikdObjects::ikdMakeColorTexture(ikdDataSet* pDataSet, glm::vec4 color)
{
	//���n�e�N�X�`���̍쐬////////////////////////////////////////////////////////////////////////

	float colorArray[4];
	memcpy(colorArray, &color[0], sizeof(float)*4);

	GLuint texHandle;
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texHandle);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0,
		GL_RGBA, GL_FLOAT, colorArray);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�U�镑���̐ݒ�
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	pDataSet->setTexHandle(texHandle);

	glBindTexture(GL_TEXTURE_2D, 0);
}

//vaoHandle�ƈꏏ��, index��ۑ����Ă����悤�Ȕz���p�ӂ����ق��������B
GLuint ikdObjects::ikddrawelemetnstest(ikdDataSet* pdataSet)
{
	float boxVert[3*3*12];
	float boxNormal[3*3*12];
	float boxTexCoord[2*3*12];
	//���_�̎擾
	ikdGetVertexBox(boxVert, boxNormal, boxTexCoord);


	float vertices[8][3] = {-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f};

	//�T�C�Y�̐ݒ�
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			vertices[i][j] *= 100;
		}
	}

	//create vertex buffer object
	GLuint vboHandle[1];
	glGenBuffers(1, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];


	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

	
	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdBillBoald(ikdDataSet* pdataSet)	
{

	glm::vec3 p1 = glm::vec3(-0.5, 0.5, 0.0);
	glm::vec3 p2 = glm::vec3(0.5, -0.5, 0.0);

	float vertices[] = {
		  p1[0], p1[1], p1[2], 
		  p1[0], p1[1], p1[2], 
		  p2[0], p2[1], p2[2], 
		  p2[0], p2[1], p2[2], 
	};
	float normals[] = {
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
	};

	
	glm::vec3 tang = p1 - p2;
	tang = glm::normalize(tang);


	float tangents[] = {
		tang[0], tang[1], tang[2],
		tang[0], tang[1], tang[2],
		tang[0], tang[1], tang[2],
		tang[0], tang[1], tang[2]
	};

	float expand[] = {
		-1.0,
		 1.0,
		 1.0,
		-1.0
	};


	for (int i = 0; i < 12; i++) {
		vertices[i] *= 100;
	}

	//create vertex buffer object
	GLuint vboHandle[5];
	glGenBuffers(5, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];
	GLuint expandBufferHandle = vboHandle[4];

	//�ʒu�o�b�t�@�Ƀf�[�^������
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), tangents, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, expandBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), expand, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//���_�z��I�u�W�F�N�g�̍쐬�Ɛݒ�
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//���_�����z���L���ɂ���
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	

	//�C���f�b�N�X0�͈ʒu�o�b�t�@
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, expandBufferHandle);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;

}
