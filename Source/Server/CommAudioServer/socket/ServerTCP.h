#ifndef SERVERTCP_H
#define SERVERTCP_H
#include "../global.h"
#include "Server.h"

enum ClientRequest { SongRequest = 1, DownloadSong = 2, SendPlaylist = 3, BroadcastVoice = 4 };

struct ClientInfo {
    SOCKET socket;
    SOCKET_INFORMATION socketInformation;
};

class ServerTCP : public Server {
public:
    ServerTCP() {}
    ~ServerTCP() {}

    std::map<std::string, ClientInfo> mapClientInfo;

    boolean InitSocket(int port);
    boolean AcceptClient();
    boolean SendOne(LPSOCKET_INFORMATION socketInformation, char * message);
    boolean SendOne2(LPSOCKET_INFORMATION socketInformation, char* message);
    boolean SendAll(char *message, LPDWORD lpNumberOfBytesSent);
};

#endif // SERVERTCP_H
