/* Copyright (C) 2013 Rainmeter Project Developers
 *
 * This Source Code Form is subject to the terms of the GNU General Public
 * License; either version 2 of the License, or (at your option) any later
 * version. If a copy of the GPL was not distributed with this file, You can
 * obtain one at <https://www.gnu.org/licenses/gpl-2.0.html>. */

#include "PathUtil.h"
#include "UnitTest.h"
#include <Shlobj.h>

namespace PathUtil {

TEST_CLASS(Common_PathUtil_Test)
{
public:
	TEST_METHOD(TestIsSeparator)
	{
		Assert::IsTrue(IsSeparator(L'\\'));
		Assert::IsTrue(IsSeparator(L'/'));
		Assert::IsFalse(IsSeparator(L'.'));
	}

	TEST_METHOD(TestIsDotOrDotDot)
	{
		Assert::IsTrue(IsDotOrDotDot(L"."));
		Assert::IsTrue(IsDotOrDotDot(L".."));
		Assert::IsFalse(IsDotOrDotDot(L"..."));
		Assert::IsFalse(IsDotOrDotDot(L""));
	}

	TEST_METHOD(TestIsUNC)
	{
		Assert::IsTrue(IsUNC(L"\\\\server"));
		Assert::IsTrue(IsUNC(L"//server"));
	}

	TEST_METHOD(TestIsAbsolute)
	{
		Assert::IsTrue(IsAbsolute(L"\\\\server"));
		Assert::IsTrue(IsAbsolute(L"C:\\test"));
		Assert::IsTrue(IsAbsolute(L"C:/test"));
		Assert::IsFalse(IsAbsolute(L"C:"));
	}

	TEST_METHOD(TestAppendBackslashIfMissing)
	{
		std::wstring path;
		AppendBackslashIfMissing(path);
		Assert::IsTrue(path.empty());

		std::wstring path2 = L"C:\\test";
		AppendBackslashIfMissing(path2);
		Assert::IsTrue(path2 == L"C:\\test\\");

		std::wstring path3 = L"C:\\test\\";
		AppendBackslashIfMissing(path3);
		Assert::IsTrue(path3 == L"C:\\test\\");
	}

	TEST_METHOD(TestGetFolderFromFilePath)
	{
		Assert::IsTrue(GetFolderFromFilePath(L"C:\\test.txt") == L"C:\\");
	}

	TEST_METHOD(TestGetVolume)
	{
		Assert::IsTrue(GetVolume(L"C:\\test.txt") == L"C:");
		Assert::IsTrue(GetVolume(L"\\\\server\\share\\") == L"\\\\server\\share");
		Assert::IsTrue(GetVolume(L"\\\\server\\C:\\path\\") == L"\\\\server\\C:");
	}

	TEST_METHOD(TestExpandEnvironmentVariables)
	{
		WCHAR appdataPath[MAX_PATH];
		SHGetFolderPath(nullptr, CSIDL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, appdataPath);

		WCHAR windirPath[MAX_PATH];
		ExpandEnvironmentStrings(L"%WINDIR%", windirPath, MAX_PATH);
		
		std::wstring test = L"%APPDATA%";
		PathUtil::ExpandEnvironmentVariables(test);
		Assert::IsTrue(test == appdataPath);

		std::wstring test2 = L"%APPDATA%%WINDIR%";
		PathUtil::ExpandEnvironmentVariables(test2);
		Assert::IsTrue(test2 == ((std::wstring)appdataPath + windirPath));

		std::wstring test3 = L"%APPDATA%%WINDIR%%APPDATA%";
		PathUtil::ExpandEnvironmentVariables(test3);
		Assert::IsTrue(test3 == ((std::wstring)appdataPath + windirPath + appdataPath));
	}
};

}  // namespace PathUtil
