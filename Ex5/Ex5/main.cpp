// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <stdlib.h>
#include "glut.h"

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set initial scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;
bool lColor = false;// 是否改变环境光颜色

int wHeight = 0;
int wWidth = 0;

//环境光
GLfloat light_x = 0.0f;
GLfloat light_y = 0.0f;
GLfloat light_z = 0.0f;
GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 }; // 环境光颜色，白色

//聚光灯
GLfloat spot_x = 0.0f;
GLfloat spot_y = 0.0f;
GLfloat spot_z = 0.0f;
GLfloat spot_angle = 5.0f;//聚光灯角度

void Draw_Leg();

void Draw_Triangle() // This function draws a triangle with RGB colors
{
	// colors used (mat_diffuse)
	GLfloat color1[] = { 0.85f,0.65f,0.2f,1.0f }; // 金黄色
	GLfloat color2[] = { 1.0f,0.0f,0.0f };
	GLfloat color3[] = { 0.0f,1.0f,0.0f };
	GLfloat color4[] = { 1.0f,1.0f,0.0f };
	GLfloat color5[] = { 0.0f,1.0f,1.0f };
	GLfloat color6[] = { 0.0f,0.0f,1.0f };

	//float mat_diffuse[] = { 0.85f, 0.65f, 0.2f, 1.0f };
	float mat_specular[] = { 0.6f, 0.6f, 0.6f, 1.0f };

	// 茶壶
	// 通过设置材料使得茶壶为金黄色
	// 修改茶壶的镜面反射系数，使之对光源呈现高光
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color1); // 漫反射，茶壶为金黄色
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); // 镜面反射
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50); // 亮度，修改茶壶的镜面反射系数
	glPushMatrix();
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	// 桌子
	// 通过设置材料使得桌面和四条腿的颜色分别为：(1, 0, 0), (0, 1, 0), (1, 1, 0), (0, 1, 1), (0, 0, 1)
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color2); // 漫反射
	glMaterialfv(GL_FRONT, GL_SPECULAR, color2); // 镜面反射
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color3); // 漫反射
	glMaterialfv(GL_FRONT, GL_SPECULAR, color3); // 镜面反射
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color4); // 漫反射
	glMaterialfv(GL_FRONT, GL_SPECULAR, color4); // 镜面反射
	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color5); // 漫反射
	glMaterialfv(GL_FRONT, GL_SPECULAR, color5); // 镜面反射
	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color6); // 漫反射
	glMaterialfv(GL_FRONT, GL_SPECULAR, color6); // 镜面反射
	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);
	} else {
		glOrtho(-3 ,3, -3, 3,-100,100);
	}

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

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}

	case 'a': { // 整体左移
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
			  }
	case 'd': { // 整体右移
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
			  }
	case 'w': { // 整体上移
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
			  }
	case 's': { // 整体下移
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
			  }
	case 'z': { // 整体前移
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
			  }
	case 'c': { // 整体后移
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
			  }
	
	// 移动场景中的光源
	case 'j': { // 环境光左移
		light_x  -= 0.2f;
		break;
			}
	case 'l': { // 环境光右移
		light_x += 0.2f;
		break;
			}
	case 'i': { // 环境光上移
		light_y += 0.2f;
		break;
			}
	case 'k': { // 环境光下移
		light_y -= 0.2f;
		break;
			}
	case 'n': { // 环境光前移
		light_z += 0.2f;
		break;
			}
	case 'm': { // 环境光后移
		light_z -= 0.2f;
		break;
			}
	
	// 改变光源的颜色（在两种颜色间切换)
	case 'u': {
		lColor = !lColor;
		break;
			}

	// 调整聚光光源的朝向
	case 'f': { // 聚光灯左移
		spot_x -= 0.05f;
		break;
			}
	case 'h': { // 聚光灯右移
		spot_x += 0.05f;
		break;
			}
	case 't': { // 聚光灯上移
		spot_y -= 0.05f;
		break;
			}
	case 'g': { // 聚光灯下移
		spot_y += 0.05f;
		break;
			}
	case 'v': { // 聚光灯前移
		spot_z += 0.05f;
		break;
			}
	case 'b': { // 聚光灯后移
		spot_z -= 0.05f;
		break;
			}
	
	// 调整聚光光源的照射角度
	case 'r': { // 角度变大
		if (spot_angle <= 89.0f)
			spot_angle += 0.2f;
		break;
			}
	case 'y': { // 角度变小
		if (spot_angle >= 1.0f)
			spot_angle -= 0.2f;
		break;
			}
	}

	updateView(wHeight, wWidth);
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST); // 开启深度测试
	glEnable(GL_LIGHTING); // 开启光照模式

	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 }; // 环境光颜色为白色
	GLfloat light_pos[] = { 5 + light_x,5 + light_y,5 + light_z,1 }; // 环境光光源位置												 
	GLfloat spot_pos[] = { 0.0,5.0,0.0,1.0 }; // 聚光灯位置
	GLfloat spot_dir[] = { 0.0f + spot_x,-1.0f + spot_y,0.0f + spot_z }; // 聚光灯照射方向
													   
	if (lColor) {// 是否改变光照颜色
		color[0] = 0.0f;
		color[2] = 0.0f; //
	}
	else {
		color[0] = 1.0f;
		color[2] = 1.0f; // 白色
	}

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos); // 第0号光源的位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, white); // 镜面反射
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white); //漫反射  
	glLightfv(GL_LIGHT0, GL_AMBIENT, color); //设置第0号光源多次反射后的光照颜色（环境光颜色）
	glEnable(GL_LIGHT0); // 开启第0号光源

	glLightfv(GL_LIGHT1, GL_POSITION, spot_pos); // 第1号光源（聚光灯）的位置
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_angle); //裁减角度
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir); //光源方向
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.); //聚集度
	glLightfv(GL_LIGHT1, GL_SPECULAR, white); //设置镜面光成分
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white); //设置漫射光成分
	glLightfv(GL_LIGHT1, GL_AMBIENT, color); //设置环境光成分
	glEnable(GL_LIGHT1); // 开启第1号光源

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Triangle();						// Draw triangle

	if (bAnim) fRotate    += 0.5f;
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


