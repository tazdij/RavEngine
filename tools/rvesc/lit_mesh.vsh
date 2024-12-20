#define MODEL_MATRIX_BINDING 10
#define ENTITY_INPUT_LOCATION 10

#include "lit_mesh_bindings.glsl"

struct LitVertexOut{
    vec4 position;
    vec3 worldPosition;
};

struct EntityIn{
    mat4 modelMtx;
    uint entityID;
};

#include "enginedata.glsl"

#include "%s"

layout(location = ENTITY_INPUT_LOCATION) in uint inEntityID;

#define VARYINGDIR out
#include "mesh_varyings.glsl"

layout(std430, binding = MODEL_MATRIX_BINDING) readonly buffer modelMatrixBuffer{mat4 model[];};

#include "lit_mesh_shared.glsl"
#include "make_engine_data.glsl"

void main(){
    EntityIn entity;
    entity.entityID = inEntityID;
    entity.modelMtx = model[inEntityID];

    EngineData data = make_engine_data(engineConstants[0]);

    LitVertexOut user_out = vert(entity, data);

    gl_Position = user_out.position;
    worldPosition = user_out.worldPosition;
    clipSpaceZ = gl_Position.z;
    viewPosition = (engineConstants[0].viewOnly * vec4(worldPosition,1)).xyz;
    varyingEntityID = inEntityID;
}
