#include "AkgMotion.h"

#include "AkgMouse.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void AkgMotion::Exec(AkgGeneralManager* obj, int x, int y)
{
/*�݌v���j
  �݌v���ʂƂ��āC�}�E�X�ŁC���ڂ����Ƃ���ɃJ��������������
  �ύX���镨
    �J�����̌���ꏊ
  �K�v�Ȍv�Z�ޗ�
    �N���b�N�����ʒu
    �N���b�N�����ʒu����̕ω���
    �������ʒu
*/
  float look[3];
  obj->GetCamera(NULL, look, NULL, NULL); 

  int* motionStart = obj->GetMotionStart();

  look[0] =  (float)-x + motionStart[0];
  look[1] = (float)-y + motionStart[1]; 

  //std::cout << look[0] << " " << look[1] << std::endl;

  obj->SetCamera(NULL, look, NULL, NULL);

  glutPostRedisplay();
}