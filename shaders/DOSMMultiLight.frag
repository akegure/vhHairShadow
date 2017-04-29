#version 400
			  
uniform float multiLightFlag_frag;
flat in float sendLightCount;
in vec4 mvpPos[10];
uniform vec3 lightPosition[10];
in vec2 texCoord;

//�t���O�֘A
uniform float shadow_noshadow;
uniform float pcfFlag;
flat in float sendHairFlag;

//DOSM�֘A
uniform float textureNum;//�ЂƂ̃I�p�V�e�B�}�b�v�����艽���̃e�N�X�`����ǂݍ��ނ̂�
uniform sampler2DArray depthTexture;
uniform sampler2DArray opacityTexture;
uniform sampler2DArray multiLightMap;
uniform float layerInterval[10];
uniform float textureInterval[10];
	  


//����
in vec3 normal;


//kajiya-kay�֘A
in vec3 mvPos;
in vec3 globalPosition;
in vec3 binormal;
in vec3 tangent;

uniform float shininessOne;
uniform float shininessTwo;
uniform float specularWeightOne;
uniform float specularWeightTwo;
uniform vec3 diffuseColor;
uniform float diffuseWeight;
uniform vec3 specularColorOne;
uniform vec3 specularColorTwo;

//phong�V�F�[�f�B���O
in vec3 localPosition;
in mat4 mMatrix;
flat in vec3 sendCameraPosition;
uniform vec3 lightDiffuseColor;
uniform vec3 materialAmbient;
uniform vec3 lightAmbient;
uniform vec3 materialShininess;
uniform vec3 lightShininess;
uniform float shininess;




//�e�N�X�`���₢���킹�悤
uniform float textureWidth;
uniform float textureHeight;

int texWidth = int(textureWidth);
int texHeight = int(textureHeight);

out vec4 fragColor;

float KajiyaKayDiffuse( vec3 hairTangent, vec3 lightVector)
{
	float df = dot( hairTangent,normalize( lightVector ) );
	df = 1.0 - (df * df);
	if ( df < 0 ) df = 0;
	if ( df > 0 ) df = sqrt( df );
	return df;
}

float KajiyaKaySpecular( vec3 hairTangent, vec3 lightVector, vec3 viewVector)
{
	float kspec;
	float sintl;
	float sinte;
	float  vt;

	sintl = KajiyaKayDiffuse( hairTangent, lightVector);

	vt =  dot(viewVector, hairTangent);
	sinte = 1.0 - (vt * vt);
	if ( sinte < 0 ) sinte = 0;
	if ( sinte > 0 ) sinte = sqrt( sinte );

	// equl on paper : (t.l)(t.e) + sin(t,l)sin(t.e)
	kspec = sintl * sinte - dot( normalize(lightVector), hairTangent ) * vt;

	if ( kspec < 0 ) kspec = 0;
	return kspec;
}
vec3 lightingKajiyaKay(int light)
{
	//�J�������W�Ōv�Z���s���Ă���
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightDir = normalize(lightPosition[light] - globalPosition);
	vec3 viewDir =  normalize(sendCameraPosition - globalPosition);

	vec3 n = normalize(normal);
  	vec3 t = normalize(tangent);

	float s1 = max(0.01, shininessOne);
	float s2 = max(0.01, shininessTwo);
	float g1 = specularWeightOne;
	float g2 = specularWeightTwo;
	
	vec3 DiffuseColor   = diffuseColor;
	vec3 specular1 = specularColorOne;
	vec3 SecondarySpecularColor = specularColorTwo;

	float DiffuseWeight = diffuseWeight;  	  	

	float HalfLambert = max(dot(lightDir, n), 0)*0.5+0.5;

  	float d = max(0.0,dot(n, normalize(lightDir)));
  	float h = min(1.0,max(0.0,HalfLambert));
  	
  	float dd = d*(1.0-h) + h;
  	float ds = d;  	
  	
  	vec3 diff  = DiffuseWeight*DiffuseColor*dd*KajiyaKayDiffuse(t, -lightDir);
  	float  ss  = KajiyaKaySpecular(t, -lightDir, -viewDir);
	vec3 spec1 = specular1*ds*pow(ss,s1)*g1;
	vec3 spec2 = SecondarySpecularColor*ds*pow (ss,s2)*g2;

	vec3 c = ((diff) + (spec1 + spec2))*lightColor;	
	return c;
}

vec3 getDiffuseSpecular(int light)
{
	vec4 worldPosition = mMatrix * vec4(localPosition, 1.0);
	vec4 worldNormal = vec4(normal, 1.0);
	worldNormal.xyz = normalize(worldNormal.xyz);
	

	vec3 V = normalize(sendCameraPosition - worldPosition.xyz);
	vec3 L = normalize(lightPosition[light] - worldPosition.xyz);	

	//diffuse
	float diffuseLight = max(dot(L, worldNormal.xyz), 0.0);
	vec3 diffuse = diffuseColor * lightDiffuseColor * diffuseLight;	
	
	//specular
	vec3 specular = vec3(0.0);
	if (diffuseLight > 0.0) {
		vec3 R = reflect(-L, worldNormal.xyz);//L�́A���_�ɓ����Ă�������̃x�N�g���ɂ���
		float specularLight = pow(max(dot(R, V), 0.0), shininess);
		
		specular = materialShininess*lightShininess*specularLight;
	}

	return diffuse + specular;

}





vec4 getGaussedValue(vec3 textureCoord) 
{
	//�擾����_�̃T�C�Y.8�ߖT��, �����܂ŊO��������Ă��邩�B
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

float deepOpacityMapShadow(int light)
{

	vec4 nowPos = mvpPos[light] / mvpPos[light].w;//���݂̒��ړ_�̈ʒu
	
	vec4 depthTexColor = texture(depthTexture, vec3((nowPos.xy+1)*0.5, light));//1~5000
	float near = depthTexColor.x;
	float nowDepth = nowPos.z;//1~5000
	nowDepth = nowDepth - near;//�S�Ă̓_��near���X�^�[�g�ɂȂ���


	//�e�N�X�`���̑I��

	int texturePos = int(floor(nowDepth / textureInterval[light]));//�����ڂ̃e�N�X�`���ɏ������邩
	if (texturePos < 0) texturePos = 0;
	
	texturePos = texturePos + light * int(textureNum);

	
	
	vec3 textureCoord = vec3((nowPos.xy+1)*0.5, texturePos);

	vec4 opacityTexColor;
	if (pcfFlag == 0) {
		opacityTexColor = texture(opacityTexture, textureCoord);//4��opacity map�l
	} else if (pcfFlag == 1) {
		opacityTexColor = getGaussedValue(textureCoord);//4��opacity map�l
	}			

	float opacityValue[4];
	opacityValue[0] = opacityTexColor.r;
	opacityValue[1] = opacityTexColor.g;
	opacityValue[2] = opacityTexColor.b;
	opacityValue[3] = opacityTexColor.a;	
	

	float beforeLastOpacityValue;//��O�̃e�N�X�`���̍Ō�̒l���Ƃ�
	if (texturePos == light * int(textureNum)) {
		beforeLastOpacityValue = 0.0;
	} else {
		textureCoord = vec3(textureCoord.xy, texturePos-1);

		opacityTexColor = getGaussedValue(textureCoord);

		beforeLastOpacityValue = opacityTexColor.a;
	}

	//���C���̑I��
	nowDepth = nowDepth - (textureInterval[light] * texturePos);//nowDepth���A�����̃e�N�X�`���̃X�^�[�g�n�_����ɂȂ���.
	if (nowDepth < 0) nowDepth = 0.0;//�v�Z��near���߂��Ȃ������̂�, �n�߂Ɉʒu���Ă���Ɖ��肷��

	int count = int(floor(nowDepth / layerInterval[light]));//�������ʂ肱�ݎn�߂郌�C���������ڂ�.
				
	//�l�ƈʒu�����Ԃ���B
	nowDepth = (nowDepth - (layerInterval[light] * count)) / layerInterval[light];//nowDepth�̃X�^�[�g�n�_��, �e���C���̎n�܂�ɂȂ���.
	
	float currentOpacity;//���݂̌��̎Օ������v�Z����
	if (count == 0) {
		currentOpacity = (1.0 - nowDepth) * beforeLastOpacityValue + nowDepth * opacityValue[count];
	} else {
		currentOpacity = (1.0 - nowDepth) * opacityValue[count-1] + nowDepth * opacityValue[count];
	}
	
	if (currentOpacity > 1.0) {
		return 0.0;
	} else {
		return 1.0 - currentOpacity;
	}
}

float getLastOpacityValue(int light)
{
	vec4 nowPos = mvpPos[light] / mvpPos[light].w;
	vec3 textureCoord = vec3((nowPos.xy+1) * 0.5, int((light+1) * textureNum - 1));

	vec4 texColor;
	if (pcfFlag == 0) {
		texColor = texture(opacityTexture, textureCoord);
	} else if (pcfFlag == 1) {
		texColor = getGaussedValue(textureCoord);
	}

	if (texColor.a > 1) {
		return 0;
	} else {
		return (1.0 - texColor.a);
	}
}





in vec3 checkVec;
in vec3 sendtangent;
in vec3 sendnormal;
in float sendexpand;
in vec3 sendVec;

void main(void)
{
	const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
	const vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
	const vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
	const vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);
	const vec4 magenta = vec4(1.0, 0.0, 1.0, 1.0);
	const vec4 cyan = vec4(0.0, 1.0, 1.0, 1.0);
	const vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	const vec4 black = vec4(0.0, 0.0, 0.0, 1.0);

	if (sendHairFlag == 1.0) {
		vec3 ambient = materialAmbient * lightAmbient;
		
		float brightness = 0;
		vec3 kajiyaKayColor = vec3(0.0, 0.0, 0.0);

		

		if (shadow_noshadow == 1.0) {
			for (int i = 0; i < sendLightCount; i++) {
				brightness = deepOpacityMapShadow(i);
				kajiyaKayColor = lightingKajiyaKay(i);
				fragColor += vec4(ambient + kajiyaKayColor * brightness, 1.0);
			}	

		} else if (shadow_noshadow == 0.0){
			for (int i = 0; i < sendLightCount; i++) {
				kajiyaKayColor = lightingKajiyaKay(i);
				fragColor += vec4(ambient + kajiyaKayColor, 1.0);
			}

		}
	} else if (sendHairFlag == 0.0) {
		//ambient
		vec3 ambient = materialAmbient * lightAmbient;

		if (shadow_noshadow == 1.0) {		
			
			for (int i = 0; i < sendLightCount; i++) {
				vec3 diffSpec = getDiffuseSpecular(i);
				float brightness = getLastOpacityValue(i);
				fragColor += vec4(ambient + diffSpec*brightness, 1.0);
			}
		} else if (shadow_noshadow == 0.0){
			for (int i = 0; i < sendLightCount; i++) {
				vec3 diffSpec = getDiffuseSpecular(i);
				fragColor += vec4(ambient + diffSpec, 1.0);
			}
		}
	}

}