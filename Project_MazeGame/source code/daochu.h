#pragma once

#include <stdlib.h>
#include "glut.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include<iostream>
#include<Windows.h>
#include <istream>
#include <fstream>
#include<sstream>
#include "obj.h"
//#include "model.cpp"
using namespace std;
namespace daochu
{

	//实现移动鼠标观察模型所需变量
	/*float c = 3.1415926 / 180.0f;
	float r = 10.0f;
	int degree = 0;
	int oldPosY = -1;
	int oldPosX = -1;
	int num = 1;
	*/
	//安置光源
	void setLightRes1();
	//初始化
	void init1();
	void display1();
	void reshape1(int width, int height);
	//移动鼠标360观察模型
	void moseMove1(int button, int state, int x, int y);
	void changeViewPoint(int x, int y);
	void myIdle1();
	
	void v_num_obj(ofstream &fout, int x, float fl);
	void f_num_obj(ofstream &fout, int x);
	void  f_star_obj(ofstream &fout);
	void  v_bunny_obj(ofstream &fout);
	void  f_bunny_obj(ofstream &fout);
	void  v_taiz_obj(ofstream &fout);
	void f_taiz_obj(ofstream &fout);
	void derive();
	void show_model();

}
