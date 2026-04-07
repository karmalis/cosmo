#version 120

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5);

    float dist = length(coord);

    if (dist > 0.75)
    
    discard;

    float softness = 1.0 - smoothstep(0.0, 0.5, dist);

    vec4 color = gl_Color;
    color.a *= softness;

    gl_FragColor = color;
}