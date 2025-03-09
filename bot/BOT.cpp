#include "../inc/Server.hpp"

void Server::checkChallengeCompletion(Challenge& challenge) {
    if (challenge.challenger_choice.empty() || challenge.target_choice.empty()) {
        return;
    }

    Channel* channel = _channelManager.search_for_channel(challenge.channel);
    if (!channel) return;

    // Determine outcome
    std::string result;
    if (challenge.challenger_choice == challenge.target_choice) {
        result = "Draw! Both chose " + challenge.challenger_choice;
    } else if (
        (challenge.challenger_choice == "rock" && challenge.target_choice == "scissors") ||
        (challenge.challenger_choice == "paper" && challenge.target_choice == "rock") ||
        (challenge.challenger_choice == "scissors" && challenge.target_choice == "paper")
    ) {
        result = challenge.challenger + " wins! " + challenge.challenger_choice + 
                " beats " + challenge.target_choice;
    } else {
        result = challenge.target + " wins! " + challenge.target_choice + 
                " beats " + challenge.challenger_choice;
    }

    // Broadcast results
    broadcastNotice(channel, "RPS Result: " + result);
    active_challenges_.erase(challenge.challenger);
}

void Server::checkChallengeTimeouts() {
    time_t now = time(NULL);
    std::vector<std::string> to_remove;

    for (const auto& [challenger, challenge] : active_challenges_) {
        if (now - challenge.created_at > 120) {  // 2-minute timeout
            to_remove.push_back(challenger);
            Channel* channel = _channelManager.search_for_channel(challenge.channel);
            if (channel) {
                broadcastNotice(channel, "RPS challenge from " + challenger + " timed out");
            }
        }
    }

    for (const auto& challenger : to_remove) {
        active_challenges_.erase(challenger);
    }
}

void Server::sendNotice(Client* client, const std::string& message) {
    std::string msg = ":FT_irc" " NOTICE " + client->getNickName() + 
                     " :" + message + "\r\n";
    sendReplay(client->getFd(), msg);
}


void Server::broadcastNotice(Channel* channel, const std::string& message) {
    std::string msg = ":FT_irc" " NOTICE " + channel->getName() + 
                     " :" + message + "\r\n";
    channel->broadcast(msg, "");
}

void Server::handleRPS(Client* client, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        sendNotice(client, "Syntax: /rps <nickname> <channel>");
        return;
    }

    std::string target_nick = params[0];
    std::string channel_name = params[1];
    
    if (client->getNickName() == target_nick) {
        sendNotice(client, "You can't challenge yourself");
        return;
    }

    Client* target = findClientByNickname(target_nick);
    if (!target) {
        sendNotice(client, "User " + target_nick + " not found");
        return;
    }

    Channel* channel = _channelManager.search_for_channel(channel_name);
    if (!channel) {
        sendNotice(client, "Channel " + channel_name + " doesn't exist");
        return;
    }

    // Create new challenge
   Challenge new_challenge;

new_challenge.challenger = client->getNickName();
new_challenge.target = target_nick;
new_challenge.challenger_choice = "";
new_challenge.target_choice = "";
new_challenge.accepted = false;
new_challenge.created_at = time(nullptr);
new_challenge.channel = channel_name;
    
    active_challenges_[client->getNickName()] = new_challenge;

    // Notify participants
    sendNotice(client, "Challenge sent to " + target_nick + " in " + channel_name);
    
    std::string target_msg = "You've been challenged to RPS by " + client->getNickName() + 
                            " in " + channel_name + ". Use /accept to confirm";
    sendNotice(target, target_msg);

    // Broadcast to channel
    broadcastNotice(channel, client->getNickName() + " challenged " + target_nick + " to RPS!");
}


void Server::handleAccept(Client* client, const std::vector<std::string>& params) {
    for (auto& [challenger, challenge] : active_challenges_) {
        if (challenge.target == client->getNickName()) {
            challenge.accepted = true;
            
            Channel* channel = _channelManager.search_for_channel(challenge.channel);
            if (channel) {
                broadcastNotice(channel, client->getNickName() + " accepted the RPS challenge!");
            }
            return;
        }
    }
    sendNotice(client, "No pending challenges to accept");
}

void Server::handleChoose(Client* client, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        sendNotice(client, "Syntax: /choose <rock|paper|scissors> <channel>");
        return;
    }

    std::string choice = params[0];
    std::string channel_name = params[1];
    std::transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

    // Validate choice
    if (choice != "rock" && choice != "paper" && choice != "scissors") {
        sendNotice(client, "Invalid choice. Valid options: rock, paper, scissors");
        return;
    }

    // Find active challenge
    bool found = false;
    for (auto& [challenger, challenge] : active_challenges_) {
        if (challenge.channel != channel_name) continue;

        if (client->getNickName() == challenge.challenger) {
            challenge.challenger_choice = choice;
            found = true;
			if (challenge.target_choice.empty()) {
                Client* target = findClientByNickname(challenge.target);
                if (target) {
                    std::string msg = "[RPS] " + challenge.challenger + " has chosen. Your turn!";
                    sendNotice(target, msg);
                }
            }
        } else if (client->getNickName() == challenge.target) {
            challenge.target_choice = choice;
            found = true;
			if (challenge.challenger_choice.empty()) {

			Client* challenger = findClientByNickname(challenge.challenger);
		    if (challenger) {
		        std::string msg = challenge.target + " has chosen. Your turn!";
		        sendNotice(challenger, msg);
		    }
			}
        }

        if (found) {
            checkChallengeCompletion(challenge);
            break;
        }
    }

    if (!found) {
        sendNotice(client, "No active RPS challenge in " + channel_name);
    }
}
