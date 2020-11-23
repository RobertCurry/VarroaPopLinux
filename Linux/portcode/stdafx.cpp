#include "stdafx.h"

std::ostream &operator<<(std::ostream &stream, const CString& string)
{
    return stream << string.ToString();
}
