// glutEx1.cpp : �������̨Ӧ�ó������ڵ㡣
//
//ע��FPS������Ӧ��

#include <stdlib.h>
#include "glut.h"
#include <stdio.h>
#include <string.h>
#include "stanford_bunny.h"

#define MAXNUM 16 // ���16ֻ����

float eye[] = {0, 4, 6};
float center[] = {0, 0, 0};
float fDistance = 0.2f;
float fRotate = 0;
bool bAnim = false;

bool bDrawList = false; // �Ƿ�ʹ����ʾ�б���Ʒ�ʽ
GLint tableList=0;      // �����б�
GLint bunnyList = 0;    //�����б�

int BunnyNum = 1; // ��ǰ��������

void DrawTable() // ������
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

void DrawAllBunnies() // ������
{
	for (int i = 1; i <= BunnyNum; i++)
	{
		if (bDrawList) // ��ʾ�б���Ʒ�ʽ
		{
			glCallList(bunnyList);
		}
		else // ����ʾ�б���Ʒ�ʽ
		{
			DrawBunny();
		}
		if (i % 4 == 0) //4��һ��
		{
			glTranslatef(2.0f, 0.0f, -0.5f); // ����
		}
		else
		{
			glTranslatef(-0.66f, 0.0f, 0.0f);
		}
	}
}

GLint GenTableList() // ������ʾ�б�
{
	GLint lid=glGenLists(1); //  ����1������ʾ�б�

	// glNewList˵��һ����ʾ�б�Ŀ�ʼ��ָ����ʾ�б������Ϊlid������ģʽΪ���б�������ֻ���Ǳ��롱
	glNewList(lid, GL_COMPILE);  

	DrawTable();

	glEndList(); // ��־��ʾ�б�Ľ���

	return lid;
}

GLint GenBunnyList() // ������ʾ�б�
{
	GLint lid = glGenLists(1); //  ����1������ʾ�б�

	// glNewList˵��һ����ʾ�б�Ŀ�ʼ��ָ����ʾ�б������Ϊlid������ģʽΪ���б�������ֻ���Ǳ��롱
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
	DrawAllBunnies(); // ��������
	glPopMatrix();

	glCallList(tableList); // �������ӣ���ʾ�б�
}

void DrawScene() // old way
{
	glPushMatrix();
	//glTranslatef(1.5, 4.5, 0);
	glTranslatef(2.0, 4.5, 1.5);
	glScalef(2, 2, 2);
	//DrawBunny(); // ��������
	DrawAllBunnies(); // ��������
	glPopMatrix();

	DrawTable(); // �������ӣ�����ʾ�б�
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

	case 'a': // ����
		{
			eye[0] += fDistance; 
			center[0] += fDistance;
			break;
		}
	case 'd':  // ����
		{
			eye[0] -= fDistance;
			center[0] -= fDistance;
			break;
		 }
	case 'w':  // ����
		{
			eye[1] -= fDistance;
			center[1] -= fDistance; 
			break;
		}
	case 's':  // ����
		{
			eye[1] += fDistance;
			center[1] += fDistance;
			break;
		}
	case 'z':  // ǰ��
		{
			eye[2] *= 0.95;
			break; 
		}
	case 'c':  // ����
		{
			eye[2] *= 1.05;
			break;
		 }
	case 'l': // �л���ʾ�б�ͷ���ʾ�б���Ʒ�ʽ
		{
			bDrawList = !bDrawList;
			break;
		}
	case ' ': // ��ת
		{
			bAnim = !bAnim; 
			break;
		}
	case 'i': // �����������������16
		{
			if (BunnyNum < MAXNUM)
				BunnyNum++;
			break;
		}
	case 'k': // ����������������С1
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
	
	// FPS��ÿ�봫��֡��(Frames Per Second)
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
	glMatrixMode(GL_PROJECTION);  // ѡ��ͶӰ����
	glPushMatrix();               // ����ԭ����
	glLoadIdentity();             // װ�뵥λ����
	glOrtho(0,480,0,480,-1,1);    // λ����ͶӰ
	glMatrixMode(GL_MODELVIEW);   // ѡ��Modelview����
	glPushMatrix();               // ����ԭ����
	glLoadIdentity();             // װ�뵥λ����
	glRasterPos2f(10,10);
	for (c=buffer; *c != '\0'; c++) {		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // ѡ��ͶӰ����
	glPopMatrix();                // ����Ϊԭ�������
	glMatrixMode(GL_MODELVIEW);   // ѡ��Modelview����
	glPopMatrix();                // ����Ϊԭ�������
	glEnable(GL_DEPTH_TEST);	
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0.5, 0, 1);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// ������0��0��0�����ӵ����� (0, 5, 50)��Y������

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_pos[] = {10, 10, 10, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,gray);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	glEnable(GL_LIGHT0);

	if (bAnim) // ��ת
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

	tableList = GenTableList(); // ������ʾ�б�
	bunnyList = GenBunnyList(); // ������ʾ�б�

	glutMainLoop();
	return 0;
}


