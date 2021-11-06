uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    // vec4 color = vec4(1.0 - gl_FragColor.x, 1.0 - gl_FragColor.y, 1.0 - gl_FragColor.z, gl_FragColor.w);
    vec4 color = vec4(1.0 - pixel.x, 1.0 - pixel.y, 1.0 - pixel.z, pixel.w);
    gl_FragColor = color;
}
