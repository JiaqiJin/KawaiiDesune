#pragma once

#include <memory>
#include <unordered_map>
#include <string>

namespace Kawaii::Graphics
{
	namespace backend 
	{
		class Resource 
		{
		public:
			virtual ~Resource() = default;
		};
	}  // namespace backend

	class Resource
	{
	public:
		Resource(std::string_view name, std::unique_ptr<backend::Resource> resource)
			: m_Name(name), m_Resource(std::move(resource)) {};
		Resource(const Resource&) = delete;
		Resource& operator=(const Resource&) = delete;
		Resource(Resource&&) = default;
		Resource& operator=(Resource&&) = default;

		inline const std::string& GetName() const noexcept { return m_Name; }

		template<typename T>
		inline T* GetBackend() const noexcept
		{
			return static_cast<T*>(m_Resource.get());
		}

	private:
		std::string m_Name;
		std::unique_ptr<backend::Resource> m_Resource;
	};

	class VertexBuffer : public Resource
	{
	public:
		using Resource::Resource;
	};

	class IndexBuffer : public Resource 
	{
	public:
		using Resource::Resource;
	};
}