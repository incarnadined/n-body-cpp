/*float4 colours[4] = {
    { 1.0f, 0.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f, 1.0f },
};*/

cbuffer ColourBuf
{
    float4 colours[4];
};

float4 main(float colour : Colour) : SV_TARGET
{
    return colours[colour % 4];
    //return float4(colour / 11.0f, colour / 11.0f, colour / 11.0f, 1.0f);
}