//for sockets
#include <atlsocket.h>
#pragma warning(suppress : 4996)
#pragma comment(lib, "Ws2_32.lib")
//#include <winsock.h>
#include <winsock2.h>
//#include <ws2tcpip.h>
//for input-output
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
//for types
#include <string.h>
#include <set>
#include <vector>
#include <queue>
//for time 
#include <ctime>
#include <errno.h>
#include <sys/types.h>


//for timeouts
//#include <Windows.h>
//for asynhrone and non-block io
#include <fcntl.h>

using namespace std;


struct command {
	public:
		char length[2];
		int number;
		char name[2];
		string data;

};
int  readFromServer(int fd, ostream& out);
int ReadFromClient(int fd, command& current_command);
int  writeToServer(int fd, queue<pair<command,int>>& in);
int ClearQueue(queue<pair<command, int>>& commands);
const int SERVER_PORT = 5555;
const int STATION_PORT = 6666;
const int TOTAL_CLIENTS_COUNT = 2000; //max number of clients at all
const int BUFLEN = 4;
const int TERMINATED_COMMAND_COUNT = 7; //max number of commands in one TERMINATED_TIME
const int TERMINATED_TIME_IN_SEC = 30;

//set of acceptable commands (which can go to server)
const set<string> acceptable_commands = {
	"AA"s,
	"AB"s,
	"AC"s,
	"AD"s,
	"AE"s,
	"AF"s,
	"AG"s,
	"AH"s,
	"AI"s,
	"AJ"s,
	"AK"s,
	"AL"s,
	"AM"s,
	"AN"s,
	"AO"s,
	"AP"s,
	"AQ"s,
	"AR"s,
	"AS"s,
	"AT"s,
	"AU"s,
	"AV"s,
	"AW"s,
	"BA"s,
	"BB"s,
	"BC"s,
	"BD"s,
	"BE"s,
	"BF"s,
	"BG"s,
	"BH"s,
	"BI"s,
	"BJ"s,
	"BK"s,
	"BL"s,
	"BM"s,
	"BN"s,
	"BO"s,
	"BP"s,
	"BQ"s,
	"BR"s,
	"BS"s,
	"BT"s,
	"BU"s,
	"BV"s,
	"BW"s,
	"CA"s,
	"CB"s,
	"CC"s,
	"CD"s,
	"CE"s,
	"CF"s,
	"CG"s,
	"CH"s,
	"CI"s,
	"CJ"s,
	"CK"s,
	"CL"s,
	"CM"s,
	"CN"s,
	"CO"s,
	"CP"s,
	"CQ"s,
	"CR"s,
	"CS"s,
	"CT"s,
	"CU"s,
	"CV"s,
	"CW"s,
	"DA"s,
	"DB"s,
	"DC"s,
	"DD"s,
	"DE"s,
	"DF"s,
	"DG"s,
	"DH"s,
	"DI"s,
	"DJ"s,
	"DK"s,
	"DL"s,
	"DM"s,
	"DN"s,
	"DO"s,
	"DP"s,
	"DQ"s,
	"DR"s,
	"DS"s,
	"DT"s,
	"DU"s,
	"DV"s,
	"DW"s,
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

int  main()
{
	const char* serverName;
	serverName = "127.0.0.1"; //localhost

	int err;
	int sock, sock2;
	int sock3;
	struct sockaddr_in server_addr;
	struct sockaddr_in  client;
	int x;
	bool l = true;
	//winsock initialization
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout<<"WSAStartup failed\n";
		return -1;
	}

	// fill out server addres struct
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	unsigned int iaddr;
	inet_pton(AF_INET, serverName, &iaddr);
	server_addr.sin_addr.s_addr = iaddr;

	// create TCP sockets
	sock = socket(PF_INET, SOCK_STREAM, 0); //for server
	sock2 = socket(PF_INET, SOCK_STREAM, 0); //for listening clients
	sock3 = socket(PF_INET, SOCK_STREAM, 0); //for connection clients
	if (sock < 0) {
		perror("Station: socket was not created");
		cin >> x;
		exit(EXIT_FAILURE);
	}
	if (sock2 < 0) {
		perror("Station: cannot create socket2");
		cin >> x;
		exit(EXIT_FAILURE);
	}
	if (sock3 < 0) {
		perror("Station: cannot create socket3");
		cin >> x;
		exit(EXIT_FAILURE);
	}
	// try to connect with server
	/*
	err = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (err < 0) {
		perror("Station:  connect failure");
	}
	*/
	struct  sockaddr_in  addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(STATION_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	err = bind(sock2, (struct sockaddr*)&addr, sizeof(addr)); //listening socket for clients
	if (err < 0) {
		perror("Station: cannot bind socket");
		WSACleanup();
		cin >> x;
		exit(EXIT_FAILURE);
	}
	err = listen(sock2, 10);
	if (err < 0) {
		perror("Station: listen queue failure");
		WSACleanup();
		cin >> x;
		exit(EXIT_FAILURE);
	}
	
	// Обмениваемся данными
	struct pollfd fd_clients[TOTAL_CLIENTS_COUNT+1]; //0-th struct to listening
	fd_clients[0].fd = sock2;
	fd_clients[0].events = POLLIN;
	fd_clients[0].revents = 0;
	int num_set=1;


	int count=0;
	int start = clock();
	vector<int>clocks;//time storage for last commands
	clocks.resize(TERMINATED_COMMAND_COUNT, (-1) * TERMINATED_TIME_IN_SEC * CLOCKS_PER_SEC); //fill out by - T_Time to correct work in the first cycle
	queue<pair<command, int>>commands; //command + socket
	//here will be main cycle
	int ret=0;
	int flag = -1;
	while (1) {
		//if no connection with server, trying to connect
		if (flag < 0) {
			flag = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
			if (flag < 0) {
				//perror("Station:  connect failure");
				/*
				for (int i = 0; i < num_set; i++) {
					cout << fd_clients[i].revents << endl;
				}
				*/
			}
			else {
				cout << "Connection is ready" << endl;
				flag = 1;
				//for non-block read from server
				if (SOCKET_ERROR == ioctlsocket(sock, FIONBIO, (unsigned long*)&l))
				{
					// Error
					int res = WSAGetLastError();
					cin >> x;
					return -1;
				}
			}
		}
		ret = WSAPoll(fd_clients, num_set, 100);
		if (ret < 0) {
			cerr << "Poll to client err\n";
		}
		else {
			if (fd_clients[0].revents & POLLIN) {
				fd_clients[0].revents = 0;
				int size = sizeof(client);
				sock3 = accept(sock2, (struct sockaddr*)&client, &size);
				if (sock3 < 0) {
					perror("Server: accept failure ");
				}
				if (num_set < TOTAL_CLIENTS_COUNT) { //if we can add new client
					fd_clients[num_set].fd = sock3;
					fd_clients[num_set].events = POLLIN;
					fd_clients[num_set].revents = 0;
					//for non-block read from client
					if (SOCKET_ERROR == ioctlsocket(fd_clients[num_set].fd, FIONBIO, (unsigned long*)&l))
					{
						// Error
						int res = WSAGetLastError();
						return -1;
					}
					//increase number of clients,which has been active some time
					++num_set;
				}
				//if we can't add new client
				else {
					cout << "all channels had been used" << endl;
				}

			}
			//if we have activity from one of clients
			for (int j = 1; j < num_set; j++) {
				if (fd_clients[j].revents & POLLIN) {
					fd_clients[j].revents = 0;
					cout << "activity on fd :" << j << endl;
					command current_command;
					string buf = "";
					//try to read
					err = ReadFromClient(fd_clients[j].fd, current_command);
					//if reading was correct
					while (err >= -1) {
						buf = current_command.data; //for error in correct reading
						clocks[count % clocks.size()] = clock(); //for time check
						count++;
						if (err > -1) {//if correct command - push to queue
							commands.push({ current_command,(int)fd_clients[j].fd });
						}
						else { //if incorrect command - notify client
							err = send(fd_clients[j].fd, buf.c_str(), buf.size(), 0);
							if (err < 0) {
								cerr << "Send to client err\n";
								closesocket(fd_clients[j].fd);
							}
						}
						buf = "";
						//old time check here was be
						
						//current time check
						int current_time_difference = clocks[(count - 1) % clocks.size()] - clocks[count % clocks.size()];
						if (current_time_difference < TERMINATED_TIME_IN_SEC * CLOCKS_PER_SEC) {
							//if too many commands - notify client, break connection with him and clear queue
							string replay = "too many commands in one time period\n";
							cout << replay << endl;
							ClearQueue(commands);
							send(fd_clients[j].fd, replay.c_str(), replay.size(), 0);
							Sleep(100);
							closesocket(fd_clients[j].fd);
							break;
						}
						err = ReadFromClient(fd_clients[j].fd, current_command);
						//cout << buf << endl;
					}
				}
				else if (fd_clients[j].revents == 3) { //if connection was closed by client
					closesocket(fd_clients[j].fd);
				}
			}
		}
		if (flag >= 0) { //if we have connection with server
			err = commands.size();
			struct pollfd fd_server;
			fd_server.fd = sock;
			fd_server.events = POLLIN;
			fd_server.revents = 0;
			while (err > 0) { //if we have commands to send
				int replay_fd = commands.front().second; //replay socket descripter
				err = writeToServer(fd_server.fd, commands); //try to write to server
				if (err < 0) { //if writing was invalid
					cerr << "Write to server err\n";
					stringstream replay("");
					replay << "Write to server err\n";
					for (int j = 1; j < num_set; j++) { //notify all clients
						err = send(replay_fd, replay.str().c_str(), replay.str().size(), 0);
						if (err < 0) {//if writing to client is incorrect
							cerr << "Send to client err\n";
							closesocket(replay_fd);
						}
					}
				}
				else if (err == 0) {//if queue is empty - correct err with no consequences
					break;
				}
				else if(err>0) { //if correct writing
					err = WSAPoll(&fd_server, 1, -1); //sleep until answer comes
					if (err > 0) {//if answer comes
						stringstream replay("");
						replay << noskipws;
						err = readFromServer(fd_server.fd, replay); //try to read answer
						if (err > 0) {//if reading is correct
							err = send(replay_fd, replay.str().c_str(), replay.str().size(), 0); //try to replay to client
							if (err < 0) {//if writing to client is incorrect
								cerr << "Send to client err\n";
								closesocket(replay_fd);
							}
						}
						else {//if reading is incorrect - some err on server - notify all clients
							cerr << "Read from server err\n";
							replay << "Read from server err\n";
							for (int j = 1; j < num_set; j++) {
								err = send(fd_clients[j].fd, replay.str().c_str(), replay.str().size(), 0);
								if (err < 0) {//if writing to client is incorrect
									cerr << "Send to client err\n";
									closesocket(fd_clients[j].fd);
								}
							}
							closesocket(sock);
							WSACleanup();
							cin >> x;
							exit(EXIT_FAILURE);
						}
					}
					else {//if Poll err - notify all clients
						stringstream replay("");
						cerr << "Poll to server err\n";
						replay << "Poll to server err\n";
						for (int j = 1; j < num_set; j++) {
							err = send(fd_clients[j].fd, replay.str().c_str(), replay.str().size(), 0);
							if (err < 0) {//if writing to client is incorrect
								cerr << "Send to client err\n";
								closesocket(fd_clients[j].fd);
							}
						}
						closesocket(sock);
						WSACleanup();
						cin >> x;
						exit(EXIT_FAILURE);
					}
				}
			}
			if (err == 0) {
				//cout << "Queue is empty\n";
			}
		}
	}
	//non actual as we have infinity cycle
	/*
	int end = clock();
	double t = (end - start) / CLOCKS_PER_SEC;
	cout << "Working time in sec: " << t << endl;
	 //for DEBUG
	cin >> x;
	cout<<"The end\n";

	closesocket(sock);
	WSACleanup();
	exit(EXIT_SUCCESS);
	*/
	return 0;
}
int ClearQueue(queue<pair<command, int>>& commands) {
	while (!commands.empty()) {
		commands.pop();
	}
	return commands.size();
}
int  writeToServer(int fd, queue<pair<command, int>>& in) //send only 1 element
{
	if (!in.empty()) {
		command com = in.front().first;
		in.pop();
		int nbytes = send(fd, com.length, 2, 0);
		if (nbytes < 0) {
			return -1;
		}

		nbytes = send(fd, (char*)(&com.number), sizeof(int), 0);
		if (nbytes < 0) {
			return -1;
		}

		nbytes = send(fd, com.name, 2, 0);
		if (nbytes < 0) {
			return -1;
		}

		nbytes = send(fd, com.data.c_str(), com.data.size(), 0);
		if (nbytes < 0) {
			return -1;
		}
		else {
			return 1;
		}
	}
	else {
		return 0;
	}
	return 0;
}


int  readFromServer(int fd, ostream& out) //read all
{
	int   nbytes;
	char  buf_arr[BUFLEN];
	stringstream buf("");
	buf << noskipws; //for reading all special symbols
	nbytes = recv(fd, buf_arr, BUFLEN, 0);
	if (nbytes < 0) {
		// reading err
		out<<"- recv err : please try again\n";
		return -1;
	}
	else if (nbytes == 0) {
		// no data
		out << "- no message \n";
		return 0;
	}
	else {
		//correct reading
		while (nbytes>0) { //while reading is correct
			//add to ss_buf
			for (int i = 0; i < nbytes /* (sizeof(char))*/; i++) {
				buf<<buf_arr[i];
			}
			//if buf isn't full - it's a tail of data
			if (nbytes < BUFLEN) {
				break;
			}
			//reading new data
			nbytes = recv(fd, buf_arr, BUFLEN, 0);
		}
	}
	string result;
	result = buf.str();
	std::cout << result << endl;
	out << result ;
	return 1;
}

int ReadFromClient(int fd,command& com) { //read only 1 element
	stringstream ss;
	ss << noskipws;
	char buf[BUFLEN];
	int  nbytes=1;
	long int command_length=0;
	buf[0] = '\x00';
	buf[1] = '\x00';
	//try to read command length
	nbytes = recv(fd, (char*) &buf, 2, 0);
	
	if (nbytes < 0) {
		// reading err
	//	perror("Station: read failure");
		return -2;
	}
	else if (nbytes == 0) {
		//no data
		return -3;
	}
	else {
		if (nbytes == 2) {//if reading of command length is correct
			command_length = 256 * ((int)buf[0]) + ((int)buf[1]);
		}
		else {//if reading of command length is incorrect
			ss << "- read: cannot read command length\n"s;
			com.data = ss.str();
			return -2;
		}
		

	}
	if (command_length > 0) {//if incorrect command length
		com.length[0] = buf[0];
		com.length[1] = buf[1];
		//cout << buf[0] << buf[1];
		while (command_length > 0) {//so far, not every has read

			nbytes = recv(fd, (char*)&buf, min(BUFLEN, command_length), 0);
			if (nbytes < 0) {//if reading err
				ss.str("");
				ss << "- wrong command length: can't read data\n"s;
				com.data = ss.str();
				cout << ss.str() << endl;
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
		char char_number[sizeof(int)]; //read from buf number of command
		for (int i = 0; i < sizeof(int); i++) {
			char_number[i] = data[i];
		}
		int number = *((int*)char_number);
		com.number = number;
		data.erase(0, sizeof(int));//edit buf
		string command = ""s + data[0] + data[1]; //read from buf name of command
		data.erase(0, 2);//edit buf
		int flag = 0;

		//print command
		/*
		cout << endl;
		cout << "number:"<<endl;
		cout << number << endl;
		cout << "command:" << endl;
		cout << command << endl;
		cout << "with data: " << endl;
		cout << data << endl;
		*/
		if (acceptable_commands.count(command) > 0) {//if command name is acceptable
			
			flag = 1;
			cout << "acceptable command" << endl;
			//print command
			cout << "number:" << endl;
			cout << number << endl;
			cout << "command:" << endl;
			cout << command << endl;
			cout << "with data: " << endl;
			cout << data << endl;
			cout << endl;
			
			com.name[0] = command[0];
			com.name[1] = command[1];
			com.data = data;
			if (command == "^C"s) { //if it's stop command - send to server to stopping it
				return 0;
			}
			return 1; //if it's acceptable command
		}
		else { //if command isn't acceptable
			ss.str("");
			ss << "- unindefinited command name: "s + command + '\n'; //replay to client
			//cout << ss.str() << endl;
			com.data = ss.str();
			return -1;
		}

	}
	else {//if command length is wrong
		ss << "- wrong command length: command length < 0\n"s;//replay to client
		cout << ss.str() << endl;
		com.data = ss.str();
		return -1;
	}
	
	return -2;
}


//old time check
/*
						int clocks_min = clock();
						int j_min = 0;
						for (int j = 0; j < clocks.size(); j++) {
							if ((clocks[j] < clocks_min) && (clocks[j] != start)) {
								clocks_min = clocks[j];
								j_min = j;
							}
						}

						//cout << count << "   " << j_min << endl;
						//int current_time_difference = (clocks[(count - 1) % clocks.size()] - clocks_min);
						//int current_command_count = (count - 1 + clocks.size() - j_min) % clocks.size();
						//cout << current_time_difference << "    " << current_command_count << endl;

						if ((current_time_difference < TERMINATED_TIME_IN_SEC * CLOCKS_PER_SEC) && (current_command_count > TERMINATED_COMMAND_COUNT)) {
							//flag = 1;
							string replay = "too many commands in one time period";
							cout << replay << endl;
							ClearQueue(commands);
							send(fd_clients[j].fd, replay.c_str(), replay.size(), 0);
							Sleep(100);
							closesocket(fd_clients[j].fd);
							break;
						}
						*/