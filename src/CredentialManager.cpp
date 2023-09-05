#include "CredentialManager.h"
#include <mbedtls/sha256.h>
#include <sstream>
#include <iomanip>

void CredentialManager::open(const String &dbPath)
{
	_db.open(dbPath);
	_db.execute("CREATE TABLE IF NOT EXISTS credentials (username TEXT NOT NULL PRIMARY KEY, password_hash TEXT NOT NULL) WITHOUT ROWID");
}

bool CredentialManager::authenticate(const String &username, const String password)
{
	String passwordHash = getPasswordHash(password);

	JSONVar result = _db.execute(
		"SELECT * FROM credentials WHERE username=? AND password_hash=?",
		username,
		passwordHash);

	if (result.length() == -1)
		return false;
	return true;
}

void CredentialManager::registerUser(const String &username, const String password)
{
	String passwordHash = getPasswordHash(password);
	_db.execute(
		"INSERT INTO credentials VALUES (?, ?)",
		username,
		passwordHash);
}

void CredentialManager::changeUsername(const String &oldUsername, const String &newUsername)
{
	_db.execute(
		"UPDATE credentials SET username=? WHERE username=?",
		newUsername,
		oldUsername);
}

void CredentialManager::changePassword(const String &username, const String &newPassword)
{
	String newPasswordHash = getPasswordHash(newPassword);
	_db.execute(
		"UPDATE credentials SET password_hash=? WHERE username=?",
		newPasswordHash,
		username);
}

String CredentialManager::getPasswordHash(const String &password)
{
	uint8_t hashResult[32];
	mbedtls_sha256_ret(reinterpret_cast<const uint8_t *>(password.c_str()),
					   password.length(),
					   hashResult,
					   0);

	std::ostringstream resultBuilder;
	for (auto &&i : hashResult)
		resultBuilder << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
	return resultBuilder.str().c_str();
}
