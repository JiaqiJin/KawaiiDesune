#pragma once

#include "../Math/KawaiiMath.h"
#include "../Core/Buffer.h"
#include "../AssertManager/Image.h"

#include <crossguid/guid.hpp>
#include <filesystem>
#include <magic_enum.hpp>

#include "../Utility/SceneObjectTypeDef.h"

/*
* GUID : Resource Manager System store diff version of resources and also stores the relationships between resources.
* There needs to be a mapping relationship between this external system and the scene objects inside our program.
*/

namespace Kawaii::Core
{
    class BaseSceneObject 
    {
    protected:
        // Global Unique ID
        xg::Guid        m_Guid;
        // Scene Obj Type
        SceneObjectType m_Type;

        // Constructors
        // can only be used as base class
        BaseSceneObject::BaseSceneObject(SceneObjectType type)
            : m_Type(type) { m_Guid = xg::newGuid(); }

        BaseSceneObject::BaseSceneObject(const xg::Guid& guid, const SceneObjectType& type) 
            : m_Guid(guid), m_Type(type) {}

        BaseSceneObject::BaseSceneObject(xg::Guid&& guid, const SceneObjectType& type)
            : m_Guid(std::move(guid)), m_Type(type) {}

        BaseSceneObject::BaseSceneObject(BaseSceneObject&& obj) 
            : m_Guid(std::move(obj.m_Guid)), m_Type(obj.m_Type) {}

        BaseSceneObject& BaseSceneObject::operator=(BaseSceneObject&& obj)
        {
            if (this != &obj) 
            {
                m_Guid = std::move(obj.m_Guid);
                m_Type = obj.m_Type;
            }
            return *this;
        }

        virtual ~BaseSceneObject() = default;

    public:
        // no default constructor
        BaseSceneObject() = default;
        // can not be copied
        BaseSceneObject(const BaseSceneObject& obj) = default;
        BaseSceneObject& operator=(const BaseSceneObject& obj) = default;

    public:
        const xg::Guid& GetGuid() const { return m_Guid; }
        const SceneObjectType GetType() const { return m_Type; }

        friend std::ostream& operator<<(std::ostream& out, const BaseSceneObject& obj)
        {
            out << "GUID: " << obj.m_Guid << std::dec << std::endl;
            out << "Type: " << magic_enum::enum_name(obj.m_Type);
            return out;
        }
    };
}