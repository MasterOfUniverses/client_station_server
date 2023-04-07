//for input-ouput
#include <iostream>
#include <fstream>
#include <sstream>
//for types
#include <string>
#include <set>
#include <vector>
#include <ctype.h>
#include <sys/types.h>
#include <errno.h>
//for windows sockets
//#include <winsock.h>
#include <winsock2.h>
//#include <ws2tcpip.h>
#include <atlsocket.h>
#pragma warning(suppress : 4996)
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <time.h>//for working with time - non actual?
#include <Windows.h>//for timeouts
#include <fcntl.h>//for asynhrone and non-block io
//#include <thread>

using namespace std;

struct command {
	public:
	char length[2];
	int number;
	char name[2];
	string data;

};


int ReadFromClient(int fd, stringstream& ss);
void  writeToClient(int fd, stringstream& ss);
const int TOTAL_SOCKET_COUNT = 2000;
const int SLEEP_TIME = 5;
const int TOTAL_CYCLE_COUNT = 1200000;
const int BUFLEN = 2049;
const int SERVER_PORT = 5555;

const set<string> acceptable_commands = {
	"EA"s,
	"EB"s,
	"EC"s,
	"ED"s,
	"EE"s,
	"EF"s,
	"EG"s,
	"EH"s,
	"EI"s,
	"EJ"s,
	"EK"s,
	"EL"s,
	"EM"s,
	"EN"s,
	"EO"s,
	"EP"s,
	"EQ"s,
	"ER"s,
	"ES"s,
	"ET"s,
	"EU"s,
	"EV"s,
	"EW"s,
	"FA"s,
	"FB"s,
	"FC"s,
	"FD"s,
	"FE"s,
	"FF"s,
	"FG"s,
	"FH"s,
	"FI"s,
	"FJ"s,
	"FK"s,
	"FL"s,
	"FM"s,
	"FN"s,
	"FO"s,
	"FP"s,
	"FQ"s,
	"FR"s,
	"FS"s,
	"FT"s,
	"FU"s,
	"FV"s,
	"FW"s,
	"GA"s,
	"GB"s,
	"GC"s,
	"GD"s,
	"GE"s,
	"GF"s,
	"GG"s,
	"GH"s,
	"GI"s,
	"GJ"s,
	"GK"s,
	"GL"s,
	"GM"s,
	"GN"s,
	"GO"s,
	"GP"s,
	"GQ"s,
	"GR"s,
	"GS"s,
	"GT"s,
	"GU"s,
	"GV"s,
	"GW"s,
	"HA"s,
	"HB"s,
	"HC"s,
	"HD"s,
	"HE"s,
	"HF"s,
	"HG"s,
	"HH"s,
	"HI"s,
	"HJ"s,
	"HK"s,
	"HL"s,
	"HM"s,
	"HN"s,
	"HO"s,
	"HP"s,
	"HQ"s,
	"HR"s,
	"HS"s,
	"HT"s,
	"HU"s,
	"HV"s,
	"HW"s,
	"IA"s,
	"IB"s,
	"IC"s,
	"ID"s,
	"IE"s,
	"IF"s,
	"IG"s,
	"IH"s,
	"II"s,
	"IJ"s,
	"IK"s,
	"IL"s,
	"IM"s,
	"IN"s,
	"IO"s,
	"IP"s,
	"IQ"s,
	"IR"s,
	"IS"s,
	"IT"s,
	"IU"s,
	"IV"s,
	"IW"s,
	"JA"s,
	"JB"s,
	"JC"s,
	"JD"s,
	"JE"s,
	"JF"s,
	"JG"s,
	"JH"s,
	"JI"s,
	"JJ"s,
	"JK"s,
	"JL"s,
	"JM"s,
	"JN"s,
	"JO"s,
	"JP"s,
	"JQ"s,
	"JR"s,
	"JS"s,
	"JT"s,
	"JU"s,
	"JV"s,
	"JW"s,
	"KA"s,
	"KB"s,
	"KC"s,
	"KD"s,
	"KE"s,
	"KF"s,
	"KG"s,
	"KH"s,
	"KI"s,
	"KJ"s,
	"KK"s,
	"KL"s,
	"KM"s,
	"KN"s,
	"KO"s,
	"KP"s,
	"KQ"s,
	"KR"s,
	"KS"s,
	"KT"s,
	"KU"s,
	"KV"s,
	"KW"s,
	"LA"s,
	"LB"s,
	"LC"s,
	"LD"s,
	"LE"s,
	"LF"s,
	"LG"s,
	"LH"s,
	"LI"s,
	"LJ"s,
	"LK"s,
	"LL"s,
	"LM"s,
	"LN"s,
	"LO"s,
	"LP"s,
	"LQ"s,
	"LR"s,
	"LS"s,
	"LT"s,
	"LU"s,
	"LV"s,
	"LW"s,
	"MA"s,
	"MB"s,
	"MC"s,
	"MD"s,
	"ME"s,
	"MF"s,
	"MG"s,
	"MH"s,
	"MI"s,
	"MJ"s,
	"MK"s,
	"ML"s,
	"MM"s,
	"MN"s,
	"MO"s,
	"MP"s,
	"MQ"s,
	"MR"s,
	"MS"s,
	"MT"s,
	"MU"s,
	"MV"s,
	"MW"s,
	"NA"s,
	"NB"s,
	"NC"s,
	"ND"s,
	"NE"s,
	"NF"s,
	"NG"s,
	"NH"s,
	"NI"s,
	"NJ"s,
	"NK"s,
	"NL"s,
	"NM"s,
	"NN"s,
	"NO"s,
	"NP"s,
	"NQ"s,
	"NR"s,
	"NS"s,
	"NT"s,
	"NU"s,
	"NV"s,
	"NW"s,
	"OA"s,
	"OB"s,
	"OC"s,
	"OD"s,
	"OE"s,
	"OF"s,
	"OG"s,
	"OH"s,
	"OI"s,
	"OJ"s,
	"OK"s,
	"OL"s,
	"OM"s,
	"ON"s,
	"OO"s,
	"OP"s,
	"OQ"s,
	"OR"s,
	"OS"s,
	"OT"s,
	"OU"s,
	"OV"s,
	"OW"s,
	"PA"s,
	"PB"s,
	"PC"s,
	"PD"s,
	"PE"s,
	"PF"s,
	"PG"s,
	"PH"s,
	"PI"s,
	"PJ"s,
	"PK"s,
	"PL"s,
	"PM"s,
	"PN"s,
	"PO"s,
	"PP"s,
	"PQ"s,
	"PR"s,
	"PS"s,
	"PT"s,
	"PU"s,
	"PV"s,
	"PW"s,
	"^C"s
};

int main()
{

	int		err, opt = 1;
	int     sock,sock2;
	struct  sockaddr_in  addr;
	struct  sockaddr_in  client;
	BOOL l = TRUE;
	
	cout << noskipws;
	// winsock initialization
	WSADATA wsaData; 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed" << endl;
		return 1;
	}

	//create sockets
	sock = socket(PF_INET, SOCK_STREAM, 0); //for listen stations
	sock2 = socket(PF_INET, SOCK_STREAM, 0); //for connection with station
	if (sock < 0) {
		perror("Server: cannot create socket");
		exit(EXIT_FAILURE);
	}
	if (sock2 < 0) {
		perror("Server: cannot create socket");
		exit(EXIT_FAILURE);
	}


	// fill out addres struct
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	err = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	if (err < 0) {
		perror("Server: cannot bind socket");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	//if server will fall
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)))
	{
		cout << endl << "setsockopt(SO_REUSEADDR) failed with error: " << WSAGetLastError();
		return -1;
	}
	// create queue for 10 connections
	err = listen(sock, 10);
	if (err < 0) {
		perror("Server: listen queue failure");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	struct pollfd fds[TOTAL_SOCKET_COUNT];
	fds[0].fd = sock;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int num_set=1;
	
	while(1){
		int ret = WSAPoll(fds, num_set, -1);

		// if Poll err
		if (ret == -1) {

			cerr << "poll err" << endl;
		}
		// ошибка
		else if (ret == 0) { //if no data
			//Sleep(SLEEP_TIME);
		}
		// таймаут, событий не произошло
		else {
			// if any activity
			if (fds[0].revents & POLLIN) {
				fds[0].revents = 0;
				//accept query to connect
				int size = sizeof(client);
				sock2 = accept(sock, (struct sockaddr*)&client, &size);
				if (sock2 < 0) {
					perror("Server: accept failure ");
				}
				//if we have free chanales
				if (num_set < TOTAL_SOCKET_COUNT) {
					fds[num_set].fd = sock2;
					fds[num_set].events = POLLIN;
					fds[num_set].revents = 0;
					//for non-block read from station
					if (SOCKET_ERROR == ioctlsocket(fds[num_set].fd, FIONBIO, (unsigned long*)&l))
					{
						// Error
						int res = WSAGetLastError();
						return -1;
					}
					//increase number of clients,which has been active some time
					++num_set;
					//cout << num_set<<endl;
				}
				//if we can't add new client
				else {
					cout << "all channels had been used" << endl;
				}
			}
			//if we have activity from one of clients
			for (int j = 1; j < num_set; j++) {
				if (fds[j].revents & POLLIN) {
					fds[j].revents = 0;
					cout <<"activity on fd :" <<j << endl;
					stringstream buf("");
					buf << noskipws; //for reading all special symbols
					err = ReadFromClient(fds[j].fd, buf);

					Sleep(1100);
					//imitating of server working
					if (err < -2) {//err
						closesocket(fds[j].fd);
						std::cout << "close socket = " << fds[j].fd << endl;
					}
					else if (err == -2) { //no more data
						//std::cout << "reading from station: no more data" << endl;
					}
					else if (err == -1) {//unacceptable command
						std::cout << "writing to client: err" << endl;
						writeToClient(fds[j].fd, buf);
					}
					else if (err == 0) { //command == stop
						std::cout << "writing to client: stop" << endl;
						closesocket(fds[j].fd);
						std::cout << "close socket = " << fds[j].fd << endl;
					}
					else {//acceptable command
						std::cout << "writing to client: OK" << endl;
						writeToClient(fds[j].fd, buf);
					}
				}
			}
		}
	}
	WSACleanup();
	return 0;
}


int ReadFromClient(int fd,stringstream& ss) {
	char buf[BUFLEN];
	int  nbytes=1;
	long int command_length=0;
	struct command com;
	buf[0] = '\x00';
	buf[1] = '\x00';
	//try to read command length
	nbytes = recv(fd, (char*) &buf, 2, 0);
	cout << "nbytes = " << nbytes << endl;
	if (nbytes < 0) {
		// reading err
		perror("Server: read failure");
		return -3;
	}
	else if (nbytes == 0) {
		//no data
		return -2;
	}
	else {
		if (nbytes == 2) {//if reading of command length is correct
			command_length = 256 * ((int)buf[0]) + ((int)buf[1]);
		}
		else {//if reading of command length is incorrect
			//ss.str("");
			//ss << "- read: cannot read command length\n"s;
			return -2;
		}

	}
	if (command_length > 0) {//if incorrect command length
		com.length[0] = buf[0];
		com.length[1] = buf[1];

		while (command_length > 0) {//so far, not every has read

			nbytes = recv(fd, (char*)&buf, min(BUFLEN, command_length), 0);
			if(nbytes<0){// if reading err
				ss.str("");
				ss << "- wrong command length: can't read data\n"s;
				std::cout << ss.str();
				return -1;
			}
			else if (nbytes == 0) {
				// no data - break cycle - correct err with no consequences
				break;
			}
			else {
				// if we have data - add to ss_buf
				for (int i = 0; i < nbytes / (sizeof(char)); i++) {
					ss << buf[i];
				}
			}
			//size of tail of data
			command_length = command_length - nbytes;
		}
		string data = ss.str();
		char char_number[sizeof(int)];//read from buf number of command
		for (int i = 0; i < sizeof(int); i++) {
			char_number[i] = data[i];
		}
		int number = *((int*)char_number);
		data.erase(0, sizeof(int));//edit buf
		string command = ""s + data[0] + data[1];//read from buf name of command
		data.erase(0, 2);//edit buf
		ss.str("");
		int flag = 0;

		if (acceptable_commands.count(command) > 0) {//if command name is acceptable
			flag = 1;
			std::cout << "acceptable command:" << endl;
			std::cout << "number:" << endl;
			std::cout << number << endl;
			std::cout << "command:" << endl;
			std::cout << command << endl;
			std::cout << "with data: " << endl;
			std::cout << data << endl;
			std::cout << endl;
			ss << number << command << data << endl;
			if (command == "^C"s) { //if it's stop command
				return 0;
			}
			return 1; //if it's acceptable and non-stop command - replay to client
		}
		else {//if command isn't acceptable
			ss.str("");
			ss << "- unindefinited command name: "s + command + '\n';//replay to client
			cout << ss.str();
			return -1;
		}

	}
	else {//if command length is wrong
		ss << "- wrong command length: command length < 0\n"s;//replay to client
		cout << ss.str() << endl;
		return -1;
	}
	return -3;//if some err
}


void  writeToClient(int fd, stringstream& ss) 
{
	int   nbytes;
	int	  i = 0;
	nbytes = send(fd, ss.str().c_str(), ss.str().size(), 0);
	ss.str("");//clear replay string
	if (nbytes < 0) { perror("write"); return; } //if send err
	return ;
}