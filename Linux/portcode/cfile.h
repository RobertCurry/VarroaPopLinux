#pragma once
#ifndef CFILE_CUSTOM_H
#define CFILE_CUSTOM_H

#include "cstring.h"

#include <fstream>

struct CFileStatus
{
	ULONGLONG m_size; // size in Bytes
};

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CFile
{
public:

	// Flag values
	enum OpenFlags 
	{
		modeRead = std::ios_base::in,
		modeWrite = std::ios_base::out,
		modeReadWrite = modeRead | modeWrite,
		modeCreate = 0, // default is C++
		modeNoTruncate = std::ios_base::app,
		typeText = 0, // used in derived classes (e.g. CStdioFile) only
		typeBinary = std::ios_base::binary, // used in derived classes (e.g. CStdioFile) only

		// shared access to file will be defaulted to C++ standard behavior which is shareDenyNone
		shareCompat = 0, 
		shareExclusive = 0,
		shareDenyWrite = 0,
		shareDenyRead = 0,
		shareDenyNone = 0
	};
    
    enum SeekPosition 
	{ 
		begin = std::ios_base::beg, 
		current = std::ios_base::cur, 
		end = std::ios_base::end 
	};

	friend class CArchive;

protected:

	const std::fstream& GetStream() const {return m_fileStream;}
	std::fstream& GetStream() {return m_fileStream;}

	// Returns true if the stream is valid
	bool IsValid() const;

protected:

	std::fstream m_fileStream;
	std::string m_fileName;
};

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CFileException
{
public:
	BOOL GetErrorMessage(LPTSTR buffer, UINT bufferSize) const;

	void SetErrorMessage(const std::string& message);

protected:
	std::string m_message;
};

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CStdioFile : public CFile
{
public:
	static void Rename(const CString& original, const CString& target);

	CStdioFile();
	CStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags);

	BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError);

	void GetStatus(CFileStatus& status) const;

	BOOL ReadString(CString& str);
	
	void WriteString(LPCTSTR str);

	CString GetFileName() const;
	
	void Close();

	void SeekToBegin();

    ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
		
	// Here we removed the constness of the method since in std c++ the rdstate of the stream
	// will be change is tellg fails
	ULONGLONG GetPosition();
};

#endif // CFILE_CUSTOM_H
