#version 410

in vec4 varyingColor;  // the same name

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void) {
    //~ color = vec4(1,1,1,1);
        color = varyingColor;
}
