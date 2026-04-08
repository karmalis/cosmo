#version 120

uniform float u_time;
uniform vec4 u_color;     
uniform float u_radius;    // < 1.0
uniform float u_glowWidth; 
uniform float u_pixelSize; 

void main() {
    vec2 p = (gl_TexCoord[0].xy * u_pixelSize) - 0.5;
    float dist = length(p);

    float planetSDF = dist - u_radius;
    
    float glow = exp(-15.0 * max(0.0, planetSDF / u_glowWidth));

    float fresnel = 0.0;
    if (dist < u_radius) {
        fresnel = pow(dist / u_radius, 3.0) * 0.6;
    }

    float scanline = 0.9 + 0.1 * sin(p.y * 500.0 + u_time * 2.0);
    
    vec3 baseColor = u_color.rgb;
    float alpha = 0.0;

    if (dist < u_radius) {
        alpha = u_color.a;
        baseColor += fresnel;
        baseColor *= scanline;
    } else {
        alpha = glow * u_color.a;
    }

    gl_FragColor = vec4(baseColor, alpha);
}