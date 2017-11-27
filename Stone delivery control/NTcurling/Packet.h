#pragma once
#include "common.h"

union PacketCvt {
	char ch[4];
	float f;
};

class Point {
public:
	float x;
	float y;

	Point(float x = 0, float y = 0) {
		this->x = x;
		this->y = y;
	}
};


enum { STONE_CNT, STONE_INFO, STONE_INFO_ACK, ROBOT_INFO, RELEASE, FLAG, SPEED_PROF, ACCEL_PROF, Reset, Drive_wait,Mode, Speed, Drive_start, Drive_time };

// ���� msg�� Type�� ��ȯ
int RecvClassify(char* msg);

// SendFunc : ������ ������ �Ű������� ���

// [���(����) > ���(����)] ���� ����
void SendPacketStoneCnt(SOCKET s, int cnt, Point* p);
// [���(����) > ���(����)]������ ��ġ
void SendPacketStoneInfo(SOCKET s, Point p);

// [���(����) > DGIST] ���� �� �κ� ���� ��ġ
void SendPacketRobotInfo(SOCKET s, float angle, Point pos, float hogDist, float hogOffs);

// [���(����) > ���(����)] ���� ���� ����
void SendPacketStoneInfoAck(SOCKET s);

// [���(����) > ������] ������ �ӷ�, ����, �ù���, (��ġ, ��ǥ ����)
void SendPacketRelease(SOCKET s, float speed, float angle, bool isRight, Point pos, Point tar_pos);

// [������ > ���(����)] ���� �÷���
void SendPacketThrowFlag(SOCKET s);

// [������ > DGIST]���� �� �κ� ����, �ӵ� ��������
void SendPacketSpeedProf(SOCKET s, float angle, float time);

// [DGIST > ������] ���ӵ� ��������
void SendPacketAccelProf(SOCKET s, Point pos, float accel);

// [������ > DGIST] Reset
void SendPacketReset(SOCKET s, float reset, float empty);

// [������ > DGIST] Drive wait
void SendPacketDriveWait(SOCKET s, float wait);

// [������ > DGIST] mode
void SendPacketmode(SOCKET s, float mode, float empty);

// [������ > DGIST] Speed
void SendPacketSpeed(SOCKET s, float speed);

// [������ > DGIST] Drive start
void SendPacketDrivestart(SOCKET s, float start, float empty);

// [������ > DGIST] Drive time
void SendPacketDrivetime(SOCKET s, float time);




// RecvFunc : ���� �Է¹��� ������ �Ű������� ���

// [���(����) > ���(����)] ���� ����
void RecvPacketStoneCnt(char* msg, int &cnt);
// [���(����) > ���(����)] ������ ��ġ
int RecvPacketStoneInfo(char* msg, Point &pos);

// [���(����) > DGIST] ���� �� �κ� ���� ��ġ
void RecvPacketRobotInfo(char* msg, float &angle, Point &pos, float &hogDist, float &hogOffs);

// [���(����) > ������] ������ �ӷ�, ����, �ù���, (��ġ, ��ǥ ����)
void RecvPacketRelease(char* msg, float &speed, float &angle, bool &isRight, Point &pos, Point &tar_pos);

// [������ > ���(����)] ���� ���� packet�� ���� �÷������� ��ȯ
bool isThrowFlag(char* msg);

// [������ > DGIST] ���� �� �κ� ����, �ӵ� ��������
void RecvPacketSpeedProf(char* msg, float &angle, float &speed);

// [DGIST > ������] ���ӵ� ��������
void RecvPacketAccelProf(char* msg, Point &pos, float &accel);