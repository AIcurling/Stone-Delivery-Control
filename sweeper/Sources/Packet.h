#pragma once
#include "common.h"

// char[4] => float ��ȯ�� union
union PacketCvt {
	char ch[4];
	float f;
};
union PacketCvtd {
	char ch[8];
	double d;
};

// ��ġ ������ ������ Point ����ü
class Point {
public:
	float x;
	float y;

	Point(float x = 0, float y = 0) {
		this->x = x;
		this->y = y;
	}
};

// ��Ŷ 3��° ����Ʈ�� �� ��Ŷ�� ������ ������ 24(0~23)���� enum
enum {
	STONE_CNT, STONE_INFO, STONE_INFO_ACK, ROBOT_INFO, RELEASE, FLAG, SPEED_PROF, POS_PROF, RESET, MODE,
	RUN, RESET_KU, START, ROBOT_MODE, PRE_MODE, MY_TURN, CALL_STONE_INFO, CALL_ROBOT_INFO, EMERGENCY, RESTART,
	CALIB_DATA, TARGET_ANGLE, CALIB_ANGLE, HOG_DIST, RIO_ENCODER, INFO_TIME, INFO_RESULT, STR_PATH_CNT, REAL_PATH_TEST, REAL_SPEED, TEST_HACK, LASER_STONE, DIS1, DIS2, DIS3, DIS4, SEND_COMM, REAL_PATH, LASER_ON, LASER_OFF, READY,
	SWIP_ON,DIS5,STONE_DATA
};
//STR_PATH_CNT ����
//STR_PATH ���

// ���� msg�� Type�� ��ȯ
int RecvClassify(char* msg);

// SendFunc : ������ ������ �Ű������� ���

// [����(����) > ����(����)] ���� ����
void SendPacketStoneCnt(SOCKET s, int cnt, Point* p, bool* isRedArr);

// [����(����) > ����(����)]������ ��ġ
void SendPacketStoneInfo(SOCKET s, Point pos, bool isRed);

// [����(����) > DGIST] ���� �� �κ� ���� ��ġ
void SendPacketRobotInfo(SOCKET s, float angle, Point pos, float hogDist, float hogOffs);

// [����(����) > ����(����)] ���� ���� ����
void SendPacketStoneInfoAck(SOCKET s, char type);

// [����(����) > ������] ������ �ӷ�, ����, �ù���, (��ġ, ��ǥ ����)
void SendPacketRelease(SOCKET s, float speed, float angle, bool isRight, Point pos, Point tar_pos);

// [������ > ����(����)] ���� �÷���
void SendPacketThrowFlag(SOCKET s);

// [������ > DGIST] ���� �÷���
void SendPacketRioThrowFlag(SOCKET s);

// [������ > DGIST] ���� �� �κ� ����, �ӵ� ��������
void SendPacketSpeedProf(SOCKET s, float angle, float Speed);

// [������ > DGIST] ��ġ ��������
void SendPacketPositionProf(SOCKET s, float posx, float posy);

// [������ > DGIST] RIO Reset
void SendPacketRioreset(SOCKET s, float reset, float emp);

// [������ > DGIST] ������
void SendPacketRunningmode(SOCKET s, float mode, float emp);

// [������ > DGIST] �����غ�
void SendPacketpredrive(SOCKET s, float Preparation, float emp);

// [������ > DGIST] �������
void SendPacketDrivestart(SOCKET s, float start, float emp);

// [������ > DGIST] KU_RESET
void SendPacketKUReset(SOCKET s, float reset, float emp);

// [����(����) > ������] �����κ� ID
void SendPacketThrowmode(SOCKET s, float number);

// [����(����) > ������] �������
void SendPacketPremode(SOCKET s, float pre, float number);

// [����(����) > ������] �κ� ���� ����
void SendPacketMyturn(SOCKET s, bool turn);

// [����(����) > ����(����)] ���� ���� ȣ��
void SendPacketCallstoneinfo(SOCKET s);

// [������ > ����(����)] �κ� ���� ȣ��
void SendPacketCallrobotinfo(SOCKET s, bool isDist);

// [����(����) > ������] ��� ����
void SendPacketEMERGENCY(SOCKET s, float emp1, float emp2);

// [����(����) > ������] �����
void SendPacketRestart(SOCKET s, float emp1, float emp2);

// [����(����) > ������] Ķ���극�̼�
void SendPacketCalibdata(SOCKET s);

// [������ > DIGST] ��ǥ ���� ����
void SendPacketTargetangle(SOCKET s, float t_angle, float emp);

// [������ > DIGST] ���� ���� ����
void SendPacketCalibAngle(SOCKET s, float d1, float d2);

