// Copyright (C) Thorsten Thormaehlen 2010

#ifndef DEF_MAINWINDOW_H
#define DEF_MAINWINDOW_H

#include <map>

#include <QtGui/qmainwindow.h>
#include <QtGui/qslider.h>
#include <QtGui/qwidget.h>
#include <QtGui/qactiongroup.h>
#include <QtGui/qlabel.h>
#include "SceneViewer.h"
#include "paramMap.h"
#include "ImageViewer.h"



class QSlider;
class GLWidget;
class CMesh;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow();

	private slots:
		void slider1Changed(int angle);
		void slider2Changed(int angle);
		void slider3Changed(int angle);
		void slider4Changed(int angle);
		void slider5Changed(int angle);
		void slider6Changed(int angle);

	public slots:
		void setParamStates();
		void setInitGLFrame();

private:
	QSlider *createSlider(paramMap::semanticParams param);
	void actionSliderChange(int angle, paramMap::semanticParams param);

	GLWidget *glWidget;
	ImageViewer *imgViewerWidget;
	QSlider *slider[NO_SEMANTIC_PARAMS];
	QCheckBox *checkBox[NO_SEMANTIC_PARAMS];	
	QPushButton *setSemanticParams;	

	paramMap mParamMap;

	CMesh initMesh, currMesh;
	CVector<double> shapeParams;
	std::map<paramMap::semanticParams, float> mSemanticParamValues;	
};


#endif
