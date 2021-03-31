cbuffer CBuf
{
	matrix mvp;
};

struct VSOut
{
	float4 pos : SV_Position;
};

VSOut main(float4 pos : Position)
{
	VSOut vsout;
	vsout.pos = mul(pos, mvp);
	return vsout;
}