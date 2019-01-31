#pragma once
#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <stdlib.h>


/*
*PATH_Prediction.h
*/
#define theta 20//다른 세타 하나더 만들어야 함
//theta is angle between y-axis and direction in which the laser sensor looks at the stone's center
#define stone_radius 0.3
#define robot_radius 0.8
#define pi 3.14159265358979323846
#define freq 1/30
#define w1 1
#define w2 0.25

class Prediction {
public:

	

	/////////////////////////////////return2route//////////////////////////////////////////
	//This would be the parameter from stone trajectory
	//double x_real[4500];
	//double y_real[4500];
	//double x_pred[4500];
	//double y_pred[4500];
	
	//define file input parameter
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


	//static UINT Sweeping_YU(LPVOID param);

	/////////////////////////////////용규/////////////////////////////////////////////////
	/*
	*Laser.h
	*/
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

};

/*
*Dynamics.h
*/
class Dynamics {
public:
	double minValX, minValY;
	double robot_speed[2000];// vel = 0; //로봇 속도
	double maxValX, maxValY;
	///////////////////다이나믹스///////////////////
	double theta1, theta2, theta3, theta4, theta1_1, theta2_1, theta3_1, theta4_1, theta4_2;
};
/*
*Kalman.h
*/
//#define term 1.0/15.0 

class Kalman {
public:
	double dt;
	int i, j;//the number of trajectory point
	double pos_x, pos_y, vel;//This is the parameter of kalman
	int flagF;
	float sum_error_kalman_x, sum_error_kalman_y;// error_sum of kalman

												 /*for test*/
	double x_stone_plan[6000];//This would be the parameter from stone trajectory
	double y_stone_plan[6000];
	double stone_vel[6000];

	struct Point
	{
		float frame_n, t, x, y;   // frame_n  프레임수, t = 실제시간, x = x좌표, y = y좌표
								  // 원점 : 백라인 중심, x좌표: 오른쪽 +, 단위 mm
								  // 1프레임 = 15hz
		Point(double _frame_n, double _t, double _x, double _y) { frame_n = _frame_n; t = _t; x = _x; y = _y; };
	};

	double get_trajectory();
	double kalman(double, double, double, double, double);
	void kalmantest();//double, double);
};
/*
* matrix.h
*/

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
	Matrix& operator*=(double);
	Matrix& operator/=(double);
	//Matrix  operator^(int);

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
Matrix operator*(const Matrix&, double);
Matrix operator*(double, const Matrix&);
Matrix operator/(const Matrix&, double);

#endif
