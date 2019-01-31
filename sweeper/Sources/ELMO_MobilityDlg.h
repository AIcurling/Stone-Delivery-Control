
// ELMO_MobilityDlg.h : 헤더 파일
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "Mmsystem.h"
#include "afxwin.h"
#include "CXBOXController.h"
#include "ColorListBox.h"
//#include "SerialPort.h"
#include <iostream>
#include "OneSocket.h" //Socket
#include "common.h"
#include "Packet.h"
//#include "Sweeping_YU.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <stdlib.h>

using namespace std;

//Serial Port(DSP-3000)
//#define DEF_COMPORT1	1
//#define DEF_BAUD1		38400

#pragma once

#define theta 20//다른 세타 하나더 만들어야 함
//theta is angle between y-axis and direction in which the laser sensor looks at the stone's center
#define stone_radius 0.3
#define robot_radius 0.8
#define pi 3.14159265358979323846
#define freq 1/30
#define w1 1
#define w2 0.25


// CELMO_MobilityDlg 대화 상자
class CELMO_MobilityDlg : public CDialogEx
{
// 생성입니다.
public:
	CELMO_MobilityDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.


public://변수//
	//ELMO Command&Feedback관련 구조체//
	ELMODATA	_ELMO;

	//Joystick
	double	m_dJoystic_UP_Down;
	double	m_dJoystic_Left_Right;
	DWORD	m_iJoystic_Button;

	//Thread//
	HANDLE			m_hThread;
	BOOL			m_bExit;
	BOOL			m_bInit;
	BOOL			m_ThreadFlag;

	//Color ListBox
	CColorListBox		m_ctrlMsgList;

//	Prediction			m_pridiction;
	//DSP-3000//
	//CSerialPort		m_hSerialPort;
	//BOOL			m_bRS232Connected;

	//소켓통신//
	COneSocket *m_pMainSocket;
	COneSocket *m_pClientSocket;// 클라이언터 소켓을 보관한다

public://함수//
	//Joystic//
	bool	JoystickControl(double &dXpos, double &dYpos, DWORD &buttons);
	//Thread
	static	UINT ThreadProc(LPVOID lpParam);
	//레지스트리//
	//Feedback
	void	Feedback_LoadDataFromRegistry(void);
	void	Feedback_WriteDataToRegistry(void);
	//Command
	void	Command_LoadDataFromRegistry(void);
	void	Command_WriteDataToRegistry(void);
	//Play Command//
	void	Play_Command(void);
	//Message Box관련//
	void writeMessageBox(CString strMsg, COLORREF crText);

	//소켓통신//
	LRESULT OnPeSocketConnect(WPARAM wParam, LPARAM lParam);// 상대방의 연결여부 이벤트 수신	
	LRESULT OnPeSocketAccept(WPARAM wParam, LPARAM lParam);// 상대방의 연결요청 이벤트수신	
	LRESULT OnPeSocketClose(WPARAM wParam, LPARAM lParam);// 상대방의 종료요청 이벤트수신	
	LRESULT OnPeSocketReceive(WPARAM wParam, LPARAM lParam);// 상대방의 수신요청 이벤트수신
	void	SocketStrSend(CString strMessage);	// 소켓통신으로 문자열 전송
	int		DoCommand(CString strCommand); //Message 받는 부분
	//Serial통신관련//
	//void	rs232Connect();
	//void	rs232Disconnect();
	//BYTE	doHexData2AsciiValue(char cData);
	//void	rs232Send_data(CString strData);
	//BOOL	Initialize_DSP3000();


// 대화 상자 데이터입니다.
	enum { IDD = IDD_ELMO_MOBILITY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//afx_msg long OnReceive(WPARAM wParam, LPARAM lParam); //DSP-3000
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedTest();
	double m_dJoystickData;
	DWORD m_dwButtonData;
	double m_dJoystickData2;
	CString m_strABSEncoderData1;
	CString m_strABSEncoderData2;
	CString m_strABSEncoderData3;
	CString m_strSteeringVelocityData1;
	CString m_strSteeringVelocityData2;
	CString m_strSteeringVelocityData3;
	CString m_strEncoderData1;
	CString m_strEncoderData2;
	CString m_strEncoderData3;
	CString m_strWheelVelocityData1;
	CString m_strWheelVelocityData2;
	CString m_strWheelVelocityData3;
	CString m_strArm_Angle1;
	CString m_strArm_Angle2;
	CString m_strSocket_SendData;
	CString m_strStreering1;
	CString m_strStreering2;
	CString m_strStreering3;
	CString m_strCurrent1;
	CString m_strCurrent2;
	CString m_strCurrent3;
	CString m_strCurrent4;
	CString m_strCurrent5;
	CString m_strCurrent6;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	double m_dSweepingArm1_Speed;
	double m_dSweepingArm2_Speed;
	double m_dSweepingArm1_Pose;

	//YU Param

	SOCKET clientSock;
	SOCKET serverSock;

	bool m_bRelease;
	bool m_bReleaseEnd;
	bool m_bEmergency;
	bool m_bTake_out;
	float m_fReleaseSpeed;
	float numOfPacket;
	float m_fReleaseAngle;
	CString m_strCurl;
	CString m_strReleasePos;
	CString m_strReleaseTarPos;
	float m_fAccel;
	CString m_strAccelPos;
	float m_fRobotAngle;
	CString m_strRobotPos;
	float m_fHogDist;
	float m_fHogOffset;
	float m_fRioangle;
	double m_fRioEncoder;

	//TestMove(171121)
	float m_fTestSpeed;
	float m_fTestAngle;
	float m_pre;
	bool m_bSetCam;
	bool m_bCW;
	float m_mode;
	bool m_isThrower;
	bool m_recvRelease;


	//SW
	int i, w, real_index, drive_count;
	float total;

	bool RIO_Ready, SBC_Ready, KU_Ready, start_ready;

	//int a;
	//int w;

	double m1;
	double m2;
	double m3;

//	float numOfPacket;
	float ex_x, ex_y, ex_speed, real_x, real_y, real_speed, num, Hog_x, Hog_y, less_speed, num_1 , cal_angle;
	int robot_hog_count;

	void get_trajectory_plan();
	void get_trajectory_real(float,float);
	void path_prediction(int, float);
	void predict_formation();


	void Auto_back();

	float drive_y, drive_x;

//	float m_pre, m_mode;
	float ex_x_stone_plan[4500];
	float ex_y_stone_plan[4500];
	float ex_v_stone_plan[4500];

	float real_x_stone_plan[4500];
	float real_y_stone_plan[4500];

	float ex_x_robot_plan[4500];
	float ex_y_robot_plan[4500];

	float real_x_robot_ideal[4500];
	float real_y_robot_ideal[4500];

	float ex_distance_R1[4500];
	float ex_angle_R1[4500];
	float ex_speed_R1[4500];

	float x_stone_real[4500];
	float y_stone_real[4500];

	float real_distance_R1[4500];
	float real_angle_R1[4500];

	float x_stone_predict[4500];
	float y_stone_predict[4500];

	float x_stone_predict_1[4500];
	float y_stone_predict_1[4500];

	float x_robot_predict[4500];
	float y_robot_predict[4500];

	float distance_R1[4500];
	float angle_R1[4500];

	double x_corr[4500];
	double y_corr[4500];
	//stone vector
	double x, y;
	float vectorplus;
	int vsign;
	//set margin -------------------------------***margin 데이터 마다 알맞게 수정해주기***---------------------------------------
	double margin;
	double margin_y;
	//the number of trajectory point : arrangement index i,j,k
	int global, funcloop, forcorr;
	//finish
	int finiflag;

	typedef struct sensor {
		double x, y;
		double rad;
		int xy;//거리
	}sensor;
	sensor data[1000];
	sensor Group[100];
	sensor Group2[100];
	sensor GroupS[100];

	double dist, ang;

	int sum;
	int check;
	int check_last;
	int check_last1;
	int check_last2;
	int check_last3;

	int check1;
	int check2;
	int check3;

	int yong1;
	int yong2;
	int yong3;

	double between1;
	double between2;
	double betweenS;

	int check_Count;
	int check_Count1;
	int check_Count2;
	int check_Count3;

	int flag;
	double maxrad;
	double minrad;
	double jirm;
	double stone_dist, stone_rad;
	double find_stone();

	//kalman
	double kal_x_stone_plan[2], kal_y_stone_plan[2]; //This would be the parameter from stone trajectory
	double kal_stone_vel;  // input initial_stone_velocity value //▲ variable value
	double x_stone_plan_save, y_stone_plan_save, stone_vel_save;
	int kalman_counter; //the number of packet point
	double pos_x, pos_y, vel; //This is the parameter of kalman
	int flagi;  //input 2 parameter and then we get 1 output from kalman filter
	int flagF;  //This is 'file print' about 'system model' of kalman
	int flagF2; //This is 'file print' about 'main test' but, print index once
	double dt; // real time of packet //▲ variable value 
	double prev_time;

public:
	static UINT RecvThreadPoint(LPVOID param);
	//static UINT SendThreadPoint(LPVOID param);
	static UINT RioThreadPoint(LPVOID param);
	static UINT EventLoop(LPVOID param);
	static UINT Path_Planning(LPVOID param);
	static UINT Sweeping(LPVOID param);


	SOCKET SetTCPServer(short pnum, int blog);//대기 소켓 설정

	HANDLE AddNetworkEvent(SOCKET sock, long net_event);

	//Test(171121)
	void SendMode(float prep);
	void TestMove(bool cst);
	void CallAngle();
	void SendAngle();
	void SendCalib();
	void SendEmergency();
	void SendTurn(float emp);

	void AcceptProc(int index);
	void ReadProc(int index, LPVOID param);
	void CloseProc(int index);

	struct PointRR
	{
		double  x, y;
		PointRR(double _x, double _y) { x = _x; y = _y; };
	};
	//txt file input
	//double get_trajectoryRR(string name, double x_data[], double y_data[], int data);
	void real2corr();
	void vectorp(int sig);
	void stone(int sign); //test stone vector //-------------------------------***가중치 데이터 마다 알맞게 수정해줘야함***-----------------------------------
	int design_traj(); //compute area and sweeping number
	int checking(double margin); //where is stone? we can check stone trajectories with margin
	int sweeping(int sign); //sweeping test
	void round_off(int sign);
	void definevalueRR(); //margin 값 중요값들 정의
	//한번 경로 복귀후 종료 // 원래는 경로 복귀 후 실시간으로 경로 생성해야함
	void return2route(); //---------***margin 데이터 마다 알맞게 수정해줘야함. 만약 잘 못 설정하면 여러번 스위핑하나 예상경로 복귀불가***----------

	//KALMAN
	struct kalman_Point
	{
		float frame_n, t, x, y;   // frame_n  프레임수, t = 실제시간, x = x좌표, y = y좌표
		// 원점 : 백라인 중심, x좌표: 오른쪽 +, 단위 mm
		// 1프레임 = 15hz
		kalman_Point(double _frame_n, double _t, double _x, double _y) { frame_n = _frame_n; t = _t; x = _x; y = _y; };
	};

	void get_packet(double real_x, double real_y, double real_time);
	double kalman(double, double, double, double, double);
	void kalman_sweeping(float, float, float);


	void kalmantest();//double, double);
	/*double m_dSweepingArm1_Speed;
	double m_dSweepingArm2_Speed;
	double m_dSweepingArm1_Pose;*/
	//afx_msg void OnBnClickedButton2();
	//afx_msg void OnBnClickedButton3();
	//afx_msg void OnBnClickedButton5();
	/*afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();*/
};

#ifndef __MATRIX_H__
#define __MATRIX_H__

class Matrix {
public:
	Matrix(int, int);
	Matrix(int, int, double, double, double, double, double, double, double, double, double);
	Matrix(double[][3], int, int);
	Matrix();
	~Matrix();
	Matrix(const Matrix&);
	Matrix& operator=(const Matrix&);
	inline double& operator()(int x, int y) { return p[x][y]; }


	Matrix& operator+=(const Matrix&);
	Matrix& operator-=(const Matrix&);
	Matrix& operator*=(const Matrix&);


	//friend std::ostream& operator<<(std::ostream&, const Matrix&);
	//friend std::istream& operator>>(std::istream&, Matrix&);


	//void swapRows(int, int);
	Matrix transpose();
	Matrix show();
	double value(int, int);
	Matrix save(FILE *fp);

	/*
	static Matrix createIdentity(int);
	static Matrix solve(Matrix, Matrix);
	static Matrix bandSolve(Matrix, Matrix, int);

	// functions on vectors
	static double dotProduct(Matrix, Matrix);

	// functions on augmented matrices
	static Matrix augment(Matrix, Matrix);
	Matrix gaussianEliminate();
	Matrix rowReduceFromGaussian();
	void readSolutionsFromRREF(std::ostream& os);
	Matrix inverse();*/

private:
	int rows_, cols_;
	double **p;

	void allocSpace();
	//Matrix expHelper(const Matrix&, int);
};

Matrix operator+(const Matrix&, const Matrix&);
Matrix operator-(const Matrix&, const Matrix&);
Matrix operator*(const Matrix&, const Matrix&);

#endif
