#version 120

uniform float u_pixelSize; // 1.0 / size of the quad
uniform float u_thickness; // Thickness of the lines (relative to quad size)
uniform float u_size;      // Length of the crosshair bars (relative to quad size)
uniform float u_gap;       // Radius of the central gap (relative to quad size)

// SDF for a centered Box
float sdBox(vec2 p, vec2 b) {
    vec2 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

// SDF for a Circle
float sdCircle(vec2 p, float r) {
    return length(p) - r;
}

void main() {
    // Use local coordinates passed from vertex shader, normalized to [-0.5, 0.5]
    vec2 uv = (gl_TexCoord[0].xy * u_pixelSize) - 0.5;
    
    // 1. Create the bars (Union of horizontal and vertical thin boxes)
    float horizontalBar = sdBox(uv, vec2(u_size, u_thickness));
    float verticalBar   = sdBox(uv, vec2(u_thickness, u_size));
    float bars = min(horizontalBar, verticalBar);

    // 2. Create the gap circle
    float gapCircle = sdCircle(uv, u_gap);

    // 3. Subtract the circle from the bars
    float crosshairSDF = max(bars, -gapCircle);

    // 4. Anti-aliasing using smoothstep relative to pixel size
    float edgeSoftness = u_pixelSize; 
    float alpha = 1.0 - smoothstep(-edgeSoftness, edgeSoftness, crosshairSDF);

    gl_FragColor = vec4(0.0, 1.0, 0.0, alpha); // Tactical Green
}