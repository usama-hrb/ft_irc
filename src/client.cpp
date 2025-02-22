#include "../inc/Client.hpp"

Client::Client(int fd) : _fd(fd), _hasPassword(false), _hasNickName(false), _hasUserName(false), _isRegistred(false) {}

Client::~Client() {}

// GETTERS
int Client::getFd() const {return _fd;}
std::string Client::getNickName() const {return _nickName;}
std::string Client::getUserName() const {return _userName;}
std::string Client::getRealName() const {return _realName;}
bool Client::isRegistred() const {return _isRegistred;}
bool Client::hasPassword() const {return _hasPassword;}
bool Client::hasNickName() const {return _hasNickName;}

// SETTERS
void Client::setNickName(const std::string& nickName) {
	_nickName = nickName, _hasNickName = true;
}
void Client::setUserName(const std::string& userName) {
	_userName = userName, _hasUserName = true;
}
void Client::setRealName(const std::string& realName) {
	_realName = realName;
}
void Client::setPassword(bool status) {
	_hasPassword = status;
}
void Client::setIsRegistered(bool status) {
	_isRegistred = status;
}