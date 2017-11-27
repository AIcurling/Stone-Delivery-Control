#include "stdafx.h"
#include "Packet.h"

void SendPacketStoneCnt(SOCKET s, int cnt, Point* posArr) {
	char* data = new char[3 + 1];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : STONE_CNT
	data[idx++] = STONE_CNT;

	// Set Data : Stone cnt
	data[idx++] = (char)cnt;

	send(s, data, idx, 0);

	for (int i = 0; i < cnt; i++) {
#ifdef WIN32
		Sleep(100);
#else
		usleep(100000);
#endif
		SendPacketStoneInfo(s, posArr[i]);
	}

	delete data;
}

void SendPacketStoneInfo(SOCKET s, Point pos) {
	char* data = new char[3 + sizeof(pos)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : STONE_INFO
	data[idx++] = STONE_INFO;

	// Set Data : Stone Info
	char * temp = (char*)&pos.x;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];
	temp = (char*)&pos.y;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	send(s, data, idx, 0);

	delete data;
}

void SendPacketRobotInfo(SOCKET s, float angle, Point pos, float hogDist, float hogOffs) {
	unsigned char checkSum = 0;
	char* data = new char[3 + sizeof(angle) + sizeof(pos) + sizeof(hogDist) + sizeof(hogOffs) + sizeof(checkSum)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : ROBOT_INFO
	data[idx++] = ROBOT_INFO;

	// Set Data : Robot Info
	char * temp = (char*)&angle;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	temp = (char*)&pos.x;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];
	temp = (char*)&pos.y;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	temp = (char*)&hogDist;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	temp = (char*)&hogOffs;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	for (int l = 0; l < idx; l++)
		checkSum |= data[l];

	data[idx++] = checkSum;

	send(s, data, idx, 0);

	delete data;
}

void SendPacketStoneInfoAck(SOCKET s) {
	char data[3];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : FLAG
	data[idx++] = STONE_INFO_ACK;

	send(s, data, idx, 0);
}

void SendPacketRelease(SOCKET s, float speed, float angle, bool isRight, Point pos, Point tar_pos) {
	char* data = new char[3 + sizeof(speed) + sizeof(angle) + sizeof(isRight) + sizeof(pos) + sizeof(tar_pos)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : RELEASE
	data[idx++] = RELEASE;

	// Set Data : speed, angle, isRight, position, target
	char * temp = (char*)&speed;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	temp = (char*)&angle;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	data[idx++] = isRight;

	temp = (char*)&pos.x;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];
	temp = (char*)&pos.y;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	temp = (char*)&tar_pos.x;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];
	temp = (char*)&tar_pos.y;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	send(s, data, idx, 0);

	delete data;
}

void SendPacketThrowFlag(SOCKET s) {
	char data[3];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : FLAG
	data[idx++] = FLAG;

	send(s, data, idx, 0);
}

void SendPacketSpeedProf(SOCKET s, float angle, float speed) {
	char* data = new char[3 + sizeof(angle) + sizeof(speed)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : SPEED_PROF
	data[idx++] = SPEED_PROF;

	// Set Data : Angle & Speed Profil
	char * temp = (char*)&angle;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	temp = (char*)&speed;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	send(s, data, idx, 0);

	delete data;
}

void SendPacketAccelProf(SOCKET s, Point pos, float accel) {
	char* data = new char[3 + sizeof(pos) + sizeof(accel)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : ACCEL_PROF
	data[idx++] = ACCEL_PROF;

	// Set Data : Accel Profil
	char * temp = (char*)&pos.x;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];
	temp = (char*)&pos.y;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	temp = (char*)&accel;
	data[idx++] = temp[0];
	data[idx++] = temp[1];
	data[idx++] = temp[2];
	data[idx++] = temp[3];

	send(s, data, idx, 0);

	delete data;
}

void SendPacketReset(SOCKET s, float reset, float empty)
{
	char* data = new char[3 + sizeof(reset) + sizeof(empty)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : SPEED_PROF
	data[idx++] = Reset;

	// Set Data : resetl
	char * temp = (char*)&reset;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	temp = (char*)&empty;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	send(s, data, idx, 0);

	delete data;
}

void SendPacketDriveReady(SOCKET s, float ready)
{
	char* data = new char[3 + sizeof(ready)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : Drive ready
	data[idx++] = Drive_wait;

	// Set Data : 100
	char * temp = (char*)&ready;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	send(s, data, idx, 0);

	delete data;
}
// [영남대 > DGIST] Angle
void SendPacketmode(SOCKET s, float mode, float empty)
{
	char* data = new char[3 + sizeof(mode) + sizeof(empty)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : SPEED_PROF
	data[idx++] = Mode;

	// Set Data : model
	char * temp = (char*)&mode;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	temp = (char*)&empty;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	send(s, data, idx, 0);

	delete data;
}

// [영남대 > DGIST] Speed
void SendPacketSpeed(SOCKET s, float speed)
{
	char* data = new char[3 + sizeof(speed)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : SPEED_PROF
	data[idx++] = Speed;

	char * temp = (char*)&speed;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	send(s, data, idx, 0);

	delete data;
}

// [영남대 > DGIST] Drive start
void SendPacketDrivestart(SOCKET s, float start, float empty)
{
	char* data = new char[3 + sizeof(start) + sizeof(empty)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : SPEED_PROF
	data[idx++] = Drive_start;

	// Set Data : start
	char * temp = (char*)&start;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	temp = (char*)&empty;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	send(s, data, idx, 0);

	delete data;
}

// [영남대 > DGIST] Drive time
void SendPacketDrivetime(SOCKET s, float time)
{
	char* data = new char[3 + sizeof(time)];
	int idx = 0;

	// Header : 0xAA, 0x00 설정
	data[idx++] = 0xAA;
	data[idx++] = 0x00;

	// Packet Type : SPEED_PROF
	data[idx++] = Drive_time;

	// Set Data : Angle & Speed Profil
	char * temp = (char*)&time;
	data[idx++] = temp[3];
	data[idx++] = temp[2];
	data[idx++] = temp[1];
	data[idx++] = temp[0];

	send(s, data, idx, 0);

	delete data;
}

int RecvClassify(char* msg) {
	if ((unsigned char)msg[0] != 0xAA || (unsigned char)msg[1] != 0x00)
		return -1;
	if (msg[2] < STONE_CNT || msg[2] > ACCEL_PROF)
		return -1;
	return msg[2];
}

void RecvPacketStoneCnt(char* msg, int &cnt) {
	cnt = msg[3];
}

int RecvPacketStoneInfo(char* msg, Point &pos) {
	PacketCvt tmp;
	int idx = 3;
	unsigned char checkSum = 0;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	pos.x = tmp.f;
	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	pos.y = tmp.f;

	for (int l = 0; l < idx; l++)
		checkSum |= msg[l];

	if (checkSum == msg[idx])
		return -1;
	else
		return 0;
}

void RecvPacketRobotInfo(char* msg, float &angle, Point &pos, float &hogDist, float &hogOff) {
	PacketCvt tmp;
	int idx = 3;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	angle = tmp.f;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	pos.x = tmp.f;
	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	pos.y = tmp.f;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	hogDist = tmp.f;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	hogOff = tmp.f;
}

void RecvPacketRelease(char* msg, float &speed, float &angle, bool &isRight, Point &pos, Point &tar_pos) {
	PacketCvt tmp;
	int idx = 3;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	speed = tmp.f;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	angle = tmp.f;

	isRight = msg[idx++];

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	pos.x = tmp.f;
	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	pos.y = tmp.f;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	tar_pos.x = tmp.f;
	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	tar_pos.y = tmp.f;
}

bool isThrowFlag(char* msg) {
	return (msg[2] == FLAG) ? true : false;
}

void RecvPacketSpeedProf(char* msg, float &angle, float &speed) {
	PacketCvt tmp;
	int idx = 3;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	angle = tmp.f;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	speed = tmp.f;
}

void RecvPacketAccelProf(char* msg, Point &pos, float &accel) {
	PacketCvt tmp;
	int idx = 3;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	pos.x = tmp.f;
	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	pos.y = tmp.f;

	for (int i = 0; i < 4; i++)
		tmp.ch[i] = msg[idx++];
	accel = tmp.f;
}