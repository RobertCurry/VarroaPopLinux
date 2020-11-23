#include "cfile.h"

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;
namespace bs = boost::system;

#include <iostream>

bool CFile::IsValid() const
{
    return m_fileStream.good();
}

void CStdioFile::Rename(const CString& original, const CString& target)
{
    bfs::rename(original.ToString(), target.ToString());
}

CStdioFile::CStdioFile()
{
}

CStdioFile::CStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{
    m_fileStream.open(lpszFileName, static_cast<std::ios_base::openmode>(nOpenFlags));
    m_fileName = lpszFileName;
}

BOOL CStdioFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
    m_fileStream.open(lpszFileName, static_cast<std::ios_base::openmode>(nOpenFlags));
    
    if (!m_fileStream.fail())
    {
        m_fileName = lpszFileName;
    }
    else
    {
        CString message;
        message.Format("Cannot open file %s", lpszFileName);
        pError->SetErrorMessage(message.ToString());
    }
    return m_fileStream.is_open();
}

BOOL CStdioFile::ReadString(CString& str)
{
    std::string data;
    if (IsValid())
    {
        std::getline(m_fileStream, data);
    }
    str = data;
    return IsValid();
}

void CStdioFile::WriteString(LPCTSTR str)
{
    std::string lStr (str);
    m_fileStream.write(lStr.c_str(), lStr.length());
}

CString CStdioFile::GetFileName() const
{
    return CString(m_fileName);
}

void CStdioFile::Close()
{
    if (m_fileStream.is_open())
    {
        m_fileStream.close();
    }
    m_fileStream.clear();
}

void CStdioFile::SeekToBegin()
{
    m_fileStream.seekp(0);
}
    
ULONGLONG CStdioFile::Seek(LONGLONG lOff, UINT nFrom)
{
    m_fileStream.seekp(lOff, static_cast<std::ios_base::seekdir>(nFrom));
    return m_fileStream.tellp();
}

ULONGLONG CStdioFile::GetPosition()
{
    return m_fileStream.tellp();
}

void CStdioFile::GetStatus(CFileStatus& status) const
{
    // If we need more status information about the file than the size
    // auto fileStatus = bfs::status(m_fileName);
    bs::error_code ec;
    status.m_size = bfs::file_size(m_fileName, ec);
}

BOOL CFileException::GetErrorMessage(LPTSTR buffer, UINT bufferSize) const
{
    bool hasErrorMessage = !m_message.empty();
    if (hasErrorMessage)
    {
        snprintf(buffer, bufferSize, "%s", m_message.c_str());
    }
    return hasErrorMessage;
}

void CFileException::SetErrorMessage(const std::string& message)
{
    m_message = message;
}

