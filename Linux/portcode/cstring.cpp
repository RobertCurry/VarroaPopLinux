#include "cstring.h"

#include "compat.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <boost/tokenizer.hpp>

#include <cassert>
#include <limits>

namespace ba = boost::algorithm;

CString::CString() : m_data()
{
}

CString::CString(const std::string& str) : m_data(str)
{
}

CString::CString(const char* cStr) : m_data(cStr)
{
}

bool CString::operator==(const CString& str) const
{
	return m_data == str.m_data;
}

bool CString::operator==(const char* str) const
{
	return m_data == str;
}

bool CString::operator!=(const CString& str) const
{
	return m_data != str.m_data;
}

bool CString::operator!=(const char* str) const
{
	return m_data != str;
}

char& CString::operator[](const size_t& index)
{
	return m_data[index];
}

const char& CString::operator[](const size_t& index) const
{
	return m_data[index];
}

CString& CString::operator+=(const CString& str)
{
	if (&str != this)
	{
		m_data += str.m_data;
	}
	return *this;
}

CString& CString::operator=(const CString& str)
{
	if (&str != this)
	{
		m_data = str.m_data;
	}
	return *this;
}

CString& CString::operator+=(const char& c)
{
	m_data += c;
	return *this;
}

bool CString::operator<(const CString& str) const
{
	return m_data < str.m_data;
}

const std::string& CString::ToString() const
{
	return m_data;
}

CString::operator const char*() const
{
	return m_data.c_str();
}

int CString::GetLength() const
{
	return static_cast<int>(m_data.length());
}

CString& CString::MakeLower()
{
	ba::to_lower(m_data);
	return *this;
}

CString& CString::MakeUpper()
{
	ba::to_upper(m_data);
	return *this;
}

void CString::Trim()
{
	ba::trim(m_data);
}

void CString::TrimLeft()
{
	ba::trim_left(m_data);
}

void CString::TrimRight()
{
	ba::trim_right(m_data);
}

int CString::Find(char element) const
{
	size_t position = m_data.find(element);
	return (position != std::string::npos)? static_cast<int>(position) : -1;
}

int CString::ReverseFind(char element) const
{
	size_t position = m_data.rfind(element);
	return (position != std::string::npos)? static_cast<int>(position) : -1;
}

int CString::Find(const char* str) const
{
	size_t position = m_data.find(str);
	return (position != std::string::npos)? static_cast<int>(position) : -1;
}

int CString::ReverseFind(const char* str) const
{
	size_t position = m_data.rfind(str);
	return (position != std::string::npos)? static_cast<int>(position) : -1;
}

void CString::Replace(const CString& toReplace, const CString& with)
{
	std::string search = toReplace.ToString();
	std::string format = with.ToString();
	ba::replace_all(m_data, search, format);
}

CString CString::Left(int count) const
{
    // make sure that during size_t to int conversion we don't loose data
	assert(std::numeric_limits<int>::max() > m_data.length());
    auto length = static_cast<int>(m_data.length());

	count = std::clamp(count, 0, length);
	return m_data.substr(0, count);
}

CString CString::Right(int count) const
{
    // make sure that during size_t to int conversion we don't loose data
    assert(std::numeric_limits<int>::max() > m_data.length());
    auto length = static_cast<int>(m_data.length());

	count = std::clamp(count, 0, length);
	return m_data.substr(length - count);
}

CString CString::Mid(int first) const
{
    // make sure that during size_t to int conversion we don't loose data
    assert(std::numeric_limits<int>::max() > m_data.length());
    auto length = static_cast<int>(m_data.length());

	first = std::clamp(first, 0, length);
	return m_data.substr(first);
}

CString CString::Mid(int first, int count) const
{
    // make sure that during size_t to int conversion we don't loose data
    assert(std::numeric_limits<int>::max() > m_data.length());
    auto length = static_cast<int>(m_data.length());

	first = std::clamp(first, 0, length);
	count = std::clamp(count, 0, length);
	return m_data.substr(first, count);
}

CString CString::Tokenize(const char* delimiter, int& startPosition) const
{
    CString cResult;
    std::string toTokenize = m_data.substr(startPosition);
    boost::char_separator<char> sep(delimiter);
    boost::tokenizer<boost::char_separator<char>> tokens(toTokenize, sep);
    if (tokens.begin() != tokens.end())
    {
        std::string result = *tokens.begin();

        // update startPosition
        auto positionInToTokenize = toTokenize.find(result);
        startPosition += positionInToTokenize + result.length();

        cResult = result;
    }
    return cResult;
}

CString CString::SpanExcluding(const char* delimiter) const
{

    CString cResult;
    boost::char_separator<char> sep(delimiter);
    boost::tokenizer<boost::char_separator<char>> tokens(m_data, sep);
    if (tokens.begin() != tokens.end())
    {
        std::string result = *tokens.begin();
        cResult = result;
    }
    return cResult;
}

CString operator+(const CString& str1, const CString& str2)
{
    CString cStr1(str1);
	cStr1 += str2;
    return cStr1;
}

CString operator+(const CString& str1, const char* str2)
{
    CString cStr1(str1);
	cStr1 += str2;
    return cStr1;
}

CString operator+(const char* str1, const CString& str2)
{
    CString cStr1(str1);
	cStr1 += str2;
    return cStr1;
}
