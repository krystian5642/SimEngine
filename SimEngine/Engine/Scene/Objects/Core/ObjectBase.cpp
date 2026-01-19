#include "ObjectBase.h"

ObjectBase::ObjectBase(ObjectBase* parent, const std::string& name)
    : parent(parent)
    , name(name)
{
}
