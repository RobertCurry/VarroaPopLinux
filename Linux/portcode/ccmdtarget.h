#pragma once
#ifndef CCMDTARGET_CUSTOM_H
#define CCMDTARGET_CUSTOM_H

#include "cobject.h"

/**
 * Empty class to be able to compile the VarroaPop application
 */
class CCmdTarget : public CObject
{
public:
	virtual ~CCmdTarget() {}
};

#endif // CCMDTARGET_CUSTOM_H
