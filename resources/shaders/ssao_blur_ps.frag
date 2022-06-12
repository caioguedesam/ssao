#version 330 core
in vec2 vTexCoord;

layout (location = 0) out float ssaoBlurResult;	// SSAO blur output texture (R16_UNORM)

uniform sampler2D tex0;     // SSAO initial result

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(tex0, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(tex0, vTexCoord + offset).r;
        }
    }
    ssaoBlurResult = result / (4.0 * 4.0);
}  