#include "readPixel.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//#include "opencv2/opencv.hpp"

ReadPixel::ReadPixel(void)
{
  count = 0;
}

void ReadPixel::saveImage( const unsigned int imageWidth, const unsigned int imageHeight )
{
  const unsigned int channelNum = 3; // RGB�Ȃ�3, RGBA�Ȃ�4
  void* dataBuffer = NULL;
  dataBuffer = ( GLubyte* )malloc( imageWidth * imageHeight * channelNum );

  // �ǂݎ��OpneGL�̃o�b�t�@���w�� GL_FRONT:�t�����g�o�b�t�@�@GL_BACK:�o�b�N�o�b�t�@
  glReadBuffer( GL_BACK );

  // OpenGL�ŉ�ʂɕ`�悳��Ă�����e���o�b�t�@�Ɋi�[
  glReadPixels(
    0,                 //�ǂݎ��̈�̍�������x���W
    0,                 //�ǂݎ��̈�̍�������y���W //0 or getCurrentWidth() - 1
    imageWidth,             //�ǂݎ��̈�̕�
    imageHeight,            //�ǂݎ��̈�̍���
    GL_BGR, //it means GL_BGR,           //�擾�������F���̌`��
    GL_UNSIGNED_BYTE,  //�ǂݎ�����f�[�^��ۑ�����z��̌^
    dataBuffer      //�r�b�g�}�b�v�̃s�N�Z���f�[�^�i���ۂɂ̓o�C�g�z��j�ւ̃|�C���^
    );

  char str[256] = {'\0'};
  sprintf_s(str, "%d", count);
  std::string sstr = str;
  GLubyte* p = static_cast<GLubyte*>( dataBuffer );
  /*std::string fname = "outputImage_" + sstr + ".jpg";
  IplImage* outImage = cvCreateImage( cvSize( imageWidth, imageHeight ), IPL_DEPTH_8U, 3 );

  for ( unsigned int j = 0; j < imageHeight; ++ j )
  {
    for ( unsigned int i = 0; i < imageWidth; ++i )
    {
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 0 ] = *p;
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 1 ] = *( p + 1 );
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 2 ] = *( p + 2 );
      p += 3;
    }
  }

  cvSaveImage( fname.c_str(), outImage );

  free( dataBuffer );
  cvReleaseImage(&outImage);
*/
  count = (count + 1) % 30;
}