#include "stdafx.h"
#include "Network.h"

//loads in screen resolution from config file
void Network::Init()
{
	mUrl = "http://52.38.28.13:8888/";
	//cout << Post("updateserver/", { { { { "type", "update" } }, { { "name", "test8" } }, { { "count", "7" } }, { { "ip", "123.345.567.789" } } } }) << endl;
	//cout << Get("servers/", { {} }) << endl;
	//cout << Post("updateserver/", { { { { "type", "delete" } }, { { "name", "test7" } } } });
	mPort = 32640;
	mPort2 = 32641;
	mTcpPort = 32642;
	mUdpSocket.setBlocking(false);
	mConnected = false;
	mHost = true;

	//mConnected = true;
	//mConnectedIp = "192.168.192.16";
	if (mUdpSocket.bind(mPort) == sf::Socket::Error)
	{
		mPort2 = mPort;
		mPort = 32641;
		mUdpSocket.bind(mPort);
	}
}

string AddressToString(sf::IpAddress ip)
{
	in_addr a;
	a.S_un.S_addr = ip.toInteger();
	string s = inet_ntoa(a);
	std::vector<string> indiv = Network::Split(s, ".");
	s = indiv[3] + "." + indiv[2] + "." + indiv[1] + "." + indiv[0];
	return s;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

string Network::GetLocalIP()
{
	char namebuff[256];
	gethostname(namebuff, 256);
	struct hostent *remoteHost;
	struct in_addr addr;
	remoteHost = gethostbyname(namebuff);
	addr.s_addr = *(u_long *)remoteHost->h_addr_list[0];
	return inet_ntoa(addr);
}

std::vector<std::string> Network::Split(std::string s, string delim) 
{
	int at = 0;
	int last = 0;
	std::vector<std::string> elements;
	while (at >= 0)
	{
		at = s.find(delim, last);
		elements.push_back(s.substr(last, at - last));
		last = at + delim.size();
	}
	return elements;
}

void Network::SendConnectMessage(string ip)
{
	cout << "sending connect message to " << ip << " : " << mTcpPort << endl;
	sf::Socket::Status status = mTcpSocket.connect(ip, mTcpPort);
	if (status == sf::Socket::Done)
	{
		cout << "connect message sent" << endl;
		mConnected = true;
		mConnectedIp = ip;
		mHost = false;
	}
	else
	{
		cout << "connect message failed" << endl;
	}
}

void Network::StartTcpListen()
{
	cout << "listening on " << mTcpPort << endl;
	mListener.listen(mTcpPort);
	if (!mConnected)
	{
		if (mListener.accept(mTcpSocket) == sf::Socket::Done)
		{
			mConnected = true;
			sf::IpAddress ip = mTcpSocket.getRemoteAddress();
			mConnectedIp = AddressToString(ip);
			cout << "connect message recieved" << endl;
			mHost = true;
		}
		else
			cout << "connect message not recieved" << endl;
	}
}

void Network::SendUdp(sf::Packet packet)
{
	if(mConnected)
	if (mUdpSocket.send(packet, sf::IpAddress(mConnectedIp), mPort2) != sf::Socket::Done)
	{
		cout << "send failed" << endl;
	}
	else
	{
		cout << "Sending to: " << mConnectedIp << ", Port: " << mPort2 << ", Size: " << packet.getDataSize() << endl;
	}
}

sf::Packet Network::UdpListen()
{
	mUdpSocket.receive(mPacket, sf::IpAddress(mConnectedIp), mPort);
	if(mPacket.getDataSize() > 0)
		cout << "Recieving from: " << mConnectedIp << ", Port: " << mPort << ", Size: " << mPacket.getDataSize() << endl;
	return mPacket;
}

std::map<string, string> Network::ResultToMap(string s)
{
	std::map<string, string> map;
	s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
	s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
	std::vector<string> keyvalues = Split(s, ",");
	for each (string kv in keyvalues)
	{
		std::vector<string> keyvalue = Split(kv, ",");
		map[keyvalue[0]] = keyvalue[1];
	}
	return map;
}

string Network::GetIP()
{
	mReadBuffer.clear();
	mHandle = curl_easy_init();
	curl_easy_setopt(mHandle, CURLOPT_URL, "http://checkip.dyndns.org/");
	curl_easy_setopt(mHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(mHandle, CURLOPT_WRITEDATA, &mReadBuffer);
	CURLcode result = curl_easy_perform(mHandle);
	curl_easy_cleanup(mHandle);
	int i = mReadBuffer.find(": ", 0);
	return mReadBuffer.substr(i + 2, mReadBuffer.find("<", i) - (i + 2));
}

string Network::Post(string url, std::vector<std::array<string, 2>> args)
{
	mReadBuffer.clear();
	string arg = ArgsToString(args);
	mHandle = curl_easy_init();
	curl_easy_setopt(mHandle, CURLOPT_URL, (mUrl + url).c_str());
	curl_easy_setopt(mHandle, CURLOPT_POSTFIELDS, arg.c_str());
	curl_easy_setopt(mHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(mHandle, CURLOPT_WRITEDATA, &mReadBuffer);
	CURLcode result = curl_easy_perform(mHandle);
	curl_easy_cleanup(mHandle);
	return mReadBuffer;
}

string Network::Get(string url, std::vector<std::array<string, 2>> args)
{
	mReadBuffer.clear();
	string arg = ArgsToString(args);
	mHandle = curl_easy_init();
	curl_easy_setopt(mHandle, CURLOPT_URL, (mUrl + url + arg).c_str());
	curl_easy_setopt(mHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(mHandle, CURLOPT_WRITEDATA, &mReadBuffer);
	CURLcode result = curl_easy_perform(mHandle);
	curl_easy_cleanup(mHandle);
	return mReadBuffer;
}

string Network::ArgsToString(std::vector<std::array<string, 2>> args)
{
	string s = "";
	if(args.size() > 0)
		s += args[0][0] + "=" + args[0][1];
	for (int i = 1; i < args.size();i++)
		s += "&" + args[i][0] + "=" + args[i][1];
	return s;
}

CURL *Network::mHandle;
string Network::mUrl;
std::string Network::mReadBuffer;
sf::UdpSocket Network::mUdpSocket;
char Network::mData[100];
std::size_t Network::mReceived;
UINT16 Network::mPort;
UINT16 Network::mPort2;
UINT16 Network::mTcpPort;
sf::TcpListener Network::mListener;
sf::TcpSocket Network::mTcpSocket;
bool Network::mConnected;
string Network::mConnectedIp;
sf::Packet Network::mPacket;
bool Network::mHost;