// glutEx1.cpp : 定义控制台应用程序的入口点。
//
//注意FPS函数的应用

#include <stdlib.h>
#include "glut.h"
#include <stdio.h>
#include <string.h>
#include "stanford_bunny.h"

#define MAXNUM 16 // 最多16只兔子

float eye[] = {0, 4, 6};
float center[] = {0, 0, 0};
float fDistance = 0.2f;
float fRotate = 0;
bool bAnim = false;

bool bDrawList = false; // 是否使用显示列表绘制方式
GLint tableList=0;      // 桌子列表
GLint bunnyList = 0;    //兔子列表

int BunnyNum = 1; // 当前兔子数量

void DrawTable() // 画桌子
{
	glPushMatrix();
	glTranslatef(0, 3.5, 0);
	glScalef(5, 1, 4);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1.5, 1);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.5, 1);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1.5, -1);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.5, -1);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();
}

void DrawAllBunnies() // 画兔子
{
	for (int i = 1; i <= BunnyNum; i++)
	{
		if (bDrawList) // 显示列表绘制方式
		{
			glCallList(bunnyList);
		}
		else // 非显示列表绘制方式
		{
			DrawBunny();
		}
		if (i % 4 == 0) //4个一行
		{
			glTranslatef(2.0f, 0.0f, -0.5f); // 换行
		}
		else
		{
			glTranslatef(-0.66f, 0.0f, 0.0f);
		}
	}
}

GLint GenTableList() // 桌子显示列表
{
	GLint lid=glGenLists(1); //  创建1个空显示列表

	// glNewList说明一个显示列表的开始，指定显示列表的名称为lid，编译模式为“列表中命令只是是编译”
	glNewList(lid, GL_COMPILE);  

	DrawTable();

	glEndList(); // 标志显示列表的结束

	return lid;
}

GLint GenBunnyList() // 兔子显示列表
{
	GLint lid = glGenLists(1); //  创建1个空显示列表

	// glNewList说明一个显示列表的开始，指定显示列表的名称为lid，编译模式为“列表中命令只是是编译”
	glNewList(lid, GL_COMPILE); 

	DrawBunny();

	glEndList();

	return lid;
}


void Draw_Bunny_Table_List() // new way
{
	glPushMatrix();
	glTranslatef(2.0, 4.5, 1.5);
	glScalef(2, 2, 2);
	DrawAllBunnies(); // 绘制兔子
	glPopMatrix();

	glCallList(tableList); // 绘制桌子（显示列表）
}

void DrawScene() // old way
{
	glPushMatrix();
	//glTranslatef(1.5, 4.5, 0);
	glTranslatef(2.0, 4.5, 1.5);
	glScalef(2, 2, 2);
	//DrawBunny(); // 绘制兔子
	DrawAllBunnies(); // 绘制兔子
	glPopMatrix();

	DrawTable(); // 绘制桌子（非显示列表）
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	gluPerspective(45, whRatio, 1, 1000);	

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }

	case 'a': // 左移
		{
			eye[0] += fDistance; 
			center[0] += fDistance;
			break;
		}
	case 'd':  // 右移
		{
			eye[0] -= fDistance;
			center[0] -= fDistance;
			break;
		 }
	case 'w':  // 上移
		{
			eye[1] -= fDistance;
			center[1] -= fDistance; 
			break;
		}
	case 's':  // 下移
		{
			eye[1] += fDistance;
			center[1] += fDistance;
			break;
		}
	case 'z':  // 前移
		{
			eye[2] *= 0.95;
			break; 
		}
	case 'c':  // 后移
		{
			eye[2] *= 1.05;
			break;
		 }
	case 'l': // 切换显示列表和非显示列表绘制方式
		{
			bDrawList = !bDrawList;
			break;
		}
	case ' ': // 旋转
		{
			bAnim = !bAnim; 
			break;
		}
	case 'i': // 增加兔子数量，最大16
		{
			if (BunnyNum < MAXNUM)
				BunnyNum++;
			break;
		}
	case 'k': // 减少兔子数量，最小1
		{
			if (BunnyNum > 1)
				BunnyNum--;
			break;
		}
	default: break;
	}
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	char mode[64];
	if (bDrawList)
		strcpy(mode, "display list");
	else
		strcpy(mode, "naive");
	
	// FPS：每秒传输帧数(Frames Per Second)
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(buffer,"FPS:%4.2f %s",
			frame*1000.0/(time-timebase), mode);
		timebase = time;		
		frame = 0;
	}

	char *c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0,480,0,480,-1,1);    // 位置正投影
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(10,10);
	for (c=buffer; *c != '\0'; c++) {		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);	
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0.5, 0, 1);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0, 5, 50)，Y轴向上

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_pos[] = {10, 10, 10, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,gray);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	glEnable(GL_LIGHT0);

	if (bAnim) // 旋转
		fRotate += 0.5f;
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis

	glScalef(0.4, 0.4, 0.4);
	if(!bDrawList)
		DrawScene();						// old way
	else 
		Draw_Bunny_Table_List();            // new way

	getFPS();
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Exercise 4");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	tableList = GenTableList(); // 桌子显示列表
	bunnyList = GenBunnyList(); // 兔子显示列表

	glutMainLoop();
	return 0;
}


