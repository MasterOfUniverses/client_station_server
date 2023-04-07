//for input-output
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

//for sockets
#include <atlsocket.h>
#pragma warning(suppress : 4996)
#pragma comment(lib, "Ws2_32.lib")
#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>

//for timeouts
#include <Windows.h>
//for asynhrone and non-block io
#include <fcntl.h>
#include <random> //for generator of commands
//for types
#include <string.h>
#include <set>
#include <errno.h>
#include <sys/types.h>

using namespace std;

const int STATION_PORT = 6666;
const int BUFLEN = 4;

const string OUT_FILE_PATH = "output.txt";
struct command {
	public:
		char length[2];
		int number;
		char name[2];
		string data;
};
int readFromStation(int fd, ofstream& out);

int main()
{
	//winsock initialization
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed\n";
		return -1;
	}
	//open ouput file
	ofstream out;
	out.open("output.txt");


	const char* serverName;
	serverName = "127.0.0.1";
	int cycle_count_1, cycle_count_2;
	int err, sock;
	srand(time(0));
	//reading entering data
	//commands in one request //in-cycle
	cout << "Enter number of commands in one request :" << endl;
	cin >> cycle_count_1;
	//number of requests //out-cycle
	cout << "Enter number of requests :" << endl;
	cin >> cycle_count_2;

	
	sock = socket(PF_INET, SOCK_STREAM, 0); //create TCP socket for station
	if (sock < 0) {
		perror("Client: socket was not created");
		exit(EXIT_FAILURE);
	}
	// fill out server addres struct
	struct sockaddr_in server_addr;
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(STATION_PORT);
	unsigned int iaddr;
	inet_pton(AF_INET, serverName, &iaddr);
	server_addr.sin_addr.s_addr = iaddr;
	// try to connect with station
	err = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (err < 0) {
		perror("Client:  connect failure");
		exit(EXIT_FAILURE);
	}
	bool l = true; //for non-block reading from station
	if (SOCKET_ERROR == ioctlsocket(sock, FIONBIO, (unsigned long*)&l))
	{
		// Error
		int res = WSAGetLastError();
		return -1;
	}
	cout << endl << "Connection is ready" << endl;
	//fill out struct for Poll function
	struct pollfd fds;
	fds.fd = sock;
	fds.events = POLLIN;
	fds.revents = 0;



	for (int k = 0; k < cycle_count_2; ++k) {
		vector <command> commands;//create request vector
		for (int i = 0; i < cycle_count_1; i++) {//generate one command
			command temp;
			int random_length = rand() % 20+10;
			temp.length[0] = (char)0;
			temp.length[1] = (char)(random_length - 2);
			int random_number = rand();
			temp.number = random_number;
			temp.name[0]=(char)(rand() % 25 + 65);
			temp.name[1] =(char)(rand() % 25 + 65);
			random_length -= 8;
			string c_data="";
			for (int j = 0; j < random_length; ++j) {
				c_data += (char)(rand() % 25 + 65);
			}
			temp.data = c_data;
			commands.push_back(temp);//add to request
		}
		
		for (int i=0;i<commands.size();i++){//send request vector
			err = send(sock, commands[i].length, 2, 0);
			if (err < 0) {
				cout << "Send err" << endl;
				return -1;
			}
			err = send(sock,(char*)(&commands[i].number), sizeof(int), 0);
			if (err < 0) {
				cout << "Send err" << endl;
				return -1;
			}
			err = send(sock, commands[i].name, 2, 0);
			if (err < 0) {
				cout << "Send err" << endl;
				return -1;
			}
			err = send(sock, commands[i].data.c_str(),commands[i].data.size() , 0);
			if (err < 0) {
				cout << "Send err" << endl;
				return -1;
			}
			//print request
			cout << commands[i].length[0]<< commands[i].length[1]<< commands[i].number<< commands[i].name[0] << commands[i].name[1] << commands[i].data<<' ';
		}
		cout << endl;
		commands.clear(); //clear request
		int ret = WSAPoll(&fds, 1, 100); //if we have some answer - trying to read
		if (ret > 0) {
			readFromStation(sock, out);
		}
		Sleep((rand() % 50000)+5000); //sleep to next request
	}
	int ret = WSAPoll(&fds, 1, -1);//sleep until we haven't input data
	if (ret > 0) {//if we have input data
		readFromStation(sock, out);//read this data
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}
int readFromStation(int fd, ofstream& out) {
	int   nbytes;
	char  buf_arr[BUFLEN];
	stringstream buf("");
	buf << noskipws;
	//try to read input data
	nbytes = recv(fd, buf_arr, BUFLEN, 0);
	if (nbytes < 0) {
		// read err
		out << "- recv err : please try again\n";
		return -1;
	}
	else if (nbytes == 0) {
		// no data
		out << "- no message" << std::endl;
		return 0;
	}
	else {
		//correct reading
		while (nbytes > 0) {//while reading is correct
			//add to ss_buf
			for (int i = 0; i < nbytes /* (sizeof(char))*/; i++) {
				buf << buf_arr[i];
			}
			//if buf isn't full - it's a tail of data
			if (nbytes < BUFLEN) {
				break;
			}
			//read new data
			nbytes = recv(fd, buf_arr, BUFLEN, 0);
		}
	}
	string result;
	result = buf.str();
	//print result in output stream
	out << result;
	return 1;
}