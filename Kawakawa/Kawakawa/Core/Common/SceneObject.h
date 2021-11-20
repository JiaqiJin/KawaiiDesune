#pragma once

#include "../BaseSceneObject.h"
#include "../../Utility/SceneObjectTypeDef.h"

namespace Kawaii::Core
{
	/*
	* Each vertex is a structure (composite data type) containing multiple fields called "attributes". Common :
	* Local Attributes, Normal Attributes, Tangent Attributes, Texture coordinates (UV)...
	* In the case of parallel computing, SOA is often more effective. 
	* SOA is the abbreviation of Struct Of Array, which is to organize the same attribute of different vertices in an array, 
	* and then organize the array of these different attributes into a structure.
	*/
	class SceneObjectVertexArray : public BaseSceneObject
	{
	public:
		SceneObjectVertexArray(std::string_view attr,
			VertexDataType data_type,
			Core::Buffer&& buffer,
			uint32_t morph_index = 0);
		SceneObjectVertexArray(const SceneObjectVertexArray&) = default;
		SceneObjectVertexArray(SceneObjectVertexArray&&) = default;
		SceneObjectVertexArray& operator=(const SceneObjectVertexArray&) = default;
		SceneObjectVertexArray& operator=(SceneObjectVertexArray&&) = default;
		~SceneObjectVertexArray() override = default;

		const std::string& GetAttributeName() const;
		VertexDataType       GetDataType() const;
		size_t               GetDataSize() const;
		const uint8_t* GetData() const;
		size_t               GetVertexCount() const;
		size_t               GetVertexSize() const;
		friend std::ostream& operator<<(std::ostream& out, const SceneObjectVertexArray& obj);
	private:
		std::string    m_Attribute;   // Vertex Attribute
		Core::Buffer   m_Data;	      // Data
		VertexDataType m_DataType;    // Data Type
		size_t         m_VertexCount; // Vertex Count
		
		uint32_t m_MorphTargetIndex;
	};

	/*
	* In order to save memory bandwidth, we should try to use 16bit index(max 65535 vertices).
	* But at the same time, in order to reduce the drawcall, we need to draw as many vertices as possible each time the drawcall is called.
	*/
	class SceneObjectIndexArray : public BaseSceneObject
	{
	public:
		SceneObjectIndexArray(const IndexDataType data_type,
			Core::Buffer&& data,
			const uint32_t restart_index = 0, const uint32_t materialIndex = 0);

		SceneObjectIndexArray(const SceneObjectIndexArray&) = default;
		SceneObjectIndexArray(SceneObjectIndexArray&&) = default;
		SceneObjectIndexArray& operator=(const SceneObjectIndexArray&) = default;
		SceneObjectIndexArray& operator=(SceneObjectIndexArray&&) = default;
		~SceneObjectIndexArray() override = default;

		const IndexDataType  GetIndexType() const;
		const uint8_t*		 GetData() const;
		uint32_t             GetMaterialID() const;
		size_t               GetIndexCount() const;
		size_t               GetDataSize() const;
		size_t               GetIndexSize() const;
		friend std::ostream& operator<<(std::ostream& out, const SceneObjectIndexArray& obj);
	private:
		uint32_t      m_MaterialIndex;
		IndexDataType m_DataType;
		size_t        m_IndexCount = 0;
		Core::Buffer  m_Data;

		size_t m_ResetartIndex = 0;
	};

	/*
	* Different vector elements correspond to different attributes of
    * vertices, such as the first element is coorespond to the position of
    * vertices, the second element is coorrespond to the normal of vertices, etc.
	*/
	class SceneObjectMesh : public BaseSceneObject
	{
	public:
		SceneObjectMesh(bool visible = true, bool shadow = true, bool motion_blur = true)
			: BaseSceneObject(SceneObjectType::Mesh) {}

		SceneObjectMesh(SceneObjectMesh&& mesh)
			: BaseSceneObject(SceneObjectType::Mesh),
			m_IndexArray(std::move(mesh.m_IndexArray)),
			m_VertexArray(std::move(mesh.m_VertexArray)),
			m_PrimitiveType(mesh.m_PrimitiveType) {}

		friend std::ostream& operator<<(std::ostream& out, const SceneObjectMesh& obj);
	protected:
		std::vector<SceneObjectVertexArray> m_VertexArray;
		std::vector<SceneObjectIndexArray>  m_IndexArray;
		PrimitiveType m_PrimitiveType;

		bool m_Visible = true;
		bool m_Shadow = false;
		bool m_MotionBlur = false;
	};

	// Scene 
	class SceneObjectGeometry : public BaseSceneObject
	{
	public:
		SceneObjectGeometry()
			:BaseSceneObject(SceneObjectType::Geometry) {}

		void SetVisibility(bool visible);
		void SetIfCastShadow(bool shadow);
		void SetIfMotionBlur(bool motion_blur);
		void SetCollisionType(SceneObjectCollisionType collision_type);
		void SetCollisionParameters(const float* param, int32_t count);
		const bool Visible() const;
		const bool CastShadow() const;
		const bool MotionBlur() const;
		const SceneObjectCollisionType CollisionType() const;
		const float* CollisionParameters() const;
		void AddMesh(std::unique_ptr<SceneObjectMesh> mesh, size_t level = 0);
		const std::vector<std::unique_ptr<SceneObjectMesh>>& GetMeshes(size_t lod = 0) const;
		friend std::ostream& operator<<(std::ostream& out, const SceneObjectGeometry& obj);
	protected:
		// LOD | meshes array
		//  0  | meshes array[0] include multiple meshes at 0 LOD
		// ... | ...
		std::vector<std::vector<std::unique_ptr<SceneObjectMesh>>> m_MeshesLOD;
		bool                     m_Visible = true;
		bool                     m_Shadow = false;
		bool                     m_MotionBlur = false;
		SceneObjectCollisionType m_CollisionType{ SceneObjectCollisionType::None };
		std::array<float, 10>    m_CollisionParameters{};
	};

	// Scene Transform
	class SceneObjectTransform
	{
	public:
		SceneObjectTransform() : m_Transform(1.0f) {}
		SceneObjectTransform(const mat4f matrix, const bool objectOnly = false)
			: m_Transform(matrix), m_SceneObjectOnly(objectOnly) {}

		operator mat4f() { return m_Transform; }
		operator const mat4f() const { return m_Transform; }

		friend std::ostream& operator<<(std::ostream& out, const SceneObjectTransform& obj);
	protected:
		mat4f m_Transform;
		bool m_SceneObjectOnly{ false };
	};

	// Scene Translation
	class SceneObjectTranslation : public SceneObjectTransform
	{
	public:
		SceneObjectTranslation(const char axis, const float amount);
		SceneObjectTranslation(const float x, const float y, const float z);
	};

	class SceneObjectRotation : public SceneObjectTransform 
	{
	public:
		SceneObjectRotation(const char axis, const float theta);
		SceneObjectRotation(vec3f& axis, const float theta);
		SceneObjectRotation(quatf quaternion);
	};

	class SceneObjectScale : public SceneObjectTransform
	{
	public:
		SceneObjectScale(const char axis, const float amount);
		SceneObjectScale(const float x, const float y, const float z);
	};
}