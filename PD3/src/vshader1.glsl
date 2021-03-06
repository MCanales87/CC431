#version 410

layout (location=0) in vec3 position;  // coord

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 varyingColor;  // be interpolated by the rasterizer

void main(void) {
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);  // right-to-left
    
    if (gl_VertexID == 0)
        varyingColor = vec4(1.0, 0.0, 0.0, 1.0);
    else if (gl_VertexID == 1)
        varyingColor = vec4(0.0, 1.0, 0.0, 1.0);
    else
        varyingColor = vec4(0.0, 0.0, 1.0, 1.0);
}
