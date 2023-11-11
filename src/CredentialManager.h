#if !defined(_CredentialManager_h)
#define _CredentialManager_h

#include <DatabaseConnection.h>

class CredentialManager
{
public:
	CredentialManager() = default;

	void open(const String &dbPath);
	bool authenticate(const String &username, const String password);
	void registerUser(const String &username, const String password);
	void changeUsername(const String &oldUsername, const String &newUsername);
	void changePassword(const String &username, const String &newPassword);

private:
	DatabaseConnection _db;

	String getPasswordHash(const String &password);
};

#endif // _CredentialManager_h
