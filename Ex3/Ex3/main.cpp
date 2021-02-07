// This assignment may cost you some efferts, so I give you some important HINTS, hope that may help you.
// Enjoy the coding and thinking, do pay more attention to the library functions used in OPENGL, such as how they are used? what are the parameters used? and why?

// ʵ�鱨�������д����룬���Ժ�������⣬�Լ����õ�˳�򣬲���ȡ�˼��Ϊʲô�������ӵ��õģ�Ϊʲô���������������õģ��������ȵ������ֵ���о������⡣
#include <stdlib.h>
#include "glut.h"

//����ƽ�ơ���ת������
float fTranslate;
float fRotate    = 0.0f;
float fScale     = 1.0f;	// set inital scale value to 1.0f

float tRotate = 0.0f; //�����ת

bool bPersp = false; //͸��ͶӰ
bool bAnim = false; //������ת
bool bWire = false; //��Ⱦ��ʽ
bool tAnim = false; //�����ת

int wHeight = 0;
int wWidth = 0;

float place[] = { 0, 0, 5 }; //��������������λ��

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
	glTranslatef(place[0], place[1], place[2]); //���ƽ�ƣ���������
	glRotatef(90, 1, 0, 0); //�����x����ת90�ȣ���������->���ǳ��Ϸ�������
	glRotatef(tRotate, 0, 1.0f, 0); //�����ת(e)
	//todo; hint: when operate the teapot, you may need to change some parameters
	glutSolidTeapot(1); //size
	glPopMatrix();
	
	//��������
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	//������������
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

	//P---�л�ͶӰ��ʽ����ͶӰ��͸��ͶӰ��
	if (bPersp){
		//todo when 'p' operation, hint: use FUNCTION gluPerspective
		gluPerspective(45, whRatio, 1, 100); //͸��ͶӰ
	}
	else
		glOrtho(-3 ,3, -3, 3,-100,100); //��ͶӰ

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

float eye[] = {0, 0, 8}; //��������������λ��
float center[] = {0, 0, 0}; //�����ͷ��׼�����������������λ��

void key(unsigned char k, int x, int y) //keyboard event
{
	switch(k)
	{
	case 27: //ESC
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth);break; } //�л�ͶӰ��ʽ

	case ' ': {bAnim = !bAnim; break;} //��������ͣ��ת
	case 'o': {bWire = !bWire; break;} //�л���Ⱦ��ʽ

			  //�����ƶ����ڸı�����λ�õ�ͬʱ�ı��ӵ�
			  //hint: eye[] and center[] are the keys to solve the problems
	case 'a': {//��������
		center[0] += 0.1f;
		eye[0] += 0.1f;
		break;
			  }
	case 'd': {//��������
		center[0] -= 0.1f;
		eye[0] -= 0.1f;
		break;
			  }
	case 'w': {//��������
		center[1] -= 0.1f;
		eye[1] -= 0.1f;
		break;
			  }
	case 's': {//��������
		center[1] += 0.1f;
		eye[1] += 0.1f;
		break;
			  }
	case 'z': {//����ǰ��
		center[2] += 0.1f;
		eye[2] += 0.1f;
		break;
			  }
	case 'c': {//�������
		center[2] -= 0.1f;
		eye[2] -= 0.1f;
		break;
			  }

			  //�����ز���
			  // hint:use place[] and notice the teapot can NOT be moved out the range of the table.
	case 'l': {//�������
		place[0] += 0.1f;
		if (place[0] > 1.5f) place[0] = 1.5f; //���ܳ�������
		break;
			  }
	case 'j': {//�������
		place[0] -= 0.1f;
		if (place[0] < -1.5f) place[0] = -1.5f; //���ܳ�������
		break;
			  }
	case 'i': {//�������
		place[1] += 0.1f;
		if (place[1] > 1.0f) place[1] = 1.0f; //���ܳ�������
		break;
			  }
	case 'k': {//���ǰ��
		place[1] -= 0.1f;
		if (place[1] < -1.0f) place[1] = -1.0f; //���ܳ�������
		break;
			  }
	case 'e': {//��ת���
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
		0, 1, 0);				// ������0��0��0�����ӵ����� (0,5,50)��Y������

	if (bWire) { //�л���Ⱦ��ʽ
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);//������Ȳ��ԣ����������Զ���Զ����ر���ס��ͼ�Σ����ϣ�
	glEnable(GL_LIGHTING); //���õ�Դ
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

	if (bAnim) fRotate += 0.5f; //������ת
	if (tAnim) tRotate += 0.5f; //�����ת

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


