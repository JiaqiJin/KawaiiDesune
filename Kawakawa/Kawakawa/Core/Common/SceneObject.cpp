#include "SceneObject.h"

#include <magic_enum.hpp>

namespace Kawaii::Core
{
	// Class SceneObjectVertexArray
    SceneObjectVertexArray::SceneObjectVertexArray(std::string_view attr,
        VertexDataType   data_type,
        Core::Buffer&& buffer,
        uint32_t         morph_index)
        : m_Attribute(attr),
        m_DataType(data_type),
        m_VertexCount(buffer.GetDataSize() / GetVertexSize()),
        m_Data(std::move(buffer)),
        m_MorphTargetIndex(morph_index) 
    {

    }

    const std::string& SceneObjectVertexArray::GetAttributeName() const { return m_Attribute; }
    VertexDataType     SceneObjectVertexArray::GetDataType() const { return m_DataType; }
    size_t             SceneObjectVertexArray::GetDataSize() const { return m_Data.GetDataSize(); }
    const uint8_t*     SceneObjectVertexArray::GetData() const { return m_Data.GetData(); }
    size_t             SceneObjectVertexArray::GetVertexCount() const { return m_VertexCount; }

    size_t SceneObjectVertexArray::GetVertexSize() const 
    {
        switch (m_DataType) 
        {
        case VertexDataType::Float1:
            return sizeof(float) * 1;
        case VertexDataType::Float2:
            return sizeof(float) * 2;
        case VertexDataType::Float3:
            return sizeof(float) * 3;
        case VertexDataType::Float4:
            return sizeof(float) * 4;
        case VertexDataType::Double1:
            return sizeof(double) * 1;
        case VertexDataType::Double2:
            return sizeof(double) * 2;
        case VertexDataType::Double3:
            return sizeof(double) * 3;
        case VertexDataType::Double4:
            return sizeof(double) * 4;
        }
        return 0;
    }

    // Class SceneObjectIndexArray
    SceneObjectIndexArray::SceneObjectIndexArray(
        const IndexDataType data_type,
        Core::Buffer&& data,
        const uint32_t restart_index,
        const uint32_t material_index)
        : m_DataType(data_type),
        m_IndexCount(data.GetDataSize() / GetIndexSize()),
        m_Data(std::move(data)),
        m_ResetartIndex(restart_index),
        m_MaterialIndex(material_index)
    {

    }
    
    const IndexDataType SceneObjectIndexArray::GetIndexType() const { return m_DataType; }
    const uint8_t*      SceneObjectIndexArray::GetData() const { return m_Data.GetData(); }
    uint32_t            SceneObjectIndexArray::GetMaterialID() const { return m_MaterialIndex; }
    size_t              SceneObjectIndexArray::GetIndexCount() const { return m_IndexCount; }
    size_t              SceneObjectIndexArray::GetDataSize() const { return m_Data.GetDataSize(); }

    size_t SceneObjectIndexArray::GetIndexSize() const
    {
        switch (m_DataType)
        {
        case IndexDataType::Int8:
            return sizeof(int8_t);
        case IndexDataType::Int16:
            return sizeof(int16_t);
        case IndexDataType::Int32:
            return sizeof(int32_t);
        case IndexDataType::Int64:
            return sizeof(int64_t);
        }
        return 0;
    }

    // Class SceneObjectGeometry
    void SceneObjectGeometry::SetVisibility(bool visible) { m_Visible = visible; }
    void SceneObjectGeometry::SetIfCastShadow(bool shadow) { m_Shadow = shadow; }
    void SceneObjectGeometry::SetIfMotionBlur(bool motion_blur) { m_MotionBlur = motion_blur; }
    void SceneObjectGeometry::SetCollisionType(SceneObjectCollisionType collision_type) { m_CollisionType = collision_type; }
    void SceneObjectGeometry::SetCollisionParameters(const float* param, int32_t count) 
    {
        assert(count > 0 && count < 10);
        memcpy(m_CollisionParameters.data(), param, sizeof(float) * count);
    }
    const bool SceneObjectGeometry::Visible() const { return m_Visible; }
    const bool SceneObjectGeometry::CastShadow() const { return m_Shadow; }
    const bool SceneObjectGeometry::MotionBlur() const { return m_MotionBlur; }
    const SceneObjectCollisionType SceneObjectGeometry::CollisionType() const { return m_CollisionType; }
    const float* SceneObjectGeometry::CollisionParameters() const { return m_CollisionParameters.data(); }

    void SceneObjectGeometry::AddMesh(std::unique_ptr<SceneObjectMesh> mesh, size_t level)
    {
        if (level >= m_MeshesLOD.size()) m_MeshesLOD.resize(level + 1);
        m_MeshesLOD[level].emplace_back(mesh.release());
    }

    const std::vector<std::unique_ptr<SceneObjectMesh>>& SceneObjectGeometry::GetMeshes(size_t lod) const
    {
        return m_MeshesLOD[lod];
    }

    // Class SceneObjectTransform
    // Class SceneObjectTranslation
    SceneObjectTranslation::SceneObjectTranslation(const char axis, const float amount)
    {
        switch (axis)
        {
        case 'x':
            m_Transform = translate(m_Transform, vec3f(amount, 0.0f, 0.0f));
            break;
        case 'y':
            m_Transform = translate(m_Transform, vec3f(0.0f, amount, 0.0f));
            break;
        case 'z':
            m_Transform = translate(m_Transform, vec3f(0.0f, 0.0f, amount));
        default:
            break;
        }
    }

    SceneObjectTranslation::SceneObjectTranslation(const float x, const float y, const float z) 
    {
        m_Transform = translate(m_Transform, vec3f(x, y, z));
    }

    // Class SceneObjectRotation
    SceneObjectRotation::SceneObjectRotation(const char axis, const float theta)
    {
        switch (axis) {
        case 'x':
            m_Transform = rotate(m_Transform, radians(theta), vec3f(1.0f, 0.0f, 0.0f));
            break;
        case 'y':
            m_Transform = rotate(m_Transform, radians(theta), vec3f(0.0f, 1.0f, 0.0f));
            break;
        case 'z':
            m_Transform = rotate(m_Transform, radians(theta), vec3f(0.0f, 0.0f, 1.0f));
            break;
        default:
            break;
        }
    }

    SceneObjectRotation::SceneObjectRotation(vec3f& axis, const float theta)
    {
        axis = normalize(axis);
        m_Transform = rotate(m_Transform, theta, axis);
    }

    SceneObjectRotation::SceneObjectRotation(quatf quaternion) { m_Transform = rotate(m_Transform, quaternion); }

    // Class SceneObjectScale
    SceneObjectScale::SceneObjectScale(const char axis, const float amount) 
    {
        switch (axis) {
        case 'x':
            m_Transform = scale(m_Transform, vec3f(amount, 0.0f, 0.0f));
            break;
        case 'y':
            m_Transform = scale(m_Transform, vec3f(0.0f, amount, 0.0f));
            break;
        case 'z':
            m_Transform = scale(m_Transform, vec3f(0.0f, 0.0f, amount));
            break;
        default:
            break;
        }
    }

    SceneObjectScale::SceneObjectScale(const float x, const float y, const float z) {
        m_Transform = scale(m_Transform, vec3f(x, y, z));
    }


    // ---------------------------- Ostream ------------------------------------------
    // -------------------------------------------------------------------------------

    // SceneObjectVertexArray
    std::ostream& operator<<(std::ostream& out, const SceneObjectVertexArray& obj)
    {
        out << "Attribute:          " << obj.m_Attribute << std::endl;
        out << "Morph Target Index: " << obj.m_MorphTargetIndex << std::endl;
        out << "Data Type:          " << magic_enum::enum_name(obj.m_DataType) << std::endl;
        out << "Data Size:          " << obj.GetDataSize() << " bytes" << std::endl;
        out << "Data Count:         " << obj.GetVertexCount() << std::endl;
        out << "Data:               ";
        const uint8_t* data = obj.m_Data.GetData();
        for (size_t i = 0; i < obj.GetVertexCount(); i++)
        {
            switch (obj.m_DataType) {
            case VertexDataType::Float1:
                std::cout << *(reinterpret_cast<const float*>(data) + i) << " ";
                break;
            case VertexDataType::Float2:
                std::cout << *(reinterpret_cast<const vec2f*>(data) + i) << " ";
                break;
            case VertexDataType::Float3:
                std::cout << *(reinterpret_cast<const vec3f*>(data) + i) << " ";
                break;
            case VertexDataType::Float4:
                std::cout << *(reinterpret_cast<const vec4f*>(data) + i) << " ";
                break;
            case VertexDataType::Double1:
                std::cout << *(reinterpret_cast<const double*>(data) + i) << " ";
                break;
            case VertexDataType::Double2:
                std::cout << *(reinterpret_cast<const Vector<double, 2>*>(data) + i) << " ";
                break;
            case VertexDataType::Double3:
                std::cout << *(reinterpret_cast<const Vector<double, 3>*>(data) + i) << " ";
                break;
            case VertexDataType::Double4:
                std::cout << *(reinterpret_cast<const Vector<double, 4>*>(data) + i) << " ";
                break;
            default:
                break;
            }
        }
        return out << std::endl;
    }

    // SceneObjectIndexArray
    std::ostream& operator<<(std::ostream& out, const SceneObjectIndexArray& obj)
    {
        out << "Restart Index:   " << obj.m_ResetartIndex << std::endl;
        out << "Index Data Type: " << magic_enum::enum_name(obj.m_DataType) << std::endl;
        out << "Data Size:       " << obj.GetDataSize() << std::endl;
        out << "Material ID:     " << obj.GetMaterialID() << std::endl;
        out << "Data:            ";
        auto data = obj.GetData();
        for (size_t i = 0; i < obj.GetIndexCount(); i++) 
        {
            switch (obj.m_DataType) 
            {
            case IndexDataType::Int8:
                out << reinterpret_cast<const int8_t*>(data)[i] << ' ';
                break;
            case IndexDataType::Int16:
                out << reinterpret_cast<const int16_t*>(data)[i] << ' ';
                break;
            case IndexDataType::Int32:
                out << reinterpret_cast<const int32_t*>(data)[i] << ' ';
                break;
            case IndexDataType::Int64:
                out << reinterpret_cast<const int64_t*>(data)[i] << ' ';
                break;
            default:
                break;
            }
        }
        return out << std::endl;
    }

    // Class SceneObjectMesh
    std::ostream& operator<<(std::ostream& out, const SceneObjectMesh& obj)
    {
        out << static_cast<const BaseSceneObject&>(obj) << std::endl;
        out << "Primitive Type: " << magic_enum::enum_name(obj.m_PrimitiveType) << std::endl;

        out << "This mesh contains " << obj.m_VertexArray.size() << " vertex properties." << std::endl;
        for (auto&& v : obj.m_VertexArray)
        {
            out << v << std::endl;
        }

        out << "This mesh contains " << obj.m_IndexArray.size() << " index properties." << std::endl;
        for (auto&& index : obj.m_IndexArray)
        {
            out << index << std::endl;
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const SceneObjectGeometry& obj) 
    {
        out << static_cast<const BaseSceneObject&>(obj) << std::endl;
        for (size_t i = 0; i < obj.m_MeshesLOD.size(); i++) 
        {
            out << "Level: " << i << std::endl;
            for (size_t j = 0; j < obj.m_MeshesLOD[i].size(); j++)
                out << "Mesh[{}]:\n" << i << *obj.m_MeshesLOD[i][j] << std::endl;
        }
        return out << std::endl;
    }

    // Scene Transform
    std::ostream& operator<<(std::ostream& out, const SceneObjectTransform& obj)
    {
        out << "Transform Matrix: " << obj.m_Transform << std::endl;
        out << "Is Object Local:  " << obj.m_SceneObjectOnly;
        return out;
    }
}