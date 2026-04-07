#version 120

void main() {
    // Transform position to screen coordinates
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Pass the local vertex position as texture coordinates
    // For a centered RectangleShape, this will be [-size/2, size/2]
    gl_TexCoord[0] = gl_Vertex;

    // Pass the vertex color
    gl_FrontColor = gl_Color;
}
