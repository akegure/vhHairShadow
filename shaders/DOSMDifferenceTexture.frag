#version 400
			  
in vec4 mvpPos;//���C�g����݂��A���ȓ��e�e�����p�̃x�N�g��

//�t���O�֘A
uniform float shadow_noshadow;
uniform float pcfFlag;
flat in float sendHairFlag;

//DOSM�֘A
uniform sampler2DArray depthTexture;
uniform sampler2DArray opacityTexture;
uniform float textureNum;

//for refer
in vec4 cameraMVP;

//����
in vec3 normal;

//ambient (for make shadow)
uniform vec3 materialAmbient;
uniform vec3 lightAmbient;

//�e�N�X�`���₢���킹�悤
uniform float textureWidth;
uniform float textureHeight;

int texWidth = int(textureWidth);
int texHeight = int(textureHeight);

out vec4 fragColor;

vec4 getGaussedValue(vec3 textureCoord) 
{
	//�擾����_�̃T�C�Y 8�ߖT��, �����܂ŊO��������Ă��邩�B
	int radius = 1;
	float nor = 0.0;

	//�ߖT�e�N�Z���̔�r���v
	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);


	vec2 nowTexCoord;
	nowTexCoord.x = textureCoord.x * texWidth;
	nowTexCoord.y = textureCoord.y * texHeight;

	
	if (nowTexCoord.x >= (texWidth - radius) || nowTexCoord.y >= (texHeight - radius) 
		|| nowTexCoord.x <= radius || nowTexCoord.y <= radius) {
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
	

	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			if (i == 0 && j == 0) continue;

			nowTexCoord.x = nowTexCoord.x + i;
			nowTexCoord.y = nowTexCoord.y + j;

			textureCoord.x = nowTexCoord.x / texWidth;
			textureCoord.y = nowTexCoord.y / texHeight;

			sum += texture(opacityTexture, textureCoord);
			nor += 1;
		}
	}

	nor = 1.0 / nor;
	
	
	return sum *= nor;
}

float deepOpacityMapShadow()
{

	const int mapNum = 4;

	vec4 nowPos = mvpPos / mvpPos.w;//���݂̒��ړ_�̈ʒu-1.0~1.0
	vec2 texPosXY = (nowPos.xy + 1) * 0.5;//0.0~1.0

	vec4 depthTexColor = texture(depthTexture, vec3(texPosXY, 0));//get near
		
	float c_near = depthTexColor.x;//-1.0 ~ 1.0

	int texturePos = 0;
	float allLayer = 2 * textureNum * (1 + 4 * textureNum);
	float c_base = (1.0 - c_near) / allLayer;//���C�����m�̍ŏ�����
	float textureFar = 0.0;
	for (int i = 0; i < textureNum; i++) {
		textureFar = c_near + c_base * 2 * (i + 1) * (1 + 4 * (i + 1));
		if (nowPos.z < textureFar) {
			texturePos = i;
			break;
		}
	}

	//get now opacity colors.
	//�F�̎擾
	float opacityValue[4] = float[4](0.0, 0.0, 0.0, 0.0);
	float beforeLastOpacityValue = 0.0;//��O�̃e�N�X�`���̍Ō�̒l���Ƃ�		

	vec4 opacityTexColor;
	if (pcfFlag == 0) {
		//single sample
		opacityTexColor = texture(opacityTexture, vec3(texPosXY, texturePos));//4��opacity map�l	
		opacityValue[0] = opacityTexColor.r;
		opacityValue[1] = opacityTexColor.g;
		opacityValue[2] = opacityTexColor.b;
		opacityValue[3] = opacityTexColor.a;

		//before opacity color 
		opacityTexColor = texture(opacityTexture, vec3(texPosXY, texturePos-1));
	} else {
		//gauss
		opacityTexColor = getGaussedValue(vec3(texPosXY, texturePos));//4��opacity map�l
		opacityValue[0] = opacityTexColor.r;
		opacityValue[1] = opacityTexColor.g;
		opacityValue[2] = opacityTexColor.b;
		opacityValue[3] = opacityTexColor.a;

		//before opaciyt color 
		opacityTexColor = getGaussedValue(vec3(texPosXY, texturePos-1));		
	}			

	if (texturePos != 0) {
		beforeLastOpacityValue = opacityTexColor.a;
	} 

	
	
	float layerDistance[mapNum];//���ڃe�N�X�`�����̎l�̒��ŁA�ǂ��̒l���Q�Ƃ��邩�����߂�
	float textureNear = 0;
	if (texturePos <= 0) {
		textureNear = c_near;
	} else {
		textureNear = c_near + 
			c_base * 2 * (texturePos) * (1 + 4 * (texturePos));
	}
	for (int i = 0; i < mapNum; i++) {
		if (i == 0) {
			layerDistance[i] = textureNear;
		} else {
			layerDistance[i] = layerDistance[i-1];
		}
		layerDistance[i] += texturePos * 4 * c_base + c_base * (i + 1);
	}

	//calc opacity value
	float resultValue = 0.0;
	for (int count = 0; count < mapNum; count++) {
		if (nowPos.z < layerDistance[count]) {
			float calcDepth = 0.0;
			if (count == 0) {
				float nowDepth = nowPos.z - textureNear;
				if (nowDepth < 0) { nowDepth = 0; }
				calcDepth = nowDepth / (layerDistance[count] - textureNear);
			} else {
				float nowDepth = nowPos.z - layerDistance[count - 1];
				if (nowDepth < 0) { nowDepth = 0; }
				calcDepth = nowDepth / (layerDistance[count] - layerDistance[count-1]);
			}

			float currentOpacity = 1.0;
			if (count == 0) {
				currentOpacity = (1.0 - calcDepth) * beforeLastOpacityValue + calcDepth * opacityValue[count];
			} else {
				currentOpacity = (1.0 - calcDepth) * opacityValue[count-1] + calcDepth * opacityValue[count];
			}

			if (currentOpacity > 1.0) {
				resultValue = 0.0;
				break;
			} else {
				resultValue = 1.0 - currentOpacity;
				break;
			}
		}
	}
	
	return resultValue;
}


float getLastOpacityValue(void)
{
	vec4 nowPos = mvpPos / mvpPos.w;
	vec2 texPosXY = (nowPos.xy + 1) * 0.5;
	vec3 textureCoord = vec3(texPosXY, int(textureNum - 1));//�Ō�̃e�N�X�`��
	
	vec4 texColor;
	if (pcfFlag == 0) {
		texColor = texture(opacityTexture, textureCoord);
	} else if (pcfFlag == 1) {
		texColor = getGaussedValue(textureCoord);
	}

	if (texColor.a > 1) {
		return 0.0;
	} else {
		return (1.0 - texColor.a); //�Ō�̃��C��
	} 
}

void main(void)
{



	vec4 nowPos = cameraMVP / cameraMVP.w;

	if (sendHairFlag == 1) {
		if (shadow_noshadow == 1.0) {
			float brightness = deepOpacityMapShadow();
			fragColor = vec4(brightness, 1.0, 1.0, 1.0);
		} else {
			fragColor = vec4(1.0, 1.0 ,1.0, 1.0);
		}
	} else if (sendHairFlag == 0.0) {
		if (shadow_noshadow == 1.0) {
			float brightness = getLastOpacityValue();
			fragColor = vec4(brightness, 1.0, 1.0, 1.0);
		} else {
			fragColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	}
}