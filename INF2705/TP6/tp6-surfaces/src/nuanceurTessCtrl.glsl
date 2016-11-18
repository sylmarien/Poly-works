#version 430

layout(vertices = 3) out;

// in int gl_PatchVerticesIn;
// in int gl_PrimitiveID;
// in int gl_InvocationID;
// in gl_PerVertex
// {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// } gl_in[gl_MaxPatchVertices];

// patch out float gl_TessLevelOuter[4];
// patch out float gl_TessLevelInner[2];
// out gl_PerVertex
// {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// } gl_out[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

void main()
{
   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

   if ( gl_InvocationID == 0 )
   {
      gl_TessLevelInner[0] = TessLevelInner;
      gl_TessLevelOuter[0] = TessLevelOuter;
      gl_TessLevelOuter[1] = TessLevelOuter;
      gl_TessLevelOuter[2] = TessLevelOuter;
   }
}
