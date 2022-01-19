#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <locale>
#include <codecvt>
#include <Windows.h>

namespace WinReg
{
	enum class eHKey
	{
		  eHkeyNotDefined		= 0
		, eHKeyClassesRoot		= 1
		, eHKeyCurrentConfig	= 2
		, eHKeyCurrentUser		= 3
		, eHKeyLocalMachine		= 4
		, eHKeyUsers			= 5
	};

	enum class eRegAccessRights
	{
		  eAccessKeyAllAccess = 0xF003F
		, eAccessKeyCreateSubKey = 0x0004
		, eAccessKeyEnumerateSubKeys = 0x0008
		, eAccessKeynotify = 0x0010
		, eAccessKeyQueryValue = 0x0001
		, eAccessKeyRead = 0x20019
		, eAccessKeySetValue = 0x0002
		, eAccessKeyWrite = 0x20006
	};

	enum class eRegView
	{
		  eViewDefault = 0x0000
		, eViewWow6432 = 0x0200
		, eViewWow6464 = 0x0100
		, eViewUndefined = 0x9999
	};

	enum class eRegValueKind
	{
		  eValueKindUnknown = 0
		, eValueKindString = 1
		, eValueKindExpandedString = 2
		, eValueKindBinary = 3
		, eValueKindDword = 4
		, eValueKindMultiString = 7
		, eValueKindQWord = 11
	};

	enum class eRegKeyAccess : std::uint32_t
	{
		  eKeyAllAccess = 0xF003F
		, eKeyCreateLink = 0x0020
		, eKeyCreateSubKey = 0x0004
		, eKeyEnumerateSubKeys = 0x0008
		, eKeyExecute = 0x20019
		, eKeyNotify = 0x0010
		, eKeyQueryValue = 0x0001
		, eKeyRead = 0x20019
		, eKeySetValue = 0x0002
		, eKeyWow6432Key = 0x0200
		, eKeyWow6464Key = 0x0100
		, eKeyWrite = 0x20006
	};

	namespace Utils
	{
		const int MAX_VALUE_NAME{ 16383 };
		const int MAX_KEY_LENGTH{ 255 };

		static inline std::string WStringToString(const std::wstring &wsVal)
		{
			return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(wsVal);
		}

		static inline void AssertIntNotEqual(const std::wstring &wsLocation, const std::wstring &swValName, const int iNotEqal, const int iTestVal)
		{
			if (iTestVal == iNotEqal)
			{
				std::wstring wsMsg(wsLocation);
				wsMsg += L": Parameter->";
				wsMsg += swValName;
				wsMsg += L" cannot be ";
				wsMsg += std::to_wstring(iNotEqal);
				throw std::invalid_argument(WStringToString(wsMsg).c_str());
			}
		}

		static inline HKEY enumHKeyToHKey(const int eHKeyVal)
		{
			{
				std::wstring sLocation(L"Utils::enumHKeyToHKey");
				AssertIntNotEqual(sLocation, L"eHKeyVal", 0, eHKeyVal);
				switch (eHKeyVal)
				{
				case 1:
					return HKEY_CLASSES_ROOT;
				case 2:
					return HKEY_CURRENT_CONFIG;
				case 3:
					return HKEY_CURRENT_USER;
				case 4:
					return HKEY_LOCAL_MACHINE;
				case 5:
					return HKEY_USERS;
				}
			}
		}

		static inline std::wstring enumHKeyToName(const int eHKeyVal)
		{
			std::wstring sLocation(L"Utils::enumHKeyToHKey");
			AssertIntNotEqual(sLocation, L"eHKeyVal", 0, eHKeyVal);
			switch (eHKeyVal)
			{
			case 1:
				return std::wstring{ L"HKEY_CLASSES_ROOT" };
			case 2:
				return std::wstring{ L"HKEY_CURRENT_CONFIG" };
			case 3:
				return std::wstring{ L"HKEY_CURRENT_USER" };
			case 4:
				return std::wstring{ L"HKEY_LOCAL_MACHINE" };
			case 5:
				return std::wstring{ L"HKEY_USERS" };
			}
		}

		static inline void AssertHr(const std::wstring & wsLocation, const std::wstring & wsFunction, const HRESULT hr)
		{
			if (hr != S_OK)
			{
				LPSTR buffer = NULL;
				size_t size(::FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
					, NULL
					, hr
					, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
					, (LPSTR)&buffer
					, 0
					, NULL));
				std::string sMsg(Utils::WStringToString(wsLocation));
				sMsg += ": Function->";
				sMsg += Utils::WStringToString(wsFunction);
				sMsg += " HRESULT->" + std::to_string(hr);
				sMsg += " ";
				sMsg += std::string(buffer);
				throw std::exception(sMsg.c_str());
			}
		}

		template<typename T>
		static inline void AssertParamNotEqual(const std::wstring &wsLocation, const std::wstring &wsParamName, const T notEqualToVal, const T testVal)
		{
			if (notEqualToVal == testVal)
			{
				std::string sMsg(Utils::WStringToString(wsLocation));
				sMsg += ": Parameter->";
				sMsg += WStringToString(wsParamName);
				sMsg += " Invalid parameter value. Parameter value cannot be equal to its current value.";
				throw std::invalid_argument(sMsg.c_str());
			}
		}

		static inline void ThrowUnknown(const std::wstring & wsLocation)
		{
			std::string sMsg(Utils::WStringToString(wsLocation));
			sMsg += " Unknown exception";
			throw std::exception(sMsg.c_str());
		}

		static inline DWORD GetRegValueSize(const HKEY &regHKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const DWORD &dwFlags)
		{
			std::wstring wsLocation(L"Utils::GetRegValueSize");
			DWORD dwDataSize{};
			Utils::AssertHr
			(
				wsLocation
				, L"::RegGetValue"
				, ::RegGetValue(regHKey, wsSubKey.c_str(), wsValueName.c_str(), dwFlags, nullptr, nullptr, &dwDataSize)
			);
			return dwDataSize;
		}

		static inline std::wstring GetStringValue(const HKEY &regHkey, const std::wstring &wsSubKey, const std::wstring &wsValueName, DWORD &dwSize)
		{
			std::wstring wsLocation(L"Utils::GetStringValue");
			std::wstring wsData;
			wsData.resize(dwSize / sizeof(wchar_t));
			Utils::AssertHr
			(
				wsLocation
				, L"::RegGetValue"
				, ::RegGetValue(regHkey, wsSubKey.c_str(), wsValueName.c_str(), RRF_RT_REG_SZ, nullptr, &wsData[0], &dwSize)
			);
			DWORD dwDataSizeWChar{ dwSize / sizeof(wchar_t) };
			--dwDataSizeWChar;
			wsData.resize(dwDataSizeWChar);
			return wsData;
		}

		static inline std::vector<std::wstring> GetMultiStringValue(const HKEY & regHKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, DWORD &dwSize)
		{
			std::wstring wsLocation(L"Utils::GetMultiStringValue");
			std::vector<wchar_t> vwcData;
			vwcData.resize(dwSize / sizeof(wchar_t));
			Utils::AssertHr
			(
				wsLocation
				, L"::RegGetValue"
				, ::RegGetValue(regHKey, wsSubKey.c_str(), wsValueName.c_str(), RRF_RT_REG_MULTI_SZ, nullptr, &vwcData[0], &dwSize)
			);
			vwcData.resize(dwSize / sizeof(wchar_t));
			std::vector<std::wstring> vwsRet;
			const wchar_t* pwcCurrStringPtr{ &vwcData[0] };
			while (*pwcCurrStringPtr != L'\0')
			{
				const size_t szCurrStringLength{ wcslen(pwcCurrStringPtr) };
				vwsRet.push_back(std::wstring{ pwcCurrStringPtr, szCurrStringLength });
				pwcCurrStringPtr += szCurrStringLength + 1;
			}
			return vwsRet;
		}

		static inline std::wstring GetExpandedStringValue(const HKEY &regHKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const bool bExpand, DWORD &dwSize)
		{
			std::wstring wsLocation(L"Utils::GetExpandedStringValue");
			DWORD isExpanded{ (DWORD)(bExpand ? RRF_RT_REG_SZ : RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND) };
			std::wstring wsData;
			wsData.resize(dwSize / sizeof(wchar_t));
			Utils::AssertHr
			(
				wsLocation
				, L"::RegGetValue"
				, ::RegGetValue(regHKey, wsSubKey.c_str(), wsValueName.c_str(), isExpanded, nullptr, &wsData[0], &dwSize)
			);
			DWORD dwDataSizeWChar{ dwSize / sizeof(wchar_t) };
			--dwDataSizeWChar;
			wsData.resize(dwDataSizeWChar);
			return wsData;
		}

		static inline DWORD GetDwordValue(const HKEY &regHKey, const std::wstring &wsSubKey, const std::wstring &wsValueName)
		{
			std::wstring wsLocation(L"Utils::GetDwordValue");
			DWORD dwRet{};
			DWORD dwDataSize{ sizeof(dwRet) };
			Utils::AssertHr
			(
				wsLocation
				, L"::RegGetValue"
				, ::RegGetValue(regHKey, wsSubKey.c_str(), wsValueName.c_str(), RRF_RT_REG_DWORD, nullptr, &dwRet, &dwDataSize)
			);
			return dwRet;
		}

		static inline ULONGLONG GetQwordValue(const HKEY &regHKey, const std::wstring &wsSubKey, const std::wstring &wsValueName)
		{
			std::wstring wsLocation(L"Utils::GetQwordValue");
			ULONGLONG dwRet{};
			DWORD dwDataSize{ sizeof(dwRet) };
			Utils::AssertHr
			(
				wsLocation
				, L"::RegGetValue"
				, ::RegGetValue(regHKey, wsSubKey.c_str(), wsValueName.c_str(), RRF_RT_REG_QWORD, nullptr, &dwRet, &dwDataSize)
			);
			return dwRet;
		}

		static inline std::vector<BYTE> GetBinaryValues(const HKEY &regHKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, DWORD &dwSize)
		{
			std::wstring wsLocation(L"Utils::GetBinaryValues");
			std::vector<BYTE> vucData;
			vucData.resize(dwSize);
			Utils::AssertHr
			(
				wsLocation
				, L"::RegGetValue"
				, ::RegGetValue(regHKey, wsSubKey.c_str(), wsValueName.c_str(), RRF_RT_REG_BINARY, nullptr, &vucData[0], &dwSize)
			);
			vucData.resize(dwSize);
			return vucData;
		}

		static inline void SetStringValue(const HKEY &regKey, const std::wstring &wsValueName, const std::wstring &wsValueData)
		{
			std::wstring wsLocation(L"Utils::SetStringValue");
			SIZE_T dwDataSize{ (wsValueData.size() + 1) * sizeof(wchar_t) };

			Utils::AssertHr
			(
				wsLocation
				, L"::RegSetValueEx"
				, ::RegSetValueExW(regKey, wsValueName.c_str(), 0, REG_SZ, (LPBYTE)(wsValueData.c_str()), dwDataSize)
			);
		}

		static inline void SetExpandedStringValue(const HKEY &regKey, const std::wstring &wsValueName, const std::wstring &wsValueData)
		{
			std::wstring wsLocation(L"Utils::SetExpandedStringValue");
			SIZE_T dwDataSize{ (wsValueData.size() + 1) * sizeof(wchar_t) };

			Utils::AssertHr
			(
				wsLocation
				, L"::RegSetValueEx"
				, ::RegSetValueExW(regKey, wsValueName.c_str(), 0, REG_EXPAND_SZ, (LPBYTE)(wsValueData.c_str()), dwDataSize)
			);
		}

		static inline void SetMultiStringValue(const HKEY &regKey, const std::wstring &wsValueName, const std::vector<std::wstring> &vwsValueData)
		{
			std::wstring wsLocation(L"Utils::SetMultiStringValue");
			std::vector<wchar_t> vwcBuffer;
			for (auto val : vwsValueData)
			{
				wchar_t const *buf = val.c_str();
				vwcBuffer.insert(vwcBuffer.end(), &buf[0], &buf[val.length() + 1]);
			}
			wchar_t const *buf = L"";
			vwcBuffer.insert(vwcBuffer.end(), &buf[0], &buf[1]);
			SIZE_T dwDataSize{ vwcBuffer.size() * sizeof(wchar_t) };
			Utils::AssertHr
			(
				wsLocation
				, L"::RegSetValueEx"
				, ::RegSetValueEx(regKey, wsValueName.c_str(), 0, REG_MULTI_SZ, (LPBYTE)(&vwcBuffer[0]), dwDataSize)
			);
		}

		static inline void SetBinaryValue(const HKEY &regKey, const std::wstring & wsValueName, const std::vector<BYTE>& vucValueData)
		{
			std::wstring wsLocation(L"Utils::SetBinaryValue");
			SIZE_T dwDataSize{ vucValueData.size() * sizeof(BYTE) };
			Utils::AssertHr
			(
				wsLocation
				, L"::RegSetValueEx"
				, ::RegSetValueEx(regKey, wsValueName.c_str(), 0, REG_BINARY, (LPBYTE)(&vucValueData[0]), dwDataSize)
			);
		}

		static inline void SetDwordValue(const HKEY &regKey, const std::wstring & wsValueName, const DWORD dwValueData)
		{
			std::wstring wsLocation(L"Utils::SetDwordValue");
			DWORD dwDataSize{ sizeof(dwValueData) };
			Utils::AssertHr
			(
				wsLocation
				, L"::RegSetValueEx"
				, ::RegSetValueEx(regKey, wsValueName.c_str(), 0, REG_DWORD, (LPBYTE)&dwValueData, dwDataSize)
			);
		}

		static inline void SetQwordValue(const HKEY &regKey, const std::wstring & wsValueName, const ULONGLONG qwValueData)
		{
			std::wstring wsLocation(L"Utils::SetQwordValue");
			DWORD dwDataSize{ sizeof(qwValueData) };
			Utils::AssertHr
			(
				wsLocation
				, L"::RegSetValueEx"
				, ::RegSetValueEx(regKey, wsValueName.c_str(), 0, REG_QWORD, (LPBYTE)&qwValueData, dwDataSize)
			);
		}

		static inline REGSAM eRegAccessRightsToRegSam(const int eRegAccessRightsVal)
		{
			switch (eRegAccessRightsVal)
			{
			case 0xF003F:
				return KEY_ALL_ACCESS;
			case 0x0004:
				return KEY_CREATE_SUB_KEY;
			case 0x0008:
				return KEY_ENUMERATE_SUB_KEYS;
			case 0x0010:
				return KEY_NOTIFY;
			case 0x0001:
				return KEY_QUERY_VALUE;
			case 0x20019:
				return KEY_READ;
			case 0x0002:
				return KEY_SET_VALUE;
			case 0x20006:
				return KEY_WRITE;
			}

		}
	}

	class CRegistryKey
	{
	public:
		CRegistryKey() = delete;
		inline CRegistryKey(const eHKey hKey)
			: mRegView(eRegView::eViewDefault)
		{
			std::wstring wsLocation(L"CRegistryKey::CRegistryKey");
			try
			{
				int iHKey{ static_cast<int>(hKey) };
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, iHKey);
				mEnumHKey = hKey;
				Utils::AssertHr
				(
					wsLocation
					, L"::RegOpenKeyEx"
					, ::RegOpenKeyEx(Utils::enumHKeyToHKey(iHKey), NULL, 0, KEY_READ, &mHKEY)
				);
			}
			catch (std::exception)
			{
				throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		CRegistryKey(CRegistryKey&&) = default;
		CRegistryKey& operator=(CRegistryKey&&) = default;

		inline virtual ~CRegistryKey()
		{
			::RegCloseKey(mHKEY);
		}

		inline std::wstring Name()
		{
			return std::wstring(Utils::enumHKeyToName(static_cast<int>(mEnumHKey)) + mwsName);
		}

		inline int SubKeyCount()
		{
			if (mbMustUpdate) UpdateProperties();
			return mdwSubKeyCount;
		}

		inline int ValueCount()
		{
			if (mbMustUpdate) UpdateProperties();
			return mdwValueCount;
		}

		inline eRegView View()
		{
			return mRegView;
		}
		
		inline CRegistryKey OpenSubKey(const std::wstring &wsSubKey, const eRegAccessRights enumAccessRights = eRegAccessRights::eAccessKeyRead)
		{
			return CRegistryKey(*this, wsSubKey, enumAccessRights);
		}

		inline CRegistryKey CreateSubKey(const std::wstring &wsSubKey, const eRegAccessRights enumAccessRights = eRegAccessRights::eAccessKeyRead)
		{
			mbMustUpdate = true;
			return CRegistryKey(*this, wsSubKey, enumAccessRights, true);
		}

		inline void DeleteSubKey(const std::wstring &wsSubKey)
		{
			std::wstring wsLocation{ L"CRegistryKey::DeleteSubkey" };
			try
			{
				Utils::AssertHr
				(
					wsLocation
					, L"::RegDeleteKey"
					, ::RegDeleteKey(mHKEY, wsSubKey.c_str())
				);
				mbMustUpdate = true;
			}
			catch (std::exception)
			{
				throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline std::vector<std::wstring> GetValueNames()
		{
			if (mbMustUpdate) UpdateProperties();
			return mvwsValueNames;
		}

		inline std::vector<std::wstring> GetSubKeyNames()
		{
			if (mbMustUpdate) UpdateProperties();
			return mvwsSubKeyNames;
		}

		inline eRegValueKind GetValueKind(const std::wstring &wsValueName)
		{
			if (mbMustUpdate) UpdateProperties();
			return mmpValueKinds.at(wsValueName);
		}

		inline void Flush()
		{
			std::wstring wsLocation{ L"CRegistryKey::Flush" };
			try
			{
				Utils::AssertHr
				(
					wsLocation
					, L"::RegFlushKey"
					, ::RegFlushKey(mHKEY)
				);
			}
			catch (std::exception)
			{
				throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline std::wstring GetStringValue(const std::wstring &wsValueName, const bool bDefaultOnError = false, const std::wstring &wsDefaultVal = L"")
		{
			std::wstring wsLocation{ L"CRegistryKey::GetStringValue" };
			try
			{
				DWORD dwDataSize
				{
					Utils::GetRegValueSize(mHKEY, L"", wsValueName, RRF_RT_REG_SZ)
				};
				return Utils::GetStringValue(mHKEY, L"", wsValueName, dwDataSize);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return wsDefaultVal;
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline std::vector<std::wstring> GetMultiStringValue(const std::wstring &wsValueName, const bool bDefaultOnError = false)
		{
			std::wstring wsLocation{ L"CRegistryKe::GetMultiStringValue" };
			try
			{
				DWORD dwDataSize
				{
					Utils::GetRegValueSize(mHKEY, L"", wsValueName, RRF_RT_REG_MULTI_SZ)
				};
				return Utils::GetMultiStringValue(mHKEY, L"", wsValueName, dwDataSize);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return std::vector<std::wstring>{};
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline std::wstring GetExpandedStringValue(const std::wstring &wsValueName, const bool bExpand = false, const bool bDefaultOnError = false, const std::wstring &wsDefaultVal = L"")
		{
			std::wstring wsLocation{ L"CRegistryKey::GetExpandedStringValue" };
			try
			{
				DWORD dwDataSize
				{
					Utils::GetRegValueSize
					(
						mHKEY
						, L""
						, wsValueName
						, (DWORD)(bExpand ? RRF_RT_REG_SZ : RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND)
					)
				};
				return Utils::GetExpandedStringValue(mHKEY, L"", wsValueName, bExpand, dwDataSize);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return wsDefaultVal;
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline DWORD GetDwordValue(const std::wstring &wsValueName, const bool bDefaultOnError = false, const DWORD dwDefaultVal = 0)
		{
			std::wstring wsLocation{ L"CRegistryKey::GetDwordValue" };
			try
			{
				return Utils::GetDwordValue(mHKEY, L"", wsValueName);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return dwDefaultVal;
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline ULONGLONG GetQwordValue(const std::wstring &wsValueName, const bool bDefaultOnError = false, const ULONGLONG qwDefaultVal = 0)
		{
			std::wstring wsLocation{ L"CRegistryKey::GetQwordValue" };
			try
			{
				return Utils::GetQwordValue(mHKEY, L"", wsValueName);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return qwDefaultVal;
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline std::vector<BYTE> GetBinaryValue(const std::wstring &wsValueName, const bool bDefaultOnError = false)
		{
			std::wstring wsLocation{ L"CRegistryKey::GetBinaryValue" };
			try
			{
				DWORD dwDataSize
				{
					Utils::GetRegValueSize(mHKEY, L"", wsValueName, RRF_RT_REG_BINARY)
				};
				return Utils::GetBinaryValues(mHKEY, L"", wsValueName, dwDataSize);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return std::vector<BYTE>{};
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetStringValue(const std::wstring &wsValueName, const std::wstring &wsVal)
		{
			std::wstring wsLocation{ L"CRegistryKey::SetStringValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::SetStringValue(mHKEY, wsValueName, wsVal);
				mbMustUpdate = true;
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetMultiStringValue(const std::wstring &wsValueName, const std::vector<std::wstring> &vwsVal)
		{
			std::wstring wsLocation{ L"CRegistryKey::SetMultiStringValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::SetMultiStringValue(mHKEY, wsValueName, vwsVal);
				mbMustUpdate = true;
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetExpandedStringValue(const std::wstring &wsValueName, const std::wstring &wsVal)
		{
			std::wstring wsLocation{ L"CRegistryKey::SetExpandedStringValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::SetExpandedStringValue(mHKEY, wsValueName, wsVal);
				mbMustUpdate = true;
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetBinaryValue(const std::wstring &wsValueName, const std::vector<BYTE> &vucVal)
		{
			std::wstring wsLocation{ L"CRegistryKey::SetMultiStringValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::SetBinaryValue(mHKEY, wsValueName, vucVal);
				mbMustUpdate = true;
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetDwordValue(const std::wstring &wsValueName, const DWORD dwVal)
		{
			std::wstring wsLocation{ L"CRegistryKey::SetDwordValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::SetDwordValue(mHKEY, wsValueName, dwVal);
				mbMustUpdate = true;
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetQwordValue(const std::wstring &wsValueName, const ULONGLONG qwVal)
		{
			std::wstring wsLocation{ L"CRegistryKey::SetQwordValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::SetQwordValue(mHKEY, wsValueName, qwVal);
				mbMustUpdate = true;
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void DeleteValue(const std::wstring &wsValueName)
		{
			std::wstring wsLocation{ L"CRegistryKey::DeleteValue" };
			try
			{
				Utils::AssertHr
				(
					wsLocation
					, L"::RegDeleteValue"
					, ::RegDeleteValue(mHKEY, wsValueName.c_str())
				);
				mbMustUpdate = true;
			}
			catch (std::exception)
			{
				throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		

	private:
		eHKey mEnumHKey{ eHKey::eHkeyNotDefined };
		HKEY mHKEY{ nullptr };
		std::wstring mwsName{ L"" };
		DWORD mdwSubKeyCount{ 0 };
		DWORD mdwValueCount{ 0 };
		std::vector<std::wstring> mvwsValueNames;
		std::vector<std::wstring> mvwsSubKeyNames;
		std::map<std::wstring, eRegValueKind> mmpValueKinds;
		eRegView mRegView{ eRegView::eViewUndefined };
		bool mbMustUpdate{ true };
		eRegAccessRights mAccessRights{ eRegAccessRights::eAccessKeyRead };

		inline CRegistryKey(const CRegistryKey &parentRegKey, const std::wstring &wsSubKey, const eRegAccessRights enumAccessRights = eRegAccessRights::eAccessKeyRead, const bool bCreate = false)
			: mEnumHKey(parentRegKey.mEnumHKey)
			, mwsName(wsSubKey.length() > 0 ? parentRegKey.mwsName + L"\\" + wsSubKey : L"")
			, mRegView(eRegView::eViewDefault)
			, mAccessRights(enumAccessRights)
		{
			std::wstring wsLocation(L"CRegistryKey::CRegistryKey");
			try
			{
				if (bCreate)
				{
					Utils::AssertHr
					(
						wsLocation
						, L"::RegCreateKeyEx"
						, ::RegCreateKeyEx(parentRegKey.mHKEY, wsSubKey.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, Utils::eRegAccessRightsToRegSam(static_cast<int>(mAccessRights)), nullptr, &mHKEY, nullptr)
					);
				}
				else
				{
					Utils::AssertHr
					(
						wsLocation
						, L"::RegOpenKeyEx"
						, ::RegOpenKeyEx(parentRegKey.mHKEY, wsSubKey.c_str(), 0, Utils::eRegAccessRightsToRegSam(static_cast<int>(mAccessRights)), &mHKEY)
					);
				}
			}
			catch (std::exception)
			{
				throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void UpdateProperties()
		{
			std::wstring wsLocation(L"CRegistryKey::UpdateProperties");
			try
			{
				Utils::AssertHr
				(
					wsLocation
					, L"::RegQueryInfoKey"
					, ::RegQueryInfoKey(mHKEY, nullptr, nullptr, nullptr, &mdwSubKeyCount, nullptr, nullptr, &mdwValueCount, nullptr, nullptr, nullptr, nullptr)
				);

				mvwsValueNames.clear();
				mvwsValueNames.reserve(mdwValueCount);
				mmpValueKinds.clear();
				if (mdwValueCount > 0)
				{
					for (DWORD dwCount = 0; dwCount < mdwValueCount; dwCount++)
					{
						std::wstring wsValueName;
						wsValueName.resize(Utils::MAX_VALUE_NAME); // function works with wide strings so no need to double size
						DWORD dwSize{ Utils::MAX_VALUE_NAME };
						DWORD dwType{ 0 };
						Utils::AssertHr
						(
							wsLocation
							, L"::RegEnumValue"
							, ::RegEnumValue(mHKEY, dwCount, &wsValueName[0], &dwSize, nullptr, &dwType, nullptr, nullptr)
						);
						wsValueName.resize(dwSize); //size not including string terminator, no need to decrease by one
						mvwsValueNames.emplace_back(wsValueName);
						mmpValueKinds.emplace(wsValueName, static_cast<eRegValueKind>(dwType));
					}
				}

				mvwsSubKeyNames.clear();
				mvwsSubKeyNames.reserve(mdwSubKeyCount);
				if (mdwSubKeyCount > 0)
				{
					for (DWORD dwCount = 0; dwCount < mdwSubKeyCount; dwCount++)
					{
						std::wstring wsSubKeyName;
						wsSubKeyName.resize(Utils::MAX_KEY_LENGTH); // function works with wide strings so no need to double size
						DWORD dwSize{ Utils::MAX_KEY_LENGTH };
						Utils::AssertHr
						(
							wsLocation
							, L"::RegEnumValue"
							, ::RegEnumKeyEx(mHKEY, dwCount, &wsSubKeyName[0], &dwSize, nullptr, nullptr, nullptr, nullptr)
						);
						wsSubKeyName.resize(dwSize); //size not including string terminator, no need to decrease by one
						mvwsSubKeyNames.emplace_back(wsSubKeyName);
					}
				}
				mbMustUpdate = false;
			}
			catch (std::exception)
			{
				throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}
};

	namespace Registry
	{
		inline CRegistryKey ClassesRoot()
		{
			return CRegistryKey(eHKey::eHKeyClassesRoot);
		}

		inline CRegistryKey CurrentConfig()
		{
			return CRegistryKey(eHKey::eHKeyCurrentConfig);
		}

		inline CRegistryKey CurrentUser()
		{
			return CRegistryKey(eHKey::eHKeyCurrentUser);
		}

		inline CRegistryKey LocalMachine()
		{
			return CRegistryKey(eHKey::eHKeyLocalMachine);
		}

		inline CRegistryKey Users()
		{
			return CRegistryKey(eHKey::eHKeyUsers);
		}

		inline std::wstring GetStringValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const bool bDefaultOnError = false, const std::wstring &wsDefaultVal = L"")
		{
			std::wstring wsLocation{ L"Registry::GetStringValue" };
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));
				DWORD dwDataSize
				{
					Utils::GetRegValueSize(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey, wsValueName, RRF_RT_REG_SZ)
				};
				return Utils::GetStringValue(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey, wsValueName, dwDataSize);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return wsDefaultVal;
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline std::wstring GetExpandedStringValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const bool bExpand = false, const bool bDefaultOnError = false, const std::wstring &wsDefaultVal = L"")
		{
			std::wstring wsLocation{ L"Registry::GetExpandedStringValue" };
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));
				DWORD dwDataSize
				{
					Utils::GetRegValueSize
					(
						Utils::enumHKeyToHKey(static_cast<int>(hKey))
						, wsSubKey
						, wsValueName
						, (DWORD)(bExpand ? RRF_RT_REG_SZ : RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND)
					)
				};
				return Utils::GetExpandedStringValue(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey, wsValueName, bExpand, dwDataSize);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return wsDefaultVal;
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline DWORD GetDwordValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const bool bDefaultOnError = false, const DWORD dwDefaultVal = 0)
		{
			std::wstring wsLocation{ L"Registry::GetDwordValue" };
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));
				return Utils::GetDwordValue(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey, wsValueName);;
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return dwDefaultVal;
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline ULONGLONG GetQwordValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const bool bDefaultOnError = false, const ULONGLONG qwDefaultVal = 0)
		{
			std::wstring wsLocation{ L"Registry::GetQwordValue" };
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));
				return Utils::GetQwordValue(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey, wsValueName);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return qwDefaultVal;
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline std::vector<std::wstring> GetMultiStringValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const bool bDefaultOnError = false)
		{
			std::wstring wsLocation{ L"Registry::GetMultiStringValue" };
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));
				DWORD dwDataSize
				{
					Utils::GetRegValueSize(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey, wsValueName, RRF_RT_REG_MULTI_SZ)
				};
				return Utils::GetMultiStringValue(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey, wsValueName, dwDataSize);
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return std::vector<std::wstring>{};
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline std::vector<BYTE> GetBinaryValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const bool bDefaultOnError = false)
		{
			std::wstring wsLocation{ L"Registry::GetBinaryValue" };
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));
				DWORD dwDataSize
				{
					Utils::GetRegValueSize(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey, wsValueName, RRF_RT_REG_BINARY)
				};
				std::vector<BYTE> vucData;
				vucData.resize(dwDataSize);
				Utils::AssertHr
				(
					wsLocation
					, L"::RegGetValue"
					, ::RegGetValue(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey.c_str(), wsValueName.c_str(), RRF_RT_REG_BINARY, nullptr, &vucData[0], &dwDataSize)
				);
				vucData.resize(dwDataSize);
				return vucData;
			}
			catch (std::exception)
			{
				if (bDefaultOnError)
					return std::vector<BYTE>{};
				else
					throw;
			}
			catch (...)
			{
				Utils::ThrowUnknown(wsLocation);
			}
		}
		
		inline void SetStringValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const std::wstring &wsVal)
		{
			std::wstring wsLocation{ L"Registry::SetStringValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));

				Utils::AssertHr
				(
					wsLocation
					, L"::RegOpenKeyEx"
					, ::RegOpenKeyEx(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey.c_str(), 0, KEY_SET_VALUE, &hRegHandle)
				);

				Utils::SetStringValue(hRegHandle, wsValueName, wsVal);

				Utils::AssertHr
				(
					wsLocation
					, L"::RegCloseKey"
					, ::RegCloseKey(hRegHandle)
				);
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetExpandedStringValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const std::wstring &wsVal)
		{
			std::wstring wsLocation{ L"Registry::SetExpandedStringValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));

				Utils::AssertHr
				(
					wsLocation
					, L"::RegOpenKeyEx"
					, ::RegOpenKeyEx(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey.c_str(), 0, KEY_SET_VALUE, &hRegHandle)
				);

				Utils::SetExpandedStringValue(hRegHandle, wsValueName, wsVal);

				Utils::AssertHr
				(
					wsLocation
					, L"::RegCloseKey"
					, ::RegCloseKey(hRegHandle)
				);
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetDwordValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const DWORD dwVal)
		{
			std::wstring wsLocation{ L"Registry::SetDwordValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));

				Utils::AssertHr
				(
					wsLocation
					, L"::RegOpenKeyEx"
					, ::RegOpenKeyEx(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey.c_str(), 0, KEY_SET_VALUE, &hRegHandle)
				);

				Utils::SetDwordValue(hRegHandle, wsValueName, dwVal);

				Utils::AssertHr
				(
					wsLocation
					, L"::RegCloseKey"
					, ::RegCloseKey(hRegHandle)
				);
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetQwordValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const ULONGLONG qwVal)
		{
			std::wstring wsLocation{ L"Registry::SetQwordValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));

				Utils::AssertHr
				(
					wsLocation
					, L"::RegOpenKeyEx"
					, ::RegOpenKeyEx(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey.c_str(), 0, KEY_SET_VALUE, &hRegHandle)
				);

				Utils::SetQwordValue(hRegHandle, wsValueName, qwVal);

				Utils::AssertHr
				(
					wsLocation
					, L"::RegCloseKey"
					, ::RegCloseKey(hRegHandle)
				);
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetMultiStringValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const std::vector<std::wstring> &vwsVal)
		{
			std::wstring wsLocation{ L"Registry::SetMultiStringValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));

				Utils::AssertHr
				(
					wsLocation
					, L"::RegOpenKeyEx"
					, ::RegOpenKeyEx(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey.c_str(), 0, KEY_SET_VALUE, &hRegHandle)
				);

				Utils::SetMultiStringValue(hRegHandle, wsValueName, vwsVal);

				Utils::AssertHr
				(
					wsLocation
					, L"::RegCloseKey"
					, ::RegCloseKey(hRegHandle)
				);
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}

		inline void SetBinaryValue(const eHKey hKey, const std::wstring &wsSubKey, const std::wstring &wsValueName, const std::vector<BYTE> &vucVal)
		{
			std::wstring wsLocation{ L"Registry::SetMultiStringValue" };
			HKEY hRegHandle{};
			try
			{
				Utils::AssertParamNotEqual(wsLocation, L"hKey", 0, static_cast<int>(hKey));

				Utils::AssertHr
				(
					wsLocation
					, L"::RegOpenKeyEx"
					, ::RegOpenKeyEx(Utils::enumHKeyToHKey(static_cast<int>(hKey)), wsSubKey.c_str(), 0, KEY_SET_VALUE, &hRegHandle)
				);

				Utils::SetBinaryValue(hRegHandle, wsValueName, vucVal);

				Utils::AssertHr
				(
					wsLocation
					, L"::RegCloseKey"
					, ::RegCloseKey(hRegHandle)
				);
			}
			catch (std::exception)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				throw;
			}
			catch (...)
			{
				if (hRegHandle) ::RegCloseKey(hRegHandle);
				Utils::ThrowUnknown(wsLocation);
			}
		}
	}
}