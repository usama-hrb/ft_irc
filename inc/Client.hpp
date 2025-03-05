#pragma once

#include "Needs.hpp"

class Client {
	int 		_fd;
	std::string _nickName;
	std::string _userName;
	std::string _realName;
	std::string _hostName;
	bool _hasPassword;
	bool _hasNickName;
	bool _hasUserName;
	bool _isRegistred;

public:
	Client(int fd);
	~Client();

	// GETTERS
	int getFd() const;
	std::string getNickName() const;
	std::string getUserName() const;
	std::string getRealName() const;
	bool isRegistred() const;
	bool hasPassword() const;
	bool hasNickName() const;
	bool hasUserName() const;

	// SETTERS
	void setNickName(const std::string& nickName);
    void setUserName(const std::string& userName);
    void setRealName(const std::string& realName);
    void setPassword(bool status);
    void setIsRegistered(bool status);
};