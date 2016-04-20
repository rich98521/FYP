#ifndef NETWORK_H
#define NETWORK_H
using namespace std;
#include "curl\curl.h"
#include <functional>
#include <array>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <SFML/Network.hpp>



class Network
{
	static CURL *mHandle;
	static string mUrl;
	static std::string mReadBuffer;
	static sf::UdpSocket mUdpSocket;
	static char mData[100];
	static std::size_t mReceived;
	static UINT16 mPort;
	static UINT16 mPort2;
	static UINT16 mTcpPort;
	static sf::TcpListener mListener;
	static sf::TcpSocket mTcpSocket;
	static bool mConnected;
	static bool mHost;
	static string mConnectedIp;
	static sf::Packet mPacket;
public:
	static void Init();
	static string Post(string, std::vector<std::array<string, 2>>);
	static string Get(string, std::vector<std::array<string, 2>>);
	static string GetIP();
	static string ArgsToString(std::vector<std::array<string, 2>>);
	static std::map<string, string> ResultToMap(string);
	static std::vector<std::string> Split(std::string s, string delim);
	static void SendConnectMessage(string);	//happens when client joins server
	static sf::Packet UdpListen();				//happens when client joins server
	static void StartTcpListen();			//happens host starts server
	static void SendUdp(sf::Packet);
	static bool Connected(){ return mConnected; }
	static string GetLocalIP();
	static bool Host(){ return mHost; };
};

#endif
