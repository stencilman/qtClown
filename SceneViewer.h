// Copyright (C) Thorsten Thormaehlen 2010

#ifndef DEF_SCENEVIEWER_H
#define DEF_SCENEVIEWER_H

#include <QtGui/qwidget.h>
#include <QtGui/qapplication.h>
#include <QtOpenGL/qgl.h>
#include <QtGui/qlayout.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qcolor.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qevent.h>
#include "CTMesh.h"



/*!
    \class SceneViewer SceneViewer.h    
    \brief User Interface widget that renders the scene.
*/




class GLWidget : public QGLWidget
{
	Q_OBJECT

public:

	GLWidget(CMesh *imesh, QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void paintGL(CMesh *mesh);


protected:
	void initializeGL();
	
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void normalizeAngle(int *angle);	
	int xRot, yRot, zRot;
	QPoint lastPos;
	QColor qtGreen;
	QColor qtPurple;	
	CMesh *glmesh;
};


#endif
