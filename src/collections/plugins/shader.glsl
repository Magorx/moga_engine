uniform sampler2D texture;

uniform int ker_size;
uniform float kernel[100];
uniform float tx_w;
uniform float tx_h;

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec2 pos = gl_TexCoord[0].xy;
    vec2 pixel_size = 1.0 / vec2(tx_w, tx_h);

    vec4 sum_color = vec4(0.0, 0.0, 0.0, 0.0);
    float sum_weight = 0.0;

    int y = -1;
    for (int dy = -ker_size; dy <= ker_size; dy = dy + 1) {
        y += 1;
        int x = -1;
        for (int dx = -ker_size; dx <= ker_size; dx = dx + 1) {
            x += 1;
            vec2 cp = pos + vec2(dx, dy) * pixel_size;
            vec4 cc = texture2D(texture, cp);
            sum_color = sum_color + cc * kernel[(ker_size * 2 + 1) * y + x];
            sum_weight = sum_weight + kernel[(ker_size * 2 + 1) * y + x];
        }
    }

    vec4 color = sum_color / sum_weight;
    gl_FragColor = color;
}
