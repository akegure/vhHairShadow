/* *****************************************************************************
 * �t�@�C����		�F loadbmp.h
 * �@�\				�F BMP�t�@�C���ǂݍ��ݗp�w�b�_
 * ----------------------------------------------------------------------------
 * �쐬�ҁ@�@		�F Arakin
 * ***************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "external_loadbmp.h"
#include "external_futil.h"

/*
 * BMP�t�@�C���̃��[�h
 */
int loadBMP(
	ImageData *lpImageData,
	char *name
)
{
	errno_t err;
	FILE *fp;
	int i,j;
	int bpp, width, height;
	int lineBits, lineSize, linePos;
	unsigned char *lpImg, *lpImgArea;
	int index;
	unsigned char bfType[2];
	unsigned int bfOffBits;
	BMPInfoHeader bmpInfo;
	RGBQuad palette[MAX_COLOR_PALETTE];

	if (lpImageData == NULL)
	{
		return -1;
	}

	err = fopen_s(&fp, name, "rb");
	if (err != 0)
	{
		return -1;
	}

	/* �t�@�C���T�C�Y */
	fseek(fp, 0, SEEK_END);
	if (ftell(fp) < (sizeof(BMPFileHeader) + sizeof(BMPInfoHeader)))
	{
		fclose(fp);
		return -1;
	}

	/* �t�@�C���^�C�v */
	fseek(fp, 0, SEEK_SET);
	fread(&bfType[0], 1, 2, fp);
	if (bfType[0] != 'B' || bfType[1] != 'M')
	{
		fclose(fp);
		return -1;
	}

	/* �摜�f�[�^�ւ̃I�t�Z�b�g */
	fseek(fp, 10, SEEK_SET);
	fread(&bfOffBits, 4, 1, fp);

	/* BMP���w�b�_�ǂݍ��� */
	fseek(fp, 14, SEEK_SET);
	fread(&bmpInfo, sizeof(BMPInfoHeader), 1, fp);
	if (bmpInfo.biCompression != 0 || bmpInfo.biSize != 40)
	{
		/* ���������O�X���k�͖��T�|�[�g */
		fclose(fp);
		return -1;
	}

	/* 1��f������̃r�b�g���A���A���� */
	bpp = bmpInfo.biBitCount;
	width = bmpInfo.biWidth;
	height = bmpInfo.biHeight;

	/* �p�b�h���܂߂����C���T�C�Y�i�o�C�g�j */
	lineBits = FUTL_UPALLIGN32(bpp * width);
	lineSize = lineBits / 8;

	/* RGBA�̈�m�� */
 	lpImgArea = (void *)malloc(sizeof(unsigned int) * width * height);
	if (lpImgArea == NULL)
	{
		fclose(fp);
		return -1;
	} 

	/* RGB�ǂݍ��� */
	lpImg = lpImgArea;
	switch(bpp)
	{
	case 8:	/* 8 bits/pixel */
		/* �J���[�}�b�v�ǂݍ��� */
		fseek(fp,54,SEEK_SET);
		fread(&palette[0], sizeof(RGBQuad), 256, fp);

#if OPENGL_IMAGE_COORDINATE != 0
		for (i = 0; i < height; i++)
#else
		for (i = height - 1; i >= 0; i--)
#endif
		{
			linePos = bfOffBits + lineSize * i;
			fseek(fp, linePos, SEEK_SET);
			for (j = 0; j < width; j++)
			{
				index = fgetc(fp);
				*lpImg++ = palette[index].rgbRed;
				*lpImg++ = palette[index].rgbGreen;
				*lpImg++ = palette[index].rgbBlue;
				*lpImg++ = 255;
			}
		}
		break;

	case 24: /* 24 bits/pixel */
#if OPENGL_IMAGE_COORDINATE != 0
		for (i = 0; i < height; i++)
#else
		for (i = height - 1; i >= 0; i--)
#endif
		{
			linePos = bfOffBits + lineSize * i;
			fseek(fp, linePos, SEEK_SET);	
			for (j = 0; j < width; j++)
			{
				*(lpImg + 2) = (unsigned char)fgetc(fp);	/* B */
				*(lpImg + 1) = (unsigned char)fgetc(fp);	/* G */
				*(lpImg + 0) = (unsigned char)fgetc(fp);	/* R */
				*(lpImg + 3) = 255;							/* A */
				lpImg += 4;
			}
		}
		break;

	case 32: /* 32 bits/pixel */
#if OPENGL_IMAGE_COORDINATE != 0
		for (i = 0; i < height; i++)
#else
		for (i = height - 1; i >= 0; i--)
#endif
		{
			linePos = bfOffBits + lineSize * i;
			fseek(fp, linePos, SEEK_SET);	
			for (j = 0; j < width; j++)
			{
				*(lpImg + 2) = (unsigned char)fgetc(fp);	/* B */
				*(lpImg + 1) = (unsigned char)fgetc(fp);	/* G */
				*(lpImg + 0) = (unsigned char)fgetc(fp);	/* R */
							   (unsigned char)fgetc(fp);	/* reserved */
				*(lpImg + 3) = 255;							/* A */
				lpImg += 4;
			}
		}
		break;

	default:
		free(lpImg);
		fclose(fp);
		return -1;
	}

	lpImageData->data = lpImgArea;
	lpImageData->width = width;
	lpImageData->height = height;

	fclose(fp);

	return 0;
}


int releaseBMP(
	ImageData *lpImageData
)
{
	if (lpImageData == NULL)
	{
		return -1;
	}
	else if (lpImageData->data != NULL)
	{
		free(lpImageData->data);
		lpImageData->data = NULL;
	}

	return 0;
}
