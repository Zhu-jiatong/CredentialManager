#include <CredentialManager.h>
#include <SD.h>

CredentialManager credentials;

void registerUser();
void authenticate();
void updatePassword();

void setup()
{
	try
	{
		Serial.begin(115200);

		if (!SD.begin())
			throw std::runtime_error("cannot initialise SD");

		if (SD.exists("/credentials.db"))
			SD.remove("/credentials.db");

		credentials.open("/sd/credentials.db");
	}
	catch (const std::exception &e)
	{
		Serial.println(e.what());
		while (true)
			;
	}
}

void loop()
{
	try
	{
		registerUser();
		authenticate();
		updatePassword();
		updateUsername();
	}
	catch (const std::exception &e)
	{
		Serial.println(e.what());
	}
}

void registerUser()
{
	Serial.println(__func__);

	Serial.println("username");
	while (!Serial.available())
		;
	String username = Serial.readString();

	Serial.println("password");
	while (!Serial.available())
		;
	String password = Serial.readString();

	credentials.registerUser(username, password);
}

void authenticate()
{
	Serial.println(__func__);

	Serial.println("username");
	while (!Serial.available())
		;
	String username = Serial.readString();

	Serial.println("password");
	while (!Serial.available())
		;
	String password = Serial.readString();

	Serial.println(credentials.authenticate(username, password) ? "OK" : "NO");
}

void updatePassword()
{
	Serial.println(__func__);

	Serial.println("username");
	while (!Serial.available())
		;
	String username = Serial.readString();

	Serial.println("new password");
	while (!Serial.available())
		;
	String password = Serial.readString();

	credentials.changePassword(username, password);
}

void updateUsername()
{
	Serial.println(__func__);

	Serial.println("username");
	while (!Serial.available())
		;
	String username = Serial.readString();

	Serial.println("new username");
	while (!Serial.available())
		;
	String newUsername = Serial.readString();

	credentials.changeUsername(username, newUsername);
}