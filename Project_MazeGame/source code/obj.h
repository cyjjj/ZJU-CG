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
	ObjLoader(string filename,int fl);//���캯��
	void Draw();//���ƺ���
	void derive();//��������
	vector<vector<GLfloat>>vSets;//��Ŷ���(x,y,z)����
	vector<vector<GLint>>fSets;//������������������
};
#endif