// This assignment may cost you some efferts, so I give you some important HINTS, hope that may help you.
// Enjoy the coding and thinking, do pay more attention to the library functions used in OPENGL, such as how they are used? what are the parameters used? and why?

// 实验报告里面多写点感想，即对函数的理解，以及调用的顺序，步骤等。思考为什么是这样子调用的，为什么参数是这样子设置的？。。。等等你觉得值得研究的问题。
#include <stdlib.h>
#include "glut.h"

//整体平移、旋转、缩放
float fTranslate;
float fRotate    = 0.0f;
float fScale     = 1.0f;	// set inital scale value to 1.0f

float tRotate = 0.0f; //茶壶旋转

bool bPersp = false; //透视投影
bool bAnim = false; //整体旋转
bool bWire = false; //渲染方式
bool tAnim = false; //茶壶旋转

int wHeight = 0;
int wWidth = 0;

float place[] = { 0, 0, 5 }; //茶壶在世界坐标的位置

//todo
//hint: some additional parameters may needed here when you operate the teapot

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene()
{
	glPushMatrix();
	glTranslatef(place[0], place[1], place[2]); //茶壶平移，放在桌上
	glRotatef(90, 1, 0, 0); //茶壶绕x轴旋转90度，看见壶盖->壶盖朝上放在桌上
	glRotatef(tRotate, 0, 1.0f, 0); //茶壶旋转(e)
	//todo; hint: when operate the teapot, you may need to change some parameters
	glutSolidTeapot(1); //size
	glPopMatrix();
	
	//绘制桌面
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	//绘制四条桌腿
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;

	//P---切换投影方式（正投影与透视投影）
	if (bPersp){
		//todo when 'p' operation, hint: use FUNCTION gluPerspective
		gluPerspective(45, whRatio, 1, 100); //透视投影
	}
	else
		glOrtho(-3 ,3, -3, 3,-100,100); //正投影

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = {0, 0, 8}; //相机在世界坐标的位置
float center[] = {0, 0, 0}; //相机镜头对准的物体在世界坐标的位置

void key(unsigned char k, int x, int y) //keyboard event
{
	switch(k)
	{
	case 27: //ESC
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth);break; } //切换投影方式

	case ' ': {bAnim = !bAnim; break;} //启动与暂停旋转
	case 'o': {bWire = !bWire; break;} //切换渲染方式

			  //整体移动，在改变物体位置的同时改变视点
			  //hint: eye[] and center[] are the keys to solve the problems
	case 'a': {//整体左移
		center[0] += 0.1f;
		eye[0] += 0.1f;
		break;
			  }
	case 'd': {//整体右移
		center[0] -= 0.1f;
		eye[0] -= 0.1f;
		break;
			  }
	case 'w': {//整体上移
		center[1] -= 0.1f;
		eye[1] -= 0.1f;
		break;
			  }
	case 's': {//整体下移
		center[1] += 0.1f;
		eye[1] += 0.1f;
		break;
			  }
	case 'z': {//整体前移
		center[2] += 0.1f;
		eye[2] += 0.1f;
		break;
			  }
	case 'c': {//整体后移
		center[2] -= 0.1f;
		eye[2] -= 0.1f;
		break;
			  }

			  //茶壶相关操作
			  // hint:use place[] and notice the teapot can NOT be moved out the range of the table.
	case 'l': {//茶壶右移
		place[0] += 0.1f;
		if (place[0] > 1.5f) place[0] = 1.5f; //不能超出桌子
		break;
			  }
	case 'j': {//茶壶左移
		place[0] -= 0.1f;
		if (place[0] < -1.5f) place[0] = -1.5f; //不能超出桌子
		break;
			  }
	case 'i': {//茶壶后移
		place[1] += 0.1f;
		if (place[1] > 1.0f) place[1] = 1.0f; //不能超出桌子
		break;
			  }
	case 'k': {//茶壶前移
		place[1] -= 0.1f;
		if (place[1] < -1.0f) place[1] = -1.0f; //不能超出桌子
		break;
			  }
	case 'e': {//旋转茶壶
		tAnim = !tAnim;
		break;
			  }
	}
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire) { //切换渲染方式
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);//启用深度测试，根据坐标的远近自动隐藏被遮住的图形（材料）
	glEnable(GL_LIGHTING); //启用灯源
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();						// Draw Scene

	if (bAnim) fRotate += 0.5f; //整体旋转
	if (tAnim) tRotate += 0.5f; //茶壶旋转

	//todo; hint: when you want to rotate the teapot, you may like to add another line here =)
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Ex3");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


