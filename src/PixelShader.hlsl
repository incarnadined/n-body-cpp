cbuffer ColourBuf
{
    float count;
    float4 colours[8];
};

float4 main(uint id : SV_PrimitiveID) : SV_TARGET
{
    return colours[id / count];
}