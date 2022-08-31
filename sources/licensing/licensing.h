#pragma once
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <string>



namespace KeyAuth {
	class api {
	public:

		std::string name, ownerid, secret, version, url;

		api(std::string name, std::string ownerid, std::string secret, std::string version, std::string url) : name(name), ownerid(ownerid), secret(secret), version(version), url(url) {}

		void ban();
		void init();
		void check();
		void log(std::string msg);
		void license(std::string key);
		std::string var(std::string varid);
		std::string webhook(std::string id, std::string params);
		void setvar(std::string var, std::string vardata);
		std::string getvar(std::string var);
		bool checkblack();
		void upgrade(std::string username, std::string key);
		void login(std::string username, std::string password);
		void web_login();
		void button(std::string value);
		std::vector<unsigned char> download(std::string fileid);
		void regstr(std::string username, std::string password, std::string key);

		class data_class {
		public:
			// app data
			std::string numUsers;
			std::string numOnlineUsers;
			std::string numKeys;
			std::string version;
			std::string customerPanelLink;
			// user data
			std::string username;
			std::string ip;
			std::string hwid;
			std::string createdate;
			std::string lastlogin;
			std::vector<std::string> subscriptions;
			std::string expiry;
			// response data
			bool success;
			std::string message;
		};
		data_class data;

	private:
		std::string sessionid, enckey;
	};
}



static std::string loggedInUser = "";

static std::string name = "Apex"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
static std::string ownerid = "OaL7cjS67R"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
static std::string secret = "d39b0c9762992ddad1ad06c1cf964dade81bd6e45872a8eb1994f3dea75df010"; // app secret, the blurred text on licenses tab and other tabs
static std::string version = "1.0"; // leave alone unless you've changed version on website
static std::string url = "https://keyauth.win/api/1.2/"; // change if you're self-hosting
static std::string sslPin = "ssl pin key (optional)"; // don't change unless you intend to pin public certificate key. you can get here in the "Pin SHA256" field https://www.ssllabs.com/ssltest/analyze.html?d=keyauth.win&latest. If you do this you need to be aware of when SSL key expires so you can update it

using namespace KeyAuth;
static api KeyAuthApp(name, ownerid, secret, version, url);


namespace licensing
{
	class Auth
	{
	public:
		static bool Authenticate(std::string username, std::string password)
		{
			KeyAuthApp.init();

			if (!KeyAuthApp.data.success)
			{
				MessageBoxA(NULL, KeyAuthApp.data.message.c_str(), "Error", NULL);

				return false;
			}

			KeyAuthApp.check();

			KeyAuthApp.login(username, password);

			if (!KeyAuthApp.data.success)
			{
				//std::cout << xor ("\n Status: ") << KeyAuthApp.data.message
				exit(0);
			}

			KeyAuthApp.check();

			MessageBoxA(NULL, KeyAuthApp.data.message.c_str(), "Error", NULL);

			return true;
		}
	};
}
