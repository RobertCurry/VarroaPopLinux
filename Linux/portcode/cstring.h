#pragma once
#ifndef CSTRING_CUSTOM_H
#define CSTRING_CUSTOM_H

#include <string>

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CString
{
public:
    
	CString();
	CString(const std::string& cStr);
    CString(const char* cStr);

	bool operator==(const CString& str) const;
	bool operator==(const char* str) const;

	bool operator!=(const CString& str) const;
	bool operator!=(const char* str) const;

	char& operator[](const size_t& index);
	const char& operator[](const size_t& index) const;

	CString& operator+=(const CString& str);
	CString& operator=(const CString& str);
	
	CString& operator+=(const char& c);

	bool operator<(const CString& str) const;

	const std::string& ToString() const;

	operator const char* () const;

	int GetLength() const;

	CString& MakeLower();
	CString& MakeUpper();

	void Trim();
	void TrimLeft();
	void TrimRight();
	
	template<typename... Args>
	void Format(const char* format, Args... args);

	// Find the first occurence of element 
	// return -1 if not found and index otherwise
	int Find(char element) const;

	// Find the last occurence of element
	// return -1 if not found and index otherwise
	int ReverseFind(char element) const;
	
	// Find the first occurence of str 
	// return -1 if not found and index otherwise
	int Find(const char* str) const;

	// Find the last occurence of str
	// return -1 if not found and index otherwise
	int ReverseFind(const char* str) const;

	void Replace(const CString& toReplace, const CString& with);

	// Return the substring consisting of the leftmost 'count' characters
	CString Left(int count) const;

	// Return the substring consisting of the rightmost 'count' characters
	CString Right(int count) const;

	// Return the substring starting at index 'first'
	CString Mid(int first) const;

	// Return the substring starting at index 'first', with length 'count'
	CString Mid(int first, int count) const;

	CString Tokenize(const char* delimiter, int& startPosition) const;

	CString SpanExcluding(const char* delimiter) const;

protected:

	std::string m_data;
}; 

CString operator+(const CString& str1, const CString& str2);
CString operator+(const CString& str1, const char* str2);
CString operator+(const char* str1, const CString& str2);

#endif // CSTRING_CUSTOM_H