// [������ > ����(����)] �����Ǵ� ȣ�׶��� �Ÿ� ����
void SendPacketHogdistance(SOCKET s, float Hog_dist);

// [DGIST > ������] �����Ǵ� ȣ�׶��� �Ÿ� ����   // �߰� ���� 171210
void SendPacketRioencoder(SOCKET s, float Rio_encoder, float emp);

void SendPacketCRLF(SOCKET s);

// info time
void SendPacketInfoTime(SOCKET s, char * msg);

// info result
void SendPacketInfoResult(SOCKET s, float release, float arrive, Point pos);

// TEST_HACK
void SendPacketTestHackPos(SOCKET s, float test_ang, float test_speed);

//// ���� ��� ����
void SendPacketStrCnt(SOCKET s, float str_cnt);
//
//// ���� ���
//void SendPacketStrPath(SOCKET s, float x, float y, float speed);

// �ǽð� ���
void SendPacketrealPath(SOCKET s, float x, float y, float time);

// �ǽð� �ӵ�
void SendRealSpeed(SOCKET s, float real_speed);

// ���� ������
void SendStonedata(SOCKET s, bool find, float dist, float angle);

// RecvFunc : ���� �Է¹��� ������ �Ű������� ���

// [����(����) > ����(����)] ���� ����
void RecvPacketStoneCnt(char* msg, int &cnt);

// [����(����) > ����(����)] ������ ��ġ
int RecvPacketStoneInfo(char* msg, Point &pos, float &isRed);

// [����(����) > DGIST] ���� �� �κ� ���� ��ġ
void RecvPacketRobotInfo(char* msg, float &angle, Point &pos, float &hogDist, float &hogOffs);

// [����(����) > ������] ������ �ӷ�, ����, �ù���, (��ġ, ��ǥ ����)
void RecvPacketRelease(char* msg, float &speed, float &angle, bool &isRight, Point &pos, Point &tar_pos);

// [������ > ����(����)] ���� ���� packet�� ���� �÷������� ��ȯ
bool isThrowFlag(char* msg);

// [������ > DGIST] ���� �� �κ� ����, �ӵ� ��������
void RecvPacketSpeedProf(char* msg, float &angle, float &speed);

// [DGIST > ������] ���ӵ� ��������
void RecvPacketPositionProf(char* msg, float &posx, float &posy);

// [����(����) > ������] RESET_KU
void RecvPacketRioReset(char* msg, float &reset, float &emp);

// [����(����) > ������] �����κ� ID ����
void RecvPacketThrowmode(char* msg, float &mode);

// [����(����) > ������] �������, ����� ����
void RecvPacketPremode(char* msg, float &pre, float &mode);

// [����(����) > ������] �κ� ���� �������� ����
void RecvPacketMyturn(char* msg, bool &turn);

// [����(����) > ����(����)] ���� ���� ��û
bool RecvPacketCallstoneinfo(char* msg);

// [������ > ����(����)] �κ� ���� ��û
bool RecvPacketCallrobotinfo(char* msg);

// [����(����) > ������] ��� ����
bool RecvPacketEmergency(char* msg);

// [����(����) > ������] �����
bool RecvPacketRestart(char* msg);

// [����(����) > ������] Calibration data�� ���
bool RecvPacketCalibdata(char* msg);

// [������ > ����(����)] �����Ǵ� ȣ�׶��� �Ÿ� ����
void RecvPacketHogdistance(char* msg, float &Hog_dist);

// [DGIST > ������] �����Ǵ� ȣ�׶��� �Ÿ� ����   // �߰� ���� 171210
void RecvPacketRioencoder(char* msg, double &Rio_encoder, double &emp);

// info time
char *RecvPacketInfoTime(char* msg);

// info result
void RecvPacketInfoResult(char* msg, float &release, float &arrive, Point &pos);

// ��Ŷ����
char RecvPacketAcK(char* msg);

//// ���� ��� ����
void RecvPacketStrCnt(char* msg, float &str_cnt);
//
//// ���� ���
//void RecvPacketStrPath(char* msg, float &x, float &y, float &speed);

//���� ���
void RecvPacketrealPath(char* msg, float &x, float &y, float &time);

//���� ���� �ӵ�
void RecvRealSpeed(char* msg, float &real_speed);

// TEST HACK
void RecvPacketTestHackPos(char* msg, float &test_ang, float &test_speed);

// ���� ������
void RecvStonedata(char* msg, bool &find, float &dist, float &angle);

// LASER_ON
void SendPacketLaserOn(SOCKET s);

// LASER_OFF
void SendPacketLaserOff(SOCKET s);

// LASER_ON
void SendPacketReady(SOCKET s);

// [����(����) > ������] ������ ������ ����
void RecvPacketSweeping(char* msg, float &swip);