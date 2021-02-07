#ifndef obj_H
#define obj_H
#pragma once
#include <stdlib.h>
#include "glut.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include<iostream>
#include <istream>
#include <fstream>
#include<sstream>
using namespace std;
class ObjLoader {
public:
	ObjLoader(string filename,int fl);//构造函数
	void Draw();//绘制函数
	void derive();//导出函数
	vector<vector<GLfloat>>vSets;//存放顶点(x,y,z)坐标
	vector<vector<GLint>>fSets;//存放面的三个顶点索引
};
#endif