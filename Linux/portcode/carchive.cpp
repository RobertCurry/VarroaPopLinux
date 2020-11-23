#include "carchive.h"

#include "stdafx.h"

CArchive::CArchive(CFile* pFile, UINT nMode)
: mStream(pFile->GetStream())
, mModeMask(nMode)
{
    
}

CArchive::~CArchive()
{

}
    
bool CArchive::IsLoading() const
{
    return (mModeMask & load) != 0;
}

bool CArchive::IsStoring() const
{
    return (mModeMask & store) != 0;;
}

template<>
void CArchive::operator<<(const COleDateTime& date)
{
    // CArchive& AFXAPI operator<<(CArchive& ar, COleDateTime dateSrc)
    // {
    // 	ar << (long)dateSrc.m_status;
    // 	return ar << dateSrc.m_dt;
    // }
    int32_t status = static_cast<int32_t>(date.GetStatus());
    DATE dt;
    date.GetAsDATE(dt);

    mStream.write((const char*)&status, sizeof(int32_t));
    mStream.write((const char*)&dt, sizeof(DATE));
}

// The following implementation is coming from arccore.cpp
void AfxWriteStringLength(CArchive& ar, UINT_PTR nLength)
{
    if (nLength < 255)
    {
        ar<<(BYTE)nLength;
    }
    else if (nLength < 0xfffe)
    {
        ar<<(BYTE)0xff;
        ar<<(WORD)nLength;
    }
    else if (nLength < 0xffffffff)
    {
        ar<<(BYTE)0xff;
        ar<<(WORD)0xffff;
        ar<<(DWORD)nLength;
    }
    else
    {
        ar<<(BYTE)0xff;
        ar<<(WORD)0xffff;
        ar<<(DWORD)0xffffffff;
        ar<<(ULONGLONG)nLength;
    }
}

// The following implementation is coming from arccore.cpp
UINT_PTR AfxReadStringLength(CArchive& ar)
{
    UINT_PTR nLength;
    BYTE b;
    ar >> b;
    if (b == 0xff)
    {
        WORD w;
        ar >> w;
        if (w == 0xffff)
        {
            DWORD dw;
            ar >> dw;
            if (dw == 0xffffffff)
            {
                ULONGLONG ulong;
                ar >> ulong;
                nLength = ulong;
            }
            else
            {
                nLength = dw;
            }
        }
        else
        {
            nLength = w;
        }
    }
    else
    {
        nLength = b;
    }
    return nLength;
}

template<>
void CArchive::operator<<(const CString& str)
{
    int strLength = str.GetLength();
    AfxWriteStringLength(*this, strLength);
    mStream.write((const char*)str.ToString().c_str(), sizeof(char)*strLength);
}

template<>
void CArchive::operator<<(const CTime& time)
{
    std::int64_t timeInt64 = time.GetAsTimeT();
    mStream.write((const char*)&timeInt64, sizeof(std::int64_t));
}

template<>
void CArchive::operator>>(COleDateTime& date)
{
    int32_t status = 0;
    mStream.read((char*)&status, sizeof(int32_t));
    
    double dt = 0.0;
    mStream.read((char*)&dt, sizeof(double));
    date = COleDateTime(dt);
}

template<>
void CArchive::operator>>(CString& str)
{
    int strLength = AfxReadStringLength(*this);

    char* buffer = new char[strLength+1];
    buffer[strLength] = '\0';
    mStream.read(buffer, strLength);
    str = buffer;
    delete[] buffer;
}

template<>
void CArchive::operator>>(CTime& time)
{
    std::int64_t timeInt64;
    mStream.read((char*)&timeInt64, sizeof(std::int64_t));
    time.FromTimeT(timeInt64);
}

DWORD_PTR CArchive::ReadCount()
{
	WORD wCount;
	*this >> wCount;
	if (wCount != 0xFFFF)
		return wCount;

	DWORD dwCount;
	*this >> dwCount;
#ifndef _WIN64
	return dwCount;
#else  // _WIN64
	if (dwCount != 0xFFFFFFFF)
		return dwCount;

	DWORD_PTR qwCount;
	*this >> qwCount;
	return qwCount;
#endif  // _WIN64
}

void CArchive::WriteCount(DWORD_PTR dwCount)
{
	if (dwCount < 0xFFFF)
		*this << (WORD)dwCount;  // 16-bit count
	else
	{
		*this << (WORD)0xFFFF;
#ifndef _WIN64
		*this << (DWORD)dwCount;  // 32-bit count
#else  // _WIN64
		if (dwCount < 0xFFFFFFFF)
			*this << (DWORD)dwCount;  // 32-bit count
		else
		{
			*this << (DWORD)0xFFFFFFFF;
			*this << dwCount;
		}
#endif  // _WIN64
	}
}

UINT CArchive::Read(void *lpBuf, UINT nMax)
{
    size_t read = mStream.readsome((char *)&lpBuf, nMax);
    return static_cast<UINT>(read);
}

void CArchive::Write(const void *lpBuf, UINT nMax)
{
    mStream.write((const char *)&lpBuf, nMax);
}