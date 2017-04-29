#version 400

in vec4 ioCenterPos;
in vec4 ioPos;


uniform sampler2DArray uDepthTex;//�[�x�l�}�b�v
uniform float uOpacityValue; 

out vec4 fragColor;

void main()
{
	//�󂯎�����ʒu�𐳋K��
	vec4 nowPos = ioPos / ioPos.w;
	float z = nowPos.z;

  //���݈ʒu���璆�S�_�Ƃ̋������v�Z, �s�����x�𒲐�
  
  
  
  vec4 nowCenterPos = ioCenterPos / ioCenterPos.w;
  float centerlength = length(nowPos.xyz - nowCenterPos.xyz) * 8;

  float opacityValue = uOpacityValue * centerlength;
  

	//�[�x�l�擾
	vec4 texColor = texture(uDepthTex, vec3((nowPos.xy + 1.0) * 0.5, 0));
	float near = texColor.x;

	//���C����u���ʒu������
	float base = (1.0 - near) / 4;
	float layerDistance[4] = float[4](0.0, 0.0, 0.0, 0.0);//�����x�L�^�ʒu����4�ɌŒ�
    layerDistance[0] = near + base;
    layerDistance[1] = layerDistance[0] + base;
    layerDistance[2] = layerDistance[1] + base;
    layerDistance[3] = layerDistance[2] + base;

	//�l�̋L�^

	float opacityMap[4] = float[4](0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 4; i++) {
		if (z < layerDistance[i]) {
			opacityMap[i] = uOpacityValue;
     // opacityMap[i] = opacityValue;
		}
	}



	fragColor = vec4(opacityMap[0], opacityMap[1], opacityMap[2], opacityMap[3]);
	

}