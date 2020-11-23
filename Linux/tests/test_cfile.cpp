#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "cfile.h"

#include "helpers/common.h"

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

// On Windows the newline character is still written \n -> \r\n
// to handle this difference we are going to explicitely add 1 character for each \n
int AdjustedStrlen(const char* str) {
    int count = std::strlen(str);
#ifdef WINDOWS    
    std::for_each(str, str+count, [&count](const char& c)
        {
            if (c == '\n') count++;
        }
    );
#endif
    return count;
}

TEST_CASE("CFile operations", "[port]") {
    
    CStdioFile file (GetFileInTempDirectory("dummy.txt").c_str(), CFile::modeWrite);
    
    const char* fileContent = "This is a single line file\n";
    int fileContentSize = AdjustedStrlen(fileContent);

    file.WriteString(fileContent);
    file.Close();

    SECTION("Read dummy file") {
        
        CStdioFile file (GetFileInTempDirectory("dummy.txt").c_str(), CFile::modeRead);

        CString content;
        file.ReadString(content);
        CHECK(content == "This is a single line file");

        file.Close();
    }

    SECTION("Tell and Seek in read mode") {

        CStdioFile file (GetFileInTempDirectory("dummy.txt").c_str(), CFile::modeRead);

        CHECK(file.GetPosition() == 0);

        file.SeekToBegin();        
        CHECK(file.GetPosition() == 0);
        
        CHECK(file.Seek(0, CFile::end) == fileContentSize);
        CHECK(file.GetPosition() == fileContentSize);
        
        CHECK(file.Seek(10, CFile::begin) == 10);
        CHECK(file.GetPosition() == 10);
        
        CString content;
        file.ReadString(content);
        CHECK(content == "single line file");

        file.Close();
    }

    SECTION("Tell and Seek in write mode") {

        CStdioFile file (GetFileInTempDirectory("dummy.txt").c_str(), CFile::modeWrite);

        CHECK(file.GetPosition() == 0);
        CHECK(file.Seek(10, CFile::begin) == 10);
        CHECK(file.GetPosition() == 10);

        const char* additionalContent = "double single line file";

        file.WriteString(additionalContent);
        CHECK(file.GetPosition() == 10 + AdjustedStrlen(additionalContent));
        
        file.Close();
    }

    SECTION("Tell and Seek in read/write mode") {

        CStdioFile file (GetFileInTempDirectory("dummy.txt").c_str(), CFile::modeReadWrite);

        CString content;
        file.ReadString(content);
        CHECK(content == "This is a single line file"); 

        CHECK(file.GetPosition() == fileContentSize);
        CHECK(file.Seek(0, CFile::end) == fileContentSize);
        CHECK(file.GetPosition() == fileContentSize);

        const char* additionalContent = "This is a second line in the file\n";
        int additionalContentSize = AdjustedStrlen(additionalContent);

        file.WriteString(additionalContent);
        CHECK(file.GetPosition() == fileContentSize + additionalContentSize);

        file.Close();

        SECTION("Iterate on the file content") {
            
            CStdioFile file (GetFileInTempDirectory("dummy.txt").c_str(), CFile::modeRead);

            std::vector<std::string> fileContent = {
                "This is a single line file",
                "This is a second line in the file"
            };

            size_t lineIt = 0;
            
            CString content;
            while(file.ReadString(content)) {

                CHECK(content == fileContent[lineIt]);
                lineIt++;
            }

            CHECK(lineIt == 2);

            file.Close();
        }

        SECTION("Get file status") {

            CStdioFile file;
            CFileException error;

            CHECK(file.Open(GetFileInTempDirectory("dummy.txt").c_str(), CFile::modeRead, &error));
            CHECK(file.GetFileName() == GetFileInTempDirectory("dummy.txt"));

            CFileStatus status;
            file.GetStatus(status);
            CHECK(status.m_size == fileContentSize + additionalContentSize);
            
            file.Close();
        }

        SECTION("Get file exception") {

            CStdioFile file;
            CFileException error;

            CHECK_FALSE(file.Open(GetFileInTempDirectory("dummy-not-exists.txt").c_str(), CFile::modeRead, &error));
            CHECK(file.GetFileName() == "");

            char buffer[12];
            CHECK(error.GetErrorMessage(buffer, sizeof(buffer)));
            
            file.Close();
        }

        SECTION("Rename") {

            CStdioFile::Rename(GetFileInTempDirectory("dummy.txt"), GetFileInTempDirectory("dummy-rename.txt"));

            CStdioFile file;
            CFileException error;

            CHECK_FALSE(file.Open(GetFileInTempDirectory("dummy.txt").c_str(), CFile::modeRead, &error));
            CHECK(file.GetFileName() == "");

            file.Close();

            CHECK(file.Open(GetFileInTempDirectory("dummy-rename.txt").c_str(), CFile::modeRead, &error));
            CHECK(file.GetFileName() == GetFileInTempDirectory("dummy-rename.txt"));
            
            file.Close();
        }
    }
}
