// Copyright (C) Thorsten Thormaehlen 2010

#include <QtGui/qwidget.h>
#include <QtOpenGL/qgl.h>


#include <math.h>
#include <iostream>
using namespace std;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include "SceneViewer.h"
#include "onlyDefines.h"
#include "CTMesh.h"


GLWidget::GLWidget(CMesh *imesh, QWidget *parent)
: QGLWidget(parent)
{
	xRot = 0;
	yRot = 0;
	zRot = 0;

	qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
	qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);	
	glmesh = imesh;
	paintGL(glmesh);
	updateGL();
}

GLWidget::~GLWidget()
{
	makeCurrent();	
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(400, 400);
}


void GLWidget::setXRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != xRot) {
		xRot = angle;	
		paintGL(glmesh);
		updateGL();
	}
}

void GLWidget::setYRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != yRot) {
		yRot = angle;
		paintGL(glmesh);
		updateGL();
	}
}

void GLWidget::setZRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != zRot) {
		zRot = angle;	
		paintGL(glmesh);
		updateGL();
	}
}

void GLWidget::initializeGL()
{
	qglClearColor(qtPurple.dark());
	//glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}

void GLWidget::paintGL(CMesh *mesh)
{
	glmesh = mesh;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -10.0);
	glRotated(xRot / STEP_SIZE_ROT, 1.0, 0.0, 0.0);
	glRotated(yRot / STEP_SIZE_ROT, 0.0, 1.0, 0.0);
	glRotated(zRot / STEP_SIZE_ROT, 0.0, 0.0, 1.0);


	glColor3f(.95,.95,.95);
	glPointSize(1.0);

	glPolygonMode(GL_FRONT, GL_LINE); 
	glPolygonMode(GL_BACK, GL_LINE);

	glBegin (GL_TRIANGLES);

	for(int i0 = 0; i0 < mesh->GetMeshSize(); ++i0) 
	{
#if 0 //plot points
		float x,y,z;
		mesh->GetPoint(i0,x,y,z);
		glVertex3f (x/2000.,y/2000.,z/2000.);
#else //plot mesh
		
		int tri[3];
		mesh->GetPatch(i0, tri[0], tri[1], tri[2]);		
		for (unsigned i1 = 0; i1 < 3; i1++)
		{	
			float x,y,z;
			mesh->GetPoint(tri[i1],x,y,z);
			glVertex3d(x/2000., y/2000., z/2000.);
		}
#endif
	}
	glEnd();
}

void GLWidget::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	//glViewport((width - side) / 2, (height - side) / 2, side, side);
	glViewport((width - side) / 2, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-0.5, +0.5, +.7, -.7, 4.0, 15.0);
	gluPerspective(-10.5, float(width)/float(height), -2, +2);//, 10.0);
	glMatrixMode(GL_MODELVIEW);
	paintGL(glmesh);
	updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(xRot + 80 * dy);
		setYRotation(yRot + 80 * dx);
	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(xRot + 80 * dy);
		setZRotation(zRot + 80 * dx);
	}
	lastPos = event->pos();
}



void GLWidget::normalizeAngle(int *angle)
{
	while (*angle < 0)
		*angle += 360 * STEP_SIZE_ROT;
	while (*angle > 360 * STEP_SIZE_ROT)
		*angle -= 360 * STEP_SIZE_ROT;
}

