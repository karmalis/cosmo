#version 120

uniform int u_affiliation;
uniform float u_pixelSize;
uniform float u_thickness;
uniform vec4 u_color;

float sdCircle( vec2 p, float r )
{
    return length(p) - r;
}

float sdRing( in vec2 p, in vec2 n, in float r, float th )
{
    p.x = abs(p.x);
    p = mat2x2(n.x,n.y,-n.y,n.x)*p;
    return max( abs(length(p)-r)-th*0.5,
                length(vec2(p.x,max(0.0,abs(r-p.y)-th*0.5)))*sign(p.x) );
}

float sdRhombus( in vec2 p, in vec2 b ) 
{
    b.y = -b.y;
    p = abs(p);
    float h = clamp( (dot(b,p)+b.y*b.y)/dot(b,b), 0.0, 1.0 );
    p -= b*vec2(h,h-1.0);
    return length(p)*sign(p.x);
}

float sdQuatrefoil(vec2 p, float r) 
{
    p = abs(p);
    return length(p - min(p.x + p.y, r) * 0.5) - r * 0.5;
}


void main() {
    vec2 p = (gl_TexCoord[0].xy * u_pixelSize) - vec2(0.5);

    float r = 0.35;
    float th = 0.15;
    float t = 3.14159*(0.75);
    vec2 cs = vec2(cos(t),sin(t));
    
    float d_frame_outer = 1e10;
    if (u_affiliation == 1) {
        d_frame_outer = sdRing(p, cs, r, th);
    } else if (u_affiliation == 3) {
        vec2 b = vec2(0.35, 0.35);
        d_frame_outer = sdRhombus(p, b);
    } else if (u_affiliation == 0) {
        d_frame_outer = sdQuatrefoil(p, r);
    }

    float d_frame_line = abs(d_frame_outer) - u_thickness;

    float aa = u_pixelSize * 1.5;

    float alpha_solid = 1.0 - smoothstep(-aa, aa, d_frame_line);
    float alpha_fill = (1.0 - smoothstep(-aa, aa, d_frame_outer)) * 0.3;
    float final_alpha = clamp(alpha_solid + alpha_fill, 0.0, 1.0);
    float glow = exp(-15.0 * max(0.0, d_frame_line)) * 0.5;

    gl_FragColor = vec4(u_color.rgb, u_color.a * clamp(final_alpha + glow, 0.0, 1.0));
}
