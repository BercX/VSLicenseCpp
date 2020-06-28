#include <windows.h>
#include <vector>
#include <cxxopts.hpp>
#include <codecvt>
#include "VSLicense.h"

int main(int argc, char** argv)
{
	try
	{
		cxxopts::Options options("VSLicenseCpp", "Get and set Visual Studio Community Edition license expiration date.");

		bool get_, set_;
		std::string version;
		int add_days = 30;

		options.
			add_options()
			("get", "Get Visual Studio expiration date", cxxopts::value<bool>(get_)->default_value("false"))
			("set", "Set Visual Studio expiration date", cxxopts::value<bool>(set_)->default_value("false"))
			("version", "Visual Studio version", cxxopts::value<std::string>(version))
			("add_days", "Add days to current date", cxxopts::value<int>(add_days), "N")
			;

		auto result = options.parse(argc, argv);

		// convert from string to wstring
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wversion = converter.from_bytes(version);

		VSLicense* license = new VSLicense(wversion);
		
		if (license->IsVersionSupported())
		{
			if (get_)
			{
				license->PrintExpirationDate();
			} else if (set_)
			{
				license->SetExpirationDate(add_days);
			}
		}
		else
		{
			wprintf(L"Version not supported!\nAvailable versions: VS2015, VS2017, VS2019\nExample: VSLicenseCpp.exe --version=VS2019 --get");
		}
	}
	catch (const cxxopts::OptionException & e)
	{
		std::cout << "error parsing options: " << e.what() << std::endl;
		exit(1);
	}

	return 0;
}