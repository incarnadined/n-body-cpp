#pragma once
#include "pch.h"

#include "Vec3.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND hWnd, int width, int height);
	void Clear(colour colour); // clears the screen to the specified colour - resets rtv, dsv
	bool AddSphere(std::function<std::tuple<float, colour, Vec3f>()> dataFunction); // adds a sphere location callback to the active drawable objects - returns true on success
	void Draw(); 
	void EndFrame();


	// Getters/Setters
	float GetStarConc();
	void SetStarConc();
	Vec3f GetCameraPos();
	void MoveCamera(Vec3f translation);

private:
	std::vector< // a vector containing pointers to the functions that can be called to get spherical data
		std::function<
			std::tuple<float, colour, Vec3f>() // the function returns a tuple in the form <RADIUS, COLOUR, POSITION)
		>> pSpheres;

	float starConcentration; // star intensity (0-1)
	Vec3f cameraPos;
};

