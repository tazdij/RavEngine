$input v_normal, v_tangent, v_texcoord0, v_worldpos

#include "common.sh"
#include <bgfx_shader.sh>
#include "pbr_fs.glsl"

SAMPLER2D(s_albedoTex,0);
uniform vec4 albedoColor;

void main()
{
	//PBR material = PBR(,v_normal,v_worldpos);
	PBR material = make_mat();
	material.color = toLinear(texture2D(s_albedoTex, v_texcoord0) ) * albedoColor;
	material.normal = v_normal;
	material.position = (u_modelView * vec4(v_worldpos,1)).xyz;
	
	store(material);
}
