#include "varroapop.h"

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

bool gl_RunGUI = false;

int MyMessageBox( LPCTSTR lpszText, UINT nType, UINT nIDHelp )
{
    NOT_IMPLEMENTED();
    return -1;
}

CString SplitPath(CString PathString, PELEMENT PathElement)
{
	bfs::path path(PathString.ToString());
	std::string result;
	switch (PathElement)
	{
	case DRV:
		result = path.root_name().string();
		break;
	case DIR:
		result = path.parent_path().string() + "/"; // add slash at the end for Windows compatibility
		break;
	case FNAME:
		result = path.stem().string();
		break;
	case EXT:
		result = path.extension().string();
		break;
	default: 
		NOT_IMPLEMENTED();
	}
    return CString(result);
}
