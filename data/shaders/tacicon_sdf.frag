#version 120

uniform float u_affiliation; 
uniform float u_type;        
uniform float u_thickness;  
uniform float u_pixelSize;  
uniform vec4 u_color;      

float sdBox(vec2 p, vec2 b) {
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float sdRhombus(vec2 p, vec2 b) {
    vec2 q = abs(p);
    float h = clamp((-2.0*(q.x*b.x - q.y*b.y) + (b.x*b.x - b.y*b.y)) / (b.x*b.x + b.y*b.y), -1.0, 1.0);
    float d = length(q - 0.5 * b * vec2(1.0-h, 1.0+h));
    return d * sign(q.x*b.y + q.y*b.x - b.x*b.y);
}

float sdCircle(vec2 p, float r) {
    return length(p) - r;
}

float sdSegment(vec2 p, vec2 a, vec2 b) {
    vec2 pa = p-a, ba = b-a;
    float h = clamp(dot(pa,ba)/dot(ba,ba), 0.0, 1.0);
    return length(pa - ba*h);
}

float sdArc(vec2 p, float r, float start_angle, float end_angle) {
    float angle = atan(p.y, p.x);
    float d = abs(length(p) - r);
    if (angle < start_angle || angle > end_angle) {
        vec2 p_start = vec2(cos(start_angle), sin(start_angle)) * r;
        vec2 p_end = vec2(cos(end_angle), sin(end_angle)) * r;
        d = min(length(p - p_start), length(p - p_end));
    }
    return d;
}

void main() {
    vec2 p = (gl_TexCoord[0].xy * u_pixelSize) - vec2(0.5);
    
    float d_frame_outer = 1e10;
    float d_icon = 1e10;
    float d_mod = 1e10; 
    
    // Reduced base scale from 0.35 to 0.28 to prevent cutoffs
    float s = 0.28; 
    float t = u_thickness;
    
    int aff = int(u_affiliation + 0.5);
    int type = int(u_type + 0.5);

    // --- 1. Frames (Affiliation) ---
    if (aff == 1) { // Friend: Rectangle
        d_frame_outer = sdBox(p, vec2(s * 1.1, s * 0.8));
    } else if (aff == 3) { // Hostile: Diamond
        d_frame_outer = sdRhombus(p, vec2(s * 1.3, s * 1.3));
    } else if (aff == 2) { // Neutral: Square
        d_frame_outer = sdBox(p, vec2(s, s));
    } else { // Unknown: Circle
        d_frame_outer = sdCircle(p, s);
    }

    float d_frame_line = abs(d_frame_outer) - t;

    // --- 2. Space Modifier (Arc at bottom) ---
    // Repositioned and scaled to fit within bounds
    d_mod = sdArc(p - vec2(0.0, -0.05), s * 1.4, 0.6, 2.5) - t * 0.8;

    // --- 3. Internal Icons (Type) ---
    if (type == 1) { // Ship: Vertical Dash
        d_icon = sdSegment(p, vec2(0.0, -s*0.5), vec2(0.0, s*0.5)) - t;
    } else if (type == 2) { // Grouping: Three dots
        d_icon = sdCircle(p - vec2(-s*0.4, 0.0), t);
        d_icon = min(d_icon, sdCircle(p - vec2(0.0, 0.0), t));
        d_icon = min(d_icon, sdCircle(p - vec2(s*0.4, 0.0), t));
    } else if (type == 3) { // Station: Diamond
        d_icon = sdRhombus(p, vec2(s*0.4, s*0.4)) - t*0.5;
    } else if (type == 4) { // Munition: Triangle
        d_icon = sdSegment(p, vec2(0.0, s*0.4), vec2(-s*0.3, -s*0.4)) - t;
        d_icon = min(d_icon, sdSegment(p, vec2(0.0, s*0.4), vec2(s*0.3, -s*0.4)) - t);
        d_icon = min(d_icon, sdSegment(p, vec2(-s*0.3, -s*0.4), vec2(s*0.3, -s*0.4)) - t);
    }

    float d_solid = min(d_frame_line, min(d_icon, d_mod));

    float aa = u_pixelSize * 1.5;
    float alpha_solid = 1.0 - smoothstep(-aa, aa, d_solid);
    float alpha_fill = (1.0 - smoothstep(-aa, aa, d_frame_outer)) * 0.3;
    
    float final_alpha = clamp(alpha_solid + alpha_fill, 0.0, 1.0);
    float glow = exp(-25.0 * max(0.0, d_solid)) * 0.4; // Tightened glow
    
    gl_FragColor = vec4(u_color.rgb, u_color.a * clamp(final_alpha + glow, 0.0, 1.0));
}
