#pragma once
#include <windows.h>
#include <map>
#include <vector>
#include <iostream>
#include <ctime>

#pragma comment(lib, "Crypt32")

class VSLicense
{
public:
	VSLicense(std::wstring version)
	{
		is_version_supported = kVSLicenseMap.count(version);

		if(is_version_supported)
		{
			this->version = version;
			this->GetExpirationDate();
		}
	}

	void PrintExpirationDate();
	void GetExpirationDate();
	void SetExpirationDate(int add_days);

	bool IsVersionSupported() { return is_version_supported; };

private:
	std::vector<byte> Encrypt(std::vector<byte> v_in);
	std::vector<byte> Decrypt(std::vector<byte> v_in);
	std::vector<byte> ReadFromRegistry();
	void WriteToRegistry(std::vector<byte> bytes);

	std::map <std::wstring, LPCWSTR> kVSLicenseMap =
	{
		{L"VS2015", L"\\Licenses\\4D8CFBCB-2F6A-4AD2-BABF-10E28F6F2C8F\\07078"},
		{L"VS2017", L"\\Licenses\\5C505A59-E312-4B89-9508-E162F8150517\\08878"},
		{L"VS2019", L"\\Licenses\\41717607-F34E-432C-A138-A3CFD7E25CDA\\09278"}
	};

	std::wstring version;
	bool is_version_supported;
	
	std::vector<byte> encrypted;
	std::vector<byte> decrypted;

	std::tm tm;
};
