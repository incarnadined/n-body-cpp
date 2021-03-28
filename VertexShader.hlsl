cbuffer CBuf
{
	matrix mvp;
};

struct VSOut
{
	float4 colour : Colour;
	float4 pos : SV_Position;
};

VSOut main(float4 pos : Position, float4 col : Colour)
{
	VSOut vsout;
	vsout.pos = mul(pos, mvp);
	vsout.colour = col;
	return vsout;
}