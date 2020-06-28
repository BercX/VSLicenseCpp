#include "VSLicense.h"

namespace utility
{
	std::tm AddDaysToCurrentDate(int add_days)
	{
		tm newtime;
		time_t now = time(0);
		localtime_s(&newtime, &now);

		newtime.tm_mday += add_days;

		mktime(&newtime);

		return newtime;
	}

	void ShortToByte(signed short num, byte* bytes, int offset) {
		bytes[offset] = num & 0xFF;
		bytes[offset + 1] = (num >> 8) & 0xFF;
	}

	short ByteToShort(byte* bytes, int offset) {
		return (bytes[offset + 1] << 8) | (bytes[offset] & 0xff);
	}

	void ConvertToByteDate(byte* date, std::tm tm)
	{
		ShortToByte(tm.tm_year + 1900, date, 0);
		ShortToByte(tm.tm_mon + 1, date, 2);
		ShortToByte(tm.tm_mday, date, 4);
	}

	std::tm ConvertFromByteDate(byte* bytes)
	{
		short year = ByteToShort(bytes, 0);
		short month = ByteToShort(bytes, 2);
		short day = ByteToShort(bytes, 4);

		std::tm tm = { /* .tm_sec  = */ 0,
					   /* .tm_min  = */ 0,
					   /* .tm_hour = */ 0,
					   /* .tm_mday = */ day,
					   /* .tm_mon  = */ month - 1,
					   /* .tm_year = */ year - 1900,
		};

		return tm;
	}
}

std::vector<byte> VSLicense::Encrypt(std::vector<byte> v_in) {
	DATA_BLOB in;
	DATA_BLOB out;

	in.pbData = v_in.data();
	in.cbData = v_in.size();

	if (CryptProtectData(&in, NULL, NULL, NULL, NULL, 0, &out)) {
		std::vector<byte> v_out(out.pbData, out.pbData + out.cbData);
		return v_out;
	}

	return std::vector<byte>();
}

std::vector<byte> VSLicense::Decrypt(std::vector<byte> v_in) {
	DATA_BLOB in;
	DATA_BLOB out;

	in.pbData = v_in.data();
	in.cbData = v_in.size();

	if (CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, &out)) {
		std::vector<byte> v_out(out.pbData, out.pbData + out.cbData);
		return v_out;
	}

	return std::vector<byte>();
}

std::vector<byte> VSLicense::ReadFromRegistry()
{
	/**
	* Read encrypted array of bytes from registry
	*/
	byte value[1024];
	DWORD buffer_size = sizeof(value);
	if (RegGetValue(HKEY_CLASSES_ROOT, kVSLicenseMap[version], 0, RRF_RT_ANY, NULL, (PVOID)&value, &buffer_size) != ERROR_SUCCESS)
	{
		wprintf(L"Can't get registry value for %s!\n", version.c_str());
		exit(EXIT_FAILURE);
	}

	return std::vector<byte>(value, value + buffer_size);
}

void VSLicense::WriteToRegistry(std::vector<byte> bytes)
{
	/**
	 * Write encrypted array of bytes to registry
	 */
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, kVSLicenseMap[version], NULL, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
	{
		wprintf(L"Can't open registry!\n");
		exit(EXIT_FAILURE);
	}

	if (RegSetValueEx(hKey, NULL, 0, REG_BINARY, (BYTE*)bytes.data(), bytes.size()) != ERROR_SUCCESS)
	{
		wprintf(L"Can't write to registry!\n");
		exit(EXIT_FAILURE);
	}
	RegCloseKey(hKey);
}

void VSLicense::PrintExpirationDate()
{
	/**
	 * Print Visual Studio license expiration date on screen.
	 */
	char buff[128] = {};
	asctime_s(buff, sizeof(buff), &tm);
	std::wcout << buff << std::endl;
}

void VSLicense::GetExpirationDate()
{
	/**
	 * Get Visual Studio license expiration date from registry.
	 */
	encrypted = ReadFromRegistry();

	decrypted = Decrypt(encrypted);

	// copy date bytes to separate array
	byte date[6];
	std::copy(decrypted.begin() + decrypted.size() - 16, decrypted.begin() + decrypted.size() - 10, date);

	tm = utility::ConvertFromByteDate(date);
}

void VSLicense::SetExpirationDate(int add_days)
{
	/**
	* Add days to current date and save value to registry.
	*/
	tm = utility::AddDaysToCurrentDate(add_days);

	byte date[6];
	utility::ConvertToByteDate(date, tm);

	int date_offset = decrypted.size() - 16;
	for (int i = 0; i < 6; i++)
	{
		decrypted[date_offset + i] = date[i];
	}

	encrypted = Encrypt(decrypted);

	WriteToRegistry(encrypted);
}