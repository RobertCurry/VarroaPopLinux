#pragma once
#ifndef CARCHIVE_CUSTOM_H
#define CARCHIVE_CUSTOM_H

#include <cstdint>
#include <istream>

class CFile;
class COleDateTime;
class CString;
class CTime;

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CArchive
{
public:

	enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };

	CArchive(CFile* pFile, UINT nMode);
	~CArchive();

	bool IsLoading() const;
	bool IsStoring() const;

    // Storing

	template<typename Type>
	void operator<<(const Type& value)
	{
		mStream.write((char*)&value, sizeof(value));
	}

    // Loading

	template<typename Type>
	void operator>>(Type& value)
	{
		mStream.read((char*)&value, sizeof(value));
	}
	
	// special functions for reading and writing (16-bit compatible) counts
	DWORD_PTR ReadCount();
	void WriteCount(DWORD_PTR dwCount);
	
	// raw methods to read / write blob of data
	UINT Read(void* lpBuf, UINT nMax);
	void Write(const void* lpBuf, UINT nMax);

protected:

	UINT mModeMask = load;
	std::iostream& mStream;
};

// Specializations

template <> void CArchive::operator<<(const CString &value);
template <> void CArchive::operator>>(CString &value);

template <> void CArchive::operator<<(const COleDateTime &value);
template <> void CArchive::operator>>(COleDateTime &value);

template <> void CArchive::operator<<(const CTime &value);
template <> void CArchive::operator>>(CTime &value);

#endif // CARCHIVE_CUSTOM_H
