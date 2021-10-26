#pragma once

#include "../Common/Input.h"

#include <memory>

namespace RHI
{
	class GraphicCore;
}

namespace KAWAII
{
	class Input;
}

struct InitParam
{
	bool vsync = false;
	bool load_default_scene = true;
};

class KawaiiEngine
{
public:
	KawaiiEngine(InitParam const& param);
	KawaiiEngine(KawaiiEngine const&) = delete;
	KawaiiEngine(KawaiiEngine&&) = delete;
	KawaiiEngine& operator=(KawaiiEngine const&) = delete;
	KawaiiEngine& operator=(KawaiiEngine&&) = delete;
	~KawaiiEngine();

private:
	void Initalize();
	void Update(float deltaTime);

private:
	bool vsync;

	KAWAII::Input m_input;

	std::unique_ptr<RHI::GraphicCore> gfx;
};