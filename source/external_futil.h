/* *****************************************************************************
 * �t�@�C����		�F futil.h
 * �@�\				�F ���[�e�B���e�B�֐�
 * ----------------------------------------------------------------------------
 * �쐬�ҁ@�@		�F Arakin
 * ***************************************************************************** */
#ifndef __FUTIL_H
#define __FUTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"
//#include "glut.h"

/* �摜���W�n */
/* 1: �������_(OpenGL�d�l), 0:���㌴�_ */
#define OPENGL_IMAGE_COORDINATE		(1)

/* �V�t�g�ɂ��ώZ */
#define FUTL_MULT2(x)		((x) << 1)
#define FUTL_MULT4(x) 		((x) << 2)
#define FUTL_MULT8(x) 		((x) << 3)
#define FUTL_MULT16(x)		((x) << 4)
#define FUTL_MULT32(x)		((x) << 5)
#define FUTL_MULT64(x)		((x) << 6)
#define FUTL_MULT128(x)		((x) << 7)
#define FUTL_MULT256(x)		((x) << 8)
#define FUTL_NMULT(x, n)	((x) << (n))

/* �V�t�g�ɂ�鏜�Z */
#define FUTL_DIV2(x)		((x) >> 1)
#define FUTL_DIV4(x) 		((x) >> 2)
#define FUTL_DIV8(x) 		((x) >> 3)
#define FUTL_DIV16(x)		((x) >> 4)
#define FUTL_DIV32(x)		((x) >> 5)
#define FUTL_DIV64(x)		((x) >> 6)
#define FUTL_DIV128(x)		((x) >> 7)
#define FUTL_DIV256(x)		((x) >> 8)
#define FUTL_DIV512(x)		((x) >> 9)
#define FUTL_DIV1024(x)		((x) >> 10)
#define FUTL_DIV2048(x)		((x) >> 11)
#define FUTL_NDIV(x, n)		((x) >> (n))

/* �r�b�g��]�Z */
#define FUTL_MOD2(x)		((x) & 0x1)
#define FUTL_MOD4(x) 		((x) & 0x3)
#define FUTL_MOD8(x) 		((x) & 0x7)
#define FUTL_MOD16(x)		((x) & 0xF)
#define FUTL_MOD32(x)		((x) & 0x1F)
#define FUTL_MOD64(x)		((x) & 0x3F)
#define FUTL_MOD128(x)		((x) & 0x7F)
#define FUTL_MOD256(x)		((x) & 0xFF)
#define FUTL_MOD512(x)		((x) & 0x1FF)
#define FUTL_MOD1024(x)		((x) & 0x3FF)
#define FUTL_NMOD(x, n)		((x) & ((n) - 1))

#define FUTL_ALLIGN4(x)		((x) & (~0x3))
#define FUTL_ALLIGN32(x)	((x) & (~0x1F))

#define FUTL_UPALLIGN4(x)	FUTL_ALLIGN4((x) + 0x3)
#define FUTL_UPALLIGN32(x)	FUTL_ALLIGN32((x) + 0x1F)


/* �� */
#define FUTL_PI (3.141592654f)

/* DEGREE����RADIAN�ւ̕ϊ� */
#define FUTL_DEG_TO_RAD(degree)\
    	( (FUTL_PI/180.0f) * (degree) )
#define FUTL_RAD_TO_DEG(rad)\
		( (rad) * (180.0f/FUTL_PI) )

/* ************************************************************ */
/* ���_�̒�`													*/
/* ************************************************************ */
/* �Q�������_�A�x�N�g���̒�` */
typedef struct {
    float x;
    float y;
} FVector2D;

/* �R�������_�A�x�N�g���̒�` */
typedef struct {
    float x;
    float y;
    float z;
} FVector3D;

/* �S�������_�A�x�N�g���̒�` */
typedef struct {
    float x;
    float y;
    float z;
    float w;
} FVector4D;

/* ************************************************************ */
/* �}�g���b�N�X�̒�`											*/
/* ************************************************************ */
typedef struct {
    float m00;
	float m10;
	float m20;
	float m30;
	float m01;
	float m11;
	float m21;
	float m31;
	float m02;
	float m12;
	float m22;
	float m32;
	float m03;
	float m13;
	float m23;
	float m33;
} FMatrix4x4, FMatrix;

/* �N�H�[�^�j�I�� */
typedef struct {
	float w;	/* ������ */
	float x;	/* ������i */
	float y;	/* ������j */
	float z;	/* ������k */
} FQuat;

/* �J���[�^�C�v */
typedef struct {
	float r;
	float g;
	float b;
	float a;
} FColorRGBA;

/* ���� */
typedef struct _tmpFLight {
	/* ���s�����@�@�@�Fw  = 0.0f  x,y,z�ŕ��� */
	/* �_�����@�@�@�@�Fw != 0.0f  x,y,z,w�Ō����ʒu */
	/* �X�|�b�g���C�g�Fw != 0.0f  x,y,z,w�Ō����ʒu */
	FVector4D position;			/* �ʒu */

	FColorRGBA ambient;			/* �A���r�G���g */
	FColorRGBA diffuse;			/* �f�B�t���[�Y */
	FColorRGBA specular;		/* �X�y�L���� */

	FVector3D spotDirection;	/* �X�|�b�g���C�g���� */

	/* [0,128]�͈̔́F0.0f�̓J�b�g�I�t�͈͓̔��͈�l */
	float spotExponent;			/* ���C�g���� */

	/* [0,90]�͈̔́F180.0f�͓_���� */
	float spotCutoff;			/* �J�b�g�I�t */

	/* �����W���F���s�����ł͖��� */
	float attenuation0;			/* �����W�� */
	float attenuation1;			/* �����W�� */
	float attenuation2;			/* �����W�� */
} FLight;

/* �}�e���A�� */
typedef struct _tmpFMaterial {
	FColorRGBA ambient;			/* �A���r�G���g */
	FColorRGBA diffuse;			/* �f�B�t���[�Y */
	FColorRGBA specular;		/* �X�y�L���� */
	FColorRGBA emission;		/* �G�~�b�V���� */
	float shininess;			/* �X�y�L�����[���� */
} FMaterial;


/* ************************************************ */
/* �v���g�^�C�v�錾									*/
/* ************************************************ */


/* ************************************************ */
/* ���[�e�B���e�B�֐�								*/
/* ************************************************ */
/* GL�g���@�\�̏����� */
int FUTL_InitalizeGLExt(void);

/* ������̕`��(���㌴�_) */
void FUTL_DrawString(char *lpStr, int x, int y);

/* Perfomance Counter�Ԃ̎��� */
float FUTL_SubPCounter(LARGE_INTEGER *lpEndTime, LARGE_INTEGER *lpStartTime);

/* �e�N�X�`���̓ǂݍ��� */
int FUTL_LoadTexture(char *name, unsigned int *lpTexID);

/* �g�[���X���f���̍쐬�ƈړ� */
int FUTL_MakeTorus(char *texFile);
int FUTL_DrawTorus(int count);
int FUTL_DrawTorusVBO(int count);

/* �����ݒ� */
int FUTL_SetLight(int lightID, FLight *lpLight);

/* �}�e���A���ݒ� */
int FUTL_SetMaterial(FMaterial *lpMaterial);

/* �V�F�[�_�̓ǂݍ��� */
int FUTL_LoadShader(char *vtxShName, char *frgShName, GLuint *lpProg);



/* ************************************************ */
/* float�p�̎Z�p���Z								*/
/* ************************************************ */
/* ������ */
/* ���s���t�� y = 1.0f / (x)^0.5 */
float FUTL_InvSqrt(float x);
float FUTL_Sqrt(float x);
                     
/* �O�p�֐�	*/
float FUTL_Sin(float rad);
float FUTL_Cos(float rad);
float FUTL_Tan(float rad);
float FUTL_Atan(float x);
float FUTL_Atan2(float y, float x);

/* ���̑��̎Z�p�֐�	*/
float FUTL_Fabs(float x);
float FUTL_Pow(float a, float b);
float FUTL_Floor(float x);
                       
/* �x�N�g���̐��K���i���K���O�̒�����Ԃ��j */
float FUTL_VecNormalize(FVector3D *lpV);
float FUTL_Vec2DNormalize(FVector2D *lpV);

/* ************************************************ */
/* float�p�̍s�񉉎Z								*/
/* ************************************************ */
/* ���_�̍��W�ϊ� */
void FUTL_MatVector(FVector3D *lpS, FMatrix *lpM, FVector3D *lpV);

/* �����x�N�g���̍��W�ϊ� */
void FUTL_MatDirection(FVector3D *lpSDir, FMatrix *lpM, FVector3D *lpDir);

/* �P�ʍs�� */
void FUTL_MatIdentity(FMatrix *lpM);

/* �s��R�s�[ */
void FUTL_MatCopy(FMatrix *lpMa, FMatrix *lpMb);

/* ���s�ړ� */
void FUTL_MatTranslate(FMatrix *lpM, float x, float y, float z);
void FUTL_MatMove(FMatrix *lpM, float x, float y, float z);

/* �O���[�o����] */
void FUTL_MatRotateX(FMatrix *lpM, float angle);
void FUTL_MatRotateY(FMatrix *lpM, float angle);
void FUTL_MatRotateZ(FMatrix *lpM, float angle);

/* ���[�J����] */
void FUTL_MatTurnX(FMatrix *lpM, float angle);
void FUTL_MatTurnY(FMatrix *lpM, float angle);
void FUTL_MatTurnZ(FMatrix *lpM, float angle);
      
/* �X�P�[�����O */
void FUTL_MatScale(FMatrix *lpM,
			   float scalex, float scaley, float scalez);
void FUTL_MatSize(FMatrix *lpM,
			   float scalex, float scaley, float scalez);

/*
 * �N�H�[�^�j�I��
 */
/* �N�H�[�^�j�I���̐� */
void FUTL_QuatMult(FQuat *lpR, FQuat *lpP, FQuat *lpQ);

/* �N�H�[�^�j�I�����s��ɕϊ� */
void FUTL_QuatToMatrix(FMatrix *lpM, FQuat *lpQ);

/* ��]�N�H�[�^�j�I���̍쐬 */
void FUTL_QuatRotation(FQuat *lpQ, float rad, float ax, float ay, float az);

/* �P�ʃN�H�[�^�j�I���̍쐬 */
void FUTL_QuatIdentity(FQuat *lpQ);

/* �N�H�[�^�j�I���̃R�s�[ */
void FUTL_QuatCopy(FQuat *lpTo, FQuat *lpFrom);

#ifdef __cplusplus
};
#endif

#endif	/* __RGFLIB_H */
