/* *****************************************************************************
 * �t�@�C����		�F futil.c
 * �@�\				�F ���[�e�B���e�B�֐�
 * ----------------------------------------------------------------------------
 * �쐬�ҁ@�@		�F Arakin
 * ***************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "GL/glew.h"
#include "GL/wglew.h"
//#include "glut.h"
#include "GL/freeglut.h"

#include "external_loadbmp.h"
#include "external_futil.h"


static int loadShader(GLuint shader, char *file);
static void printShaderInfoLog(GLuint shader);
static void printProgramInfoLog(GLuint program);

/* �V�F�[�_�R���p�C���̃��O�o�b�t�@ */
#define MAX_SHADER_LOG_SIZE		(1024)
char s_logBuffer[MAX_SHADER_LOG_SIZE]; 

/* glutBitmapCharacter�p�t�H���g */
#ifdef F9X15
#define FUTL_FONT_TYPE		GLUT_BITMAP_9_BY_15
#define FUTL_FONT_HEIGHT	(15)
#else
#define FUTL_FONT_TYPE		GLUT_BITMAP_8_BY_13
#define FUTL_FONT_HEIGHT	(13)
#endif


/*
 * �e�N�X�`���̓ǂݍ���
 */
int FUTL_LoadTexture(
	char *name,
	unsigned int *lpTexID
)
{
	ImageData img;
	unsigned int texID;

	memset(&img, 0, sizeof(img));
	if (loadBMP(&img, name) < 0)
	{
		fprintf(stderr, "%s is not found!!\n", name);
		return -1;
	}

	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* �e�N�X�`���摜�͂S�o�C�g�A���C�����g */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
		img.width, img.height, 0, GL_RGBA, 
		GL_UNSIGNED_INT_8_8_8_8_REV,	/* GL_UNSIGNED_SHORT_1_5_5_5_REV, */
		img.data);

	*lpTexID = texID;

	releaseBMP(&img);

	return 0;
}


/*
 * �����̕`��
 */
void FUTL_DrawString(
	char *str,					/* ������i�I�['\0'�j */
	int x,						/* �����̕`��ʒu */
	int y						/* �����̕`��ʒu */
)
{
	int wy;
	int width, height;

	width = glutGet((GLenum)GLUT_WINDOW_WIDTH);
	height = glutGet((GLenum)GLUT_WINDOW_HEIGHT);

	/* �Œ�p�C�v���C���ɖ߂� */
	glUseProgram(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0f, (float)width, 0.0f, (float)height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

#if OPENGL_IMAGE_COORDINATE
	wy = y;
#else
	wy = height - y - FUTL_FONT_HEIGHT;
#endif
	glRasterPos2i(x, wy);

	for (; *str != 0; str++)
	{
		if (*str == '\n')
		{
			wy -= FUTL_FONT_HEIGHT;
			glRasterPos2i( x, wy );
			continue;
		}

		glutBitmapCharacter(FUTL_FONT_TYPE, *str);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


/*
 * ���Ԍv��
 */
float FUTL_SubPCounter(
	LARGE_INTEGER *lpEndTime,
	LARGE_INTEGER *lpStartTime
)
{
	LARGE_INTEGER ticksPerSec;
	double qpf;
	float ftime;

	QueryPerformanceFrequency(&ticksPerSec);
	qpf = (double)ticksPerSec.QuadPart;

	ftime = (float)((lpEndTime->QuadPart - lpStartTime->QuadPart) / qpf);

	return ftime;
}



/* �����ݒ� */
int FUTL_SetLight(
	int lightID,
	FLight *lpLight
)
{
	glLightfv(lightID, GL_AMBIENT, (GLfloat *)(&(lpLight->ambient)));
	glLightfv(lightID, GL_DIFFUSE, (GLfloat *)(&(lpLight->diffuse)));
	glLightfv(lightID, GL_SPECULAR, (GLfloat *)(&(lpLight->specular)));
	glLightfv(lightID, GL_POSITION, (GLfloat *)(&(lpLight->position)));

	/* ���s�����̏ꍇ�A���L�p�����[�^�͖��� */
	if (lpLight->position.w == 0.0f)
		return 0;

	glLightfv(lightID, GL_SPOT_DIRECTION, (GLfloat *)(&(lpLight->spotDirection)));
	glLightfv(lightID, GL_SPOT_EXPONENT, &(lpLight->spotExponent));
	glLightfv(lightID, GL_SPOT_CUTOFF, &(lpLight->spotCutoff));
	
	glLightfv(lightID, GL_CONSTANT_ATTENUATION, &(lpLight->attenuation0));
	glLightfv(lightID, GL_LINEAR_ATTENUATION, &(lpLight->attenuation1));
	glLightfv(lightID, GL_QUADRATIC_ATTENUATION, &(lpLight->attenuation2));

	return 0;
}

/* �}�e���A���ݒ� */
int FUTL_SetMaterial(
	FMaterial *lpMaterial
)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)(&(lpMaterial->ambient)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)(&(lpMaterial->diffuse)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)(&(lpMaterial->specular)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat *)(&(lpMaterial->emission)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat *)(&(lpMaterial->shininess)));

	return 0;
}


/*
 * �V�F�[�_�v���O�����̃��[�h
 */
int FUTL_LoadShader(
	char *vtxShdName,
	char *frgShdName,
	GLuint *lpProg
)
{
	GLuint vtxShader;
	GLuint frgShader;
	GLuint prog;
	GLint linked;

	/* �V�F�[�_�I�u�W�F�N�g�̍쐬 */
	vtxShader = glCreateShader(GL_VERTEX_SHADER);
	frgShader = glCreateShader(GL_FRAGMENT_SHADER);

	/* �o�[�e�b�N�X�V�F�[�_�̃��[�h�ƃR���p�C�� */
	if (loadShader(vtxShader, vtxShdName) < 0)
	{
		return -1;
	}

	/* �t���O�����g�V�F�[�_�̃��[�h�ƃR���p�C�� */
	if (loadShader(frgShader, frgShdName) < 0)
	{
		return -1;
	}

	/* �v���O�����I�u�W�F�N�g�̍쐬 */
	prog = glCreateProgram();

	/* �V�F�[�_�I�u�W�F�N�g�̃V�F�[�_�v���O�����ւ̓o�^ */
	glAttachShader(prog, vtxShader);
	glAttachShader(prog, frgShader);

	/* �V�F�[�_�I�u�W�F�N�g�̍폜 */
	glDeleteShader(vtxShader);
	glDeleteShader(frgShader);

	/* �V�F�[�_�v���O�����̃����N */
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	printProgramInfoLog(prog);
	if (linked == GL_FALSE)
	{
		fprintf(stderr, "Link error of %s & %s!!\n", vtxShdName, frgShdName);
		return -1;
	}

	*lpProg = prog;

	return 0;
}


/*
 * �V�F�[�_�[�v���O���������[�h���A�R���p�C��
 */
static int loadShader(
	GLuint shader, 
	char *name
)
{
	errno_t err;
	FILE *fp;
	void *buf;
	int size;
	GLint compiled;

	if ((err = fopen_s(&fp, name, "rb")) != 0)
	{
		fprintf(stderr, "%s is not found!!\n", name);
		return -1;
	}
  
	/* �t�@�C���T�C�Y�̎擾 */
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	/* �V�F�[�_�v���O�����̓ǂݍ��ݗ̈���m�� */
	if ((buf = (void *)malloc(size)) == NULL)
	{
		fprintf(stderr, "Memory is not enough for %s\n", name);
		fclose(fp);
		return -1;
	}
  
	/* �t�@�C����擪����ǂݍ��� */
	fseek(fp, 0, SEEK_SET);
	fread(buf, 1, size, fp);

	/* �V�F�[�_�I�u�W�F�N�g�Ƀv���O�������Z�b�g */
	glShaderSource(shader, 1, (GLchar **)&buf, &size);
  
	/* �V�F�[�_�ǂݍ��ݗ̈�̉�� */
	free(buf);
	fclose(fp);

	/* �V�F�[�_�̃R���p�C�� */
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(shader);		/* �R���p�C�����O�̏o�� */
	if (compiled == GL_FALSE)
	{
		fprintf(stderr, "Compile error in %s!!\n", name);
		return -1;
	}

	return 0;
}

/* �V�F�[�_�R���p�C���G���[�̏o�� */
static void printShaderInfoLog(
	GLuint shader
)
{
	int logSize;
	int length;

	/* ���O�̒����́A�Ō��NULL�������܂� */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		fprintf(stderr, "Shader Info Log\n%s\n", s_logBuffer);
	}
}


/* �V�F�[�_�����N�G���[�̏o�� */
static void printProgramInfoLog(
	GLuint program
)
{
	int logSize;
	int length;

	/* ���O�̒����́A�Ō��NULL�������܂� */
	glGetProgramiv(program, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetProgramInfoLog(program, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		fprintf(stderr, "Program Info Log\n%s\n", s_logBuffer);
	}
}
