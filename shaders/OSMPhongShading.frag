#version 400



//�t���O�֘A
flat in float sendHairFlag;
uniform float shadow_noshadow;
uniform float pcfFlag;

//����
in vec3 normal;

//kajiya-kay�֘A
in vec3 mvPos;
in vec3 globalPosition;
in vec3 binormal;
in vec3 tangent;
uniform vec3 lightPosition;
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


//OSM�p
in vec4 mlvlpPos;
in vec4 mvFromLight;
uniform sampler2DArray shadowTexture;
uniform float osmNum;
uniform float osmNearArray[32];
uniform float osmFarArray[32];
uniform float oNear, interval;




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
vec3 lightingKajiyaKay()
{
	//�J�������W�Ōv�Z���s���Ă���
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightDir = normalize(lightPosition - globalPosition);
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

vec3 getDiffuseSpecular()
{
		vec4 worldPosition = mMatrix * vec4(localPosition, 1.0);
	vec4 worldNormal = vec4(normal, 1.0);
	worldNormal.xyz = normalize(worldNormal.xyz);
	
	vec3 V = normalize(sendCameraPosition - worldPosition.xyz);
	vec3 L = normalize(lightPosition - worldPosition.xyz);

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




float getGaussedValue(vec3 textureCoord) 
{
	if (pcfFlag == 1) {
		//�擾����_�̃T�C�Y.8�ߖT��, �����܂ŊO��������Ă��邩�B
		int radius = 1;
		//float nor = 1.0 / ((radius*2+1) * (radius*2+1) - 1);//1����int�̂܂܂Ōv�Z�����B
		float nor = 0.0;

		//�ߖT�e�N�Z���̔�r���v
		float sum = 0;

		vec2 nowTexCoord;
		nowTexCoord.x = textureCoord.x * texWidth;
		nowTexCoord.y = textureCoord.y * texHeight;
		
		if (nowTexCoord.x >= (texWidth - radius) || nowTexCoord.y >= (texHeight - radius) 
			|| nowTexCoord.x <= radius || nowTexCoord.y <= radius) {
			return 0.0;
		}

		
		for (int i = -radius; i <= radius; i++) {
			for (int j = -radius; j <= radius; j++) {
				if (i == 0 && j == 0) continue;


				nowTexCoord.x = nowTexCoord.x + i;
				nowTexCoord.y = nowTexCoord.y + j;

				textureCoord.x = nowTexCoord.x / texWidth;
				textureCoord.y = nowTexCoord.y / texHeight;

				sum += texture(shadowTexture, textureCoord).x;
				nor += 1;
			}
		}
		
		nor = 1.0 / nor;
		return sum *= nor;
	} else if (pcfFlag == 0) {
		return texture(shadowTexture, textureCoord).x;
	}
}

float getCorrectShadow() 
{
	//�g���e�N�X�`���̌���
	float zPos = abs(mvFromLight.z) - oNear;
	int count = int(floor(zPos / interval));//�����̏�������e�N�X�`���̌���. floor���g���B


	if (count >= osmNum) count = int(osmNum) - 1;


	//���ڂ��Ă���ꏊ
	vec4 nowPos = mlvlpPos / mlvlpPos.w;//mvp�ς�
	
	//��Ԃ�����ʒu���v�Z
	float interpolatePoint = (abs(mvFromLight.z) - osmNearArray[count]) / interval;
	
	//���ڒn�_�ł̒l���Ԃ��ċ��߂�
	vec3 texCoord = vec3((nowPos.xy + 1) * 0.5, count);
	float opacityDcurrent = getGaussedValue(texCoord);

	

	
	//�ŏ��̈ꖇ�ڂ������g���ꍇ�͏������Ⴄ
	if (count > 0) {
		texCoord = vec3((nowPos.xy + 1) * 0.5, count-1);
		float opacityDprev = getGaussedValue(texCoord);
		opacityDcurrent = interpolatePoint * opacityDcurrent + (1.0 - interpolatePoint) * opacityDprev;
	} else if (count == 0) {
		opacityDcurrent = interpolatePoint * opacityDcurrent;
	} 

	//�����֐��v�Z
	//float taw = pow(2.718, (-1.0 * 5.56 * opacityDcurrent));
	
	float shadow = 1.0 - opacityDcurrent;
	if (shadow < 0.0) {
		shadow = 0.0;
	}
	return shadow;
	
}

void main(void)
{
	float shadow = getCorrectShadow();
	
	
	if (sendHairFlag == 1.0) {
		vec3 ambient = materialAmbient * lightAmbient;

		vec3 kajiyaKayColor = vec3(0.0, 0.0, 0.0);
		
		if (shadow_noshadow == 1.0) {
			kajiyaKayColor = lightingKajiyaKay();	
			fragColor = vec4(ambient + kajiyaKayColor * shadow, 1.0);
		} else if (shadow_noshadow == 0.0){
			kajiyaKayColor = lightingKajiyaKay();	
			fragColor = vec4(ambient + kajiyaKayColor, 1.0);
		}
		return;
	} else if (sendHairFlag == 0.0) {
		/*
		//ambient
		vec3 ambient = materialAmbient * lightAmbient;
		vec3 diffSpec = getDiffuseSpecular();

		if (shadow_noshadow == 1.0) {
			fragColor = vec4(ambient + diffSpec*shadow, 1.0);
		} else if (shadow_noshadow == 0.0){
			fragColor = vec4(ambient + diffSpec, 1.0);
		}
		return;
		*/
		vec3 ambient = materialAmbient * lightAmbient;
		if (shadow < 0) shadow = 0;
		if (shadow_noshadow == 1.0) {
			fragColor = vec4(ambient + vec3(1.0, 1.0, 1.0) * shadow, 1.0);
		} else if (shadow_noshadow == 0.0){
			fragColor = vec4(ambient + vec3(1.0, 1.0, 1.0), 1.0);
		}
		return;
	}
}