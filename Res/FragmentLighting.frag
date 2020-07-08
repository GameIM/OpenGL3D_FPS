/**
* @file FragmentLighting.frag
*/
#version 410
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

out vec4 fragColor;

//����
struct AmbientLight
{
	vec3 color;
};

//�w�������C�g
struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLight;

uniform sampler2D texColor;

/**
* �t���O�����g�V�F�[�_�[
*/
void main()
{
	vec3 normal = normalize(inNormal);

	//�����o�[�g���˂ɂ��g�U���ˌ��̌v�Z
	float cosTheta = clamp(dot(normal, -directionalLight.direction),0.0,1.0);
	vec3 lightColor = directionalLight.color * cosTheta;

	//�����̖��邳���v�Z
	lightColor += ambientLight.color;

	fragColor = inColor * texture(texColor, inTexCoord);
	fragColor.rgb *= lightColor;
}
