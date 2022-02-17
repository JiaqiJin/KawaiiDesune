#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace spdlog
{
	class logger;
}

namespace Excalibur
{
	class IModule 
	{
	public:
		virtual ~IModule() = default;
		virtual int Initialize() noexcept = 0;
		virtual void Finalize() noexcept = 0;

	protected:
		std::shared_ptr<spdlog::logger> m_Logger;
	};

	class ITickableModule : public IModule
	{
		virtual void Tick() noexcept = 0;
	};
}