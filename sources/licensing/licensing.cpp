#include <iostream>
#include "licensing.h"
#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>
#include <cryptolens/Configuration_Windows.hpp>
#include <cryptolens/MachineCodeComputer_static.hpp>
#pragma warning(disable : 4996)
namespace cryptolens = ::cryptolens_io::v20190401;
using Cryptolens = cryptolens::basic_Cryptolens<cryptolens::Configuration_Windows<cryptolens::MachineCodeComputer_static>>;

INT CheckLicense(Product p, const char* license)
{
	cryptolens::Error e;
	Cryptolens cryptolens_handle(e);

	// Setting up the signature verifier with credentials from "Security Settings"
	// on serialkeymanager.com
	cryptolens_handle.signature_verifier.set_modulus_base64(e, p.module_base);
	cryptolens_handle.signature_verifier.set_exponent_base64(e, p.exponent_base);

	cryptolens_handle.machine_code_computer.set_machine_code(e, p.machine_code);

	cryptolens::optional<cryptolens::LicenseKey> license_key =
		cryptolens_handle.activate(
			e,
			// SKM Access Token
			p.skm_token,
			// Product id
			p.pid,
			// License Key
			license
		);

	if (e) {
		using namespace cryptolens::errors;

		if (e.get_subsystem() == Subsystem::Main) {
			// Handle errors from the SKM API
			OutputDebugString(L"Licensing: SKM API ERROR");
		}
		else if (e.get_subsystem() == Subsystem::RequestHandler) {
			size_t code = e.get_extra();
			OutputDebugString(L"Licensing: Couldn't connect to Cryptolens Server.");
		}
		else {
			OutputDebugString(L"Licensing: Unhandled Exception in Licensing.");
		}
		return 0;
	}


	OutputDebugString(L"------------------------ACTIVATION------------------------");
	OutputDebugString(std::wstring(std::to_string(p.pid).begin(), std::to_string(p.pid).end()).c_str());
	OutputDebugString(reinterpret_cast<LPCWSTR>(license));
	OutputDebugString(L"----------------------------------------------------------");


	std::time_t current_time = std::time(nullptr);
	std::asctime(std::localtime(&current_time));
	// Use LicenseKeyChecker to check properties of the license key
	if (license_key->check().has_expired(current_time) || license_key->check().is_blocked() || !license_key->check().is_on_right_machine()) {
		return 0;
	}

	return 1;
}

/*
* This example uses the basic_SKM class to make a request to the WebAPI
* and then checks some properties of the license keys.
*/
int main()
{

}
