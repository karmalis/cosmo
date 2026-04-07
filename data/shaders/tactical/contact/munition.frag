#version 120

uniform int u_affiliation;
uniform float u_pixelSize;
uniform float u_thickness;
uniform vec4 u_color;

float sdEquilateralTriangle( in vec2 p, in float r )
{
    const float k = sqrt(3.0);
    p.x = abs(p.x) - r;
    p.y = p.y + r/k;
    if( p.x+k*p.y>0.0 ) p = vec2(p.x-k*p.y,-k*p.x-p.y)/2.0;
    p.x -= clamp( p.x, -2.0*r, 0.0 );
    return -length(p)*sign(p.y);
}

void main() {
    vec2 p = (gl_TexCoord[0].xy * u_pixelSize) - vec2(0.5);

    float r = 0.35;
    float th = 0.15;
    float t = 3.14159*(0.75);
    vec2 cs = vec2(cos(t),sin(t));
    
    float d_frame_outer = sdEquilateralTriangle(p, r);
    
    float d_frame_line = abs(d_frame_outer) - u_thickness;

    float aa = u_pixelSize * 1.5;

    float alpha_solid = 1.0 - smoothstep(-aa, aa, d_frame_line);
    float alpha_fill = (1.0 - smoothstep(-aa, aa, d_frame_outer)) * 0.3;
    float final_alpha = clamp(alpha_solid + alpha_fill, 0.0, 1.0);
    float glow = exp(-15.0 * max(0.0, d_frame_line)) * 0.5;

    gl_FragColor = vec4(u_color.rgb, u_color.a * clamp(final_alpha + glow, 0.0, 1.0));
}
