/* *****************************************************************************
 * �t�@�C����		�F loadbmp.h
 * �@�\				�F BMP�t�@�C���ǂݍ��ݗp�w�b�_
 * ----------------------------------------------------------------------------
 * �쐬�ҁ@�@		�F Arakin
 * ***************************************************************************** */
#ifndef	_LOADBMP_H_
#define	_LOADBMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/* �t�@�C���w�b�_ */
typedef struct _tmpBMPFileHeader {
	unsigned short bfType;			/* �t�@�C���^�C�v */
	unsigned int bfSize;			/* �t�@�C���T�C�Y */
	unsigned short bfReserved1;		/* �\��̈� */
	unsigned short bfReserved2;		/* �\��̈� */
	unsigned int bfOffBits;			/* �t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g */
} BMPFileHeader;

/* BMP���w�b�_ */
typedef struct _tmpBMPInfoHeader {
	unsigned int biSize;			/* ���w�b�_�[�̃T�C�Y */
	int biWidth;					/* �� */
	int biHeight;					/* ����(���Ȃ�Ή������A���Ȃ�Ώォ�牺) */
	unsigned short biPlanes;		/* �v���[����(���1) */
	unsigned short biBitCount;		/* 1��f������̃r�b�g��(1,4,8,24,32) */
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;			/* �p���b�g�̐F�� */
	unsigned int biClrImportant;
} BMPInfoHeader;

/* �p���b�g�f�[�^ */
typedef struct _tmpRGBQuad {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQuad;


/*
 * �ǂݍ��� BMP File ���i�[�����
 */
typedef struct _tmpImageData {
	unsigned int format;	/* �摜�̃t�H�[�}�b�g */
	int  width, height;		/* �摜�̑傫�� */
	void *data;				/* �摜�f�[�^ */
} ImageData;

#define MAX_COLOR_PALETTE		(256)

int	loadBMP(ImageData *pp, char *name);
int	releaseBMP(ImageData *pp);

#ifdef __cplusplus
}
#endif

#endif	/* _DOC_FILE_ */
