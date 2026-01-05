#include "ObjectBase.h"

namespace SimEngine
{
    ObjectBase::ObjectBase(ObjectBase* parent, const std::string& name)
        : parent(parent)
        , name(name)
    {
    }

    ObjectBase::~ObjectBase()
    {
    }
}
