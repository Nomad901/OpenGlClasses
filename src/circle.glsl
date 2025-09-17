void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy * 2.0f - 1.0f; // is just a position
    float aspect = iResolution.x / iResolution.y;
    uv.x *= aspect;
    
    fragColor.rg = uv;
    fragColor.b = 0.0;
    
    float fade = 0.05f;
    float distance = 1.0f - length(uv);
    distance = smoothstep(0.0f, fade, distance);
    // if(distance > 0.0f)
    //    distance = 1.0f;
 
    fragColor.rgb = vec3(distance);
}
