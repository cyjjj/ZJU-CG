//模型路径
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
#include"daochu.h"
//#include "model.cpp"
using namespace std;
namespace daochu
{

	//实现移动鼠标观察模型所需变量
		float c = 3.1415926 / 180.0f;
		float r = 10.0f;
		int degree = 0;
		int oldPosY = -1;
		int oldPosX = -1;
		int num = 1;
		
	//安置光源
	void setLightRes1() {
		GLfloat lightPosition[] = { 0.0f, 0.0f, 30.0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glEnable(GL_LIGHTING); //启用光源
		glEnable(GL_LIGHT0);   //使用指定灯光

		glEnable(GL_NORMALIZE);
	}

	//初始化
	void init1() {
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(500, 500);
		glutCreateWindow("ObjLoader");
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		setLightRes1();
		glEnable(GL_DEPTH_TEST);
	}

	void display1()
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		string filePath2 = "1.obj";
		ObjLoader Model_1 = ObjLoader(filePath2, 1);

		glColor3f(1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_NORMALIZE);
		glLoadIdentity();
		gluLookAt(0, 0, r, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		//glTranslatef(0.0f, 0.0f, -5.0f);
		//glScalef(3.0, 3.0, 3.0);
		setLightRes1();
		//glPushMatrix();
		glRotatef(degree, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, -2.0f, 0.0f);
		//glScalef(1.0, 0.1, 0.1);
	//	GLfloat mat_diffuse0[] = { 1.0, 1.0f, 1.0f, 1.0f };//茶壶
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse0); //设置多边形正面漫反射属性
		Model_1.Draw();//绘制obj模型
		glutSwapBuffers();
		//degree++;
	}

	void reshape1(int width, int height)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
		glMatrixMode(GL_MODELVIEW);
	}

	//移动鼠标360观察模型
	void moseMove1(int button, int state, int x, int y)
	{
		if (state == GLUT_DOWN) {
			oldPosX = x; oldPosY = y;
		}
	}
	void changeViewPoint(int x, int y)
	{
		int temp = x - oldPosX;
		degree += temp;
		oldPosX = x;
		oldPosY = y;
	}

	void myIdle1()
	{
		glutPostRedisplay();
	}

	//int num = 1;
	void v_num_obj(ofstream &fout, int x, float fl)
	{
		string  filePath = "num\\";
		filePath.append(1, x + '0');
		filePath += ".obj";
		ObjLoader Model_bunny = ObjLoader(filePath, 2);

		for (int i = 0; i < (Model_bunny.vSets).size(); i++)
		{
			fout << "v" << " " << Model_bunny.vSets[i][0] / 2 + 1.5*fl - 1.5 << " " << -Model_bunny.vSets[i][2] / 2 + 4.5 << " " << -Model_bunny.vSets[i][1] / 2 << endl;
		}

	}
	void f_num_obj(ofstream &fout, int x)
	{
		string  filePath = "num\\";
		filePath.append(1, x + '0');
		filePath += ".obj";
		ObjLoader Model_bunny = ObjLoader(filePath, 2);
		for (int i = 0; i < (Model_bunny.fSets).size(); i++)
		{
			fout << "f" << " " << Model_bunny.fSets[i][0] + num << " " << Model_bunny.fSets[i][1] + num << " " << Model_bunny.fSets[i][2] + num << endl;
		}
		num += Model_bunny.vSets.size();
	}


	void  v_star_obj(ofstream &fout, float fl)
	{
		string filePath = "star.obj";

		ObjLoader Model_bunny = ObjLoader(filePath, 2);
		//Model_bunny.Draw();//绘制obj模型
		//draw_bunny_obj();

		//fout << "write something";
		for (int i = 0; i < (Model_bunny.vSets).size(); i++)
		{
			fout << "v" << " " << Model_bunny.vSets[i][2] / 10 + fl * 2 << " " << Model_bunny.vSets[i][0] / 10 + 1 << " " << Model_bunny.vSets[i][1] / 10 << endl;
		}

	}
	void  f_star_obj(ofstream &fout)
	{
		string filePath = "star.obj";

		ObjLoader Model_bunny = ObjLoader(filePath, 2);
		for (int i = 0; i < (Model_bunny.fSets).size(); i++)
		{
			fout << "f" << " " << Model_bunny.fSets[i][0] + num << " " << Model_bunny.fSets[i][1] + num << " " << Model_bunny.fSets[i][2] + num << endl;
		}
		num += Model_bunny.vSets.size();
	}
	void  v_bunny_obj(ofstream &fout)
	{
		string filePath = "bunny.obj";

		ObjLoader Model_bunny = ObjLoader(filePath, 1);
		//Model_bunny.Draw();//绘制obj模型
		//draw_bunny_obj();

		//fout << "write something";
		for (int i = 0; i < (Model_bunny.vSets).size(); i++)
		{
			fout << "v" << " " << Model_bunny.vSets[i][0] * 10 << " " << Model_bunny.vSets[i][1] * 10 + 0.3 << " " << Model_bunny.vSets[i][2] * 10 << endl;
		}

	}
	void  f_bunny_obj(ofstream &fout)
	{
		string filePath = "bunny.obj";

		ObjLoader Model_bunny = ObjLoader(filePath, 1);
		for (int i = 0; i < (Model_bunny.fSets).size(); i++)
		{
			fout << "f" << " " << Model_bunny.fSets[i][0] + num << " " << Model_bunny.fSets[i][1] + num << " " << Model_bunny.fSets[i][2] + num << endl;
		}
		num += Model_bunny.vSets.size();
	}
	void  v_taiz_obj(ofstream &fout)
	{
		string filePath = "taiz.obj";

		ObjLoader Model_bunny = ObjLoader(filePath, 2);
		//Model_bunny.Draw();//绘制obj模型
		//draw_bunny_obj();

		//fout << "write something";
		for (int i = 0; i < (Model_bunny.vSets).size(); i++)
		{
			fout << "v" << " " << Model_bunny.vSets[i][0] / 4 << " " << Model_bunny.vSets[i][1] / 4 << " " << Model_bunny.vSets[i][2] / 4 << endl;
		}
	}
	void f_taiz_obj(ofstream &fout)
	{
		string filePath = "taiz.obj";
		ObjLoader Model_bunny = ObjLoader(filePath, 2);
		for (int i = 0; i < (Model_bunny.fSets).size(); i++)
		{
			fout << "f" << " " << Model_bunny.fSets[i][0] + num << " " << Model_bunny.fSets[i][1] + num << " " << Model_bunny.fSets[i][2] + num << endl;
		}
		num += Model_bunny.vSets.size();
	}
	void derive()
	{

		ofstream fout;
		fout.open("1.obj");
		v_taiz_obj(fout);
		v_bunny_obj(fout);
		v_star_obj(fout, -1);
		v_star_obj(fout, 1);
		v_num_obj(fout, 0, -1);
		v_num_obj(fout, 3, 1);
		f_taiz_obj(fout);
		f_bunny_obj(fout);
		f_star_obj(fout);
		f_star_obj(fout);
		f_num_obj(fout, 0);
		f_num_obj(fout, 3);
		//string filePath = "bunny.obj";
		fout.close();
	}
	void show_model()
	{
		derive();

		init1();
		glutDisplayFunc(display1);
		glutReshapeFunc(reshape1);
		glutMouseFunc(moseMove1);
		glutMotionFunc(changeViewPoint);
		glutIdleFunc(myIdle1);
		glutMainLoop();
	}
}
