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

// 받은 msg의 Type을 반환
int RecvClassify(char* msg);

// SendFunc : 전송할 값들을 매개변수로 사용

// [고대(비젼) > 고대(전략)] 스톤 갯수
void SendPacketStoneCnt(SOCKET s, int cnt, Point* p);
// [고대(비젼) > 고대(전략)]스톤의 위치
void SendPacketStoneInfo(SOCKET s, Point p);

// [고대(비젼) > DGIST] 투구 중 로봇 각도 위치
void SendPacketRobotInfo(SOCKET s, float angle, Point pos, float hogDist, float hogOffs);

// [고대(전략) > 고대(비젼)] 스톤 정보 응답
void SendPacketStoneInfoAck(SOCKET s);

// [고대(전략) > 영남대] 릴리즈 속력, 각도, 컬방향, (위치, 목표 지점)
void SendPacketRelease(SOCKET s, float speed, float angle, bool isRight, Point pos, Point tar_pos);

// [영남대 > 고대(비전)] 투구 플래그
void SendPacketThrowFlag(SOCKET s);

// [영남대 > DGIST]투구 중 로봇 각도, 속도 프로파일
void SendPacketSpeedProf(SOCKET s, float angle, float time);

// [DGIST > 영남대] 가속도 프로파일
void SendPacketAccelProf(SOCKET s, Point pos, float accel);

// [영남대 > DGIST] Reset
void SendPacketReset(SOCKET s, float reset, float empty);

// [영남대 > DGIST] Drive wait
void SendPacketDriveWait(SOCKET s, float wait);

// [영남대 > DGIST] mode
void SendPacketmode(SOCKET s, float mode, float empty);

// [영남대 > DGIST] Speed
void SendPacketSpeed(SOCKET s, float speed);

// [영남대 > DGIST] Drive start
void SendPacketDrivestart(SOCKET s, float start, float empty);

// [영남대 > DGIST] Drive time
void SendPacketDrivetime(SOCKET s, float time);




// RecvFunc : 값을 입력받을 변수를 매개변수로 사용

// [고대(비젼) > 고대(전략)] 스톤 갯수
void RecvPacketStoneCnt(char* msg, int &cnt);
// [고대(비젼) > 고대(전략)] 스톤의 위치
int RecvPacketStoneInfo(char* msg, Point &pos);

// [고대(비젼) > DGIST] 투구 중 로봇 각도 위치
void RecvPacketRobotInfo(char* msg, float &angle, Point &pos, float &hogDist, float &hogOffs);

// [고대(전략) > 영남대] 릴리즈 속력, 각도, 컬방향, (위치, 목표 지점)
void RecvPacketRelease(char* msg, float &speed, float &angle, bool &isRight, Point &pos, Point &tar_pos);

// [영남대 > 고대(비전)] 지금 받은 packet이 투구 플래그인지 반환
bool isThrowFlag(char* msg);

// [영남대 > DGIST] 투구 중 로봇 각도, 속도 프로파일
void RecvPacketSpeedProf(char* msg, float &angle, float &speed);

// [DGIST > 영남대] 각속도 프로파일
void RecvPacketAccelProf(char* msg, Point &pos, float &accel);