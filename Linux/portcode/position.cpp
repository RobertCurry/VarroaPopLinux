#include "position.h"

POSITION::POSITION() : m_position(nullptr)
{
}

POSITION::POSITION(const POSITION &other)
{
    if (other.m_position != nullptr)
    {
        m_position.reset(other.m_position->copy());
    }
}

POSITION::POSITION(const POSITION_PTR &other) : m_position()
{
    if (other != nullptr)
    {
        m_position.reset(other->copy());
    }
}

POSITION::POSITION(const std::nullptr_t &other)
    : m_position(other)
{
}

POSITION &POSITION::operator=(const POSITION &other)
{
    if (this != &other)
    {
        POSITION_PTR::pointer value = other.m_position ? other.m_position->copy() : nullptr;
        m_position.reset(value);
    }
    return *this;
}

POSITION &POSITION::operator=(const POSITION_PTR &other)
{
    POSITION_PTR::pointer value = other ? other->copy() : nullptr;
    m_position.reset(value);
    return *this;
}

POSITION &POSITION::operator=(const std::nullptr_t &other)
{
    m_position.reset(other);
    return *this;
}

bool POSITION::operator==(const POSITION &other) const
{
    return m_position == other.m_position;
}

bool POSITION::operator!=(const POSITION &other) const
{
    return m_position != other.m_position;
}