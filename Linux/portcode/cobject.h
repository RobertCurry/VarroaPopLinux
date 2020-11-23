#pragma once
#ifndef COBJECT_CUSTOM_H
#define COBJECT_CUSTOM_H

class CArchive;

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CObject
{
public:
	virtual ~CObject() {}
	virtual void Serialize(CArchive& ar);
};

#endif // COBJECT_CUSTOM_H
