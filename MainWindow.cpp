// Copyright (C) Thorsten Thormaehlen 2010

#include "qtIncludeFiles.h"

#include <iostream>
#include <algorithm>
using namespace std;

#include "MainWindow.h"
#include "SceneViewer.h"
#include "onlyDefines.h"

MainWindow::MainWindow()
{
	initMesh.readModel("data/maya_model.dat", true);
	initMesh.updateJntPos();
	initMesh.centerModel();
	initMesh.readShapeSpaceEigens("data/final_eigenvectors.dat"); 
	currMesh = initMesh;
	shapeParams.setSize(NO_OF_EIGENVECTORS);
	shapeParams = 0;	

	glWidget = new GLWidget(&currMesh);	
	imgViewerWidget = new ImageViewer(&currMesh);

	for(unsigned int i0 = 0; i0 < NO_SEMANTIC_PARAMS; i0++)
	{
		slider[i0] = createSlider((paramMap::semanticParams)i0);
		string &name = mParamMap.getNameforParam(i0);
		checkBox[i0] = new QCheckBox(name.c_str());
		mSemanticParamValues[(paramMap::semanticParams)i0] = 0;
	}
	setSemanticParams = new QPushButton("Set Sliders");		

	connect(slider[0], SIGNAL(valueChanged(int)), this, SLOT(slider1Changed(int)));
	connect(slider[1], SIGNAL(valueChanged(int)), this, SLOT(slider2Changed(int)));
	connect(slider[2], SIGNAL(valueChanged(int)), this, SLOT(slider3Changed(int)));
	connect(slider[3], SIGNAL(valueChanged(int)), this, SLOT(slider4Changed(int)));
	connect(slider[4], SIGNAL(valueChanged(int)), this, SLOT(slider5Changed(int)));
	connect(slider[5], SIGNAL(valueChanged(int)), this, SLOT(slider6Changed(int)));

	connect(setSemanticParams, SIGNAL(clicked()), this, SLOT(setParamStates()));

	//////////////////////////////////////////////////////////////////////////
	// Layout
	//////////////////////////////////////////////////////////////////////////
	QHBoxLayout *mainLayout = new QHBoxLayout;	
	mainLayout->addWidget(glWidget);
	QVBoxLayout *imgLayout = new QVBoxLayout;
	imgViewerWidget->updateImgViewer(&currMesh);
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
	imgLayout->addItem( spacer );
	for(int i0 = NO_IMAGE_PARTS - 1; i0 >= 0; i0--)	
		imgLayout->addWidget(imgViewerWidget->getImageLabel(i0));	
	imgLayout->addItem( spacer );	

	mainLayout->addLayout(imgLayout);	
	for(unsigned int i0 = 0; i0 < NO_SEMANTIC_PARAMS; i0++)
		mainLayout->addWidget(slider[i0]);
	
	QVBoxLayout *cBoxLayout = new QVBoxLayout;
	for(unsigned int i0 = 0; i0 < NO_SEMANTIC_PARAMS; i0++)
		cBoxLayout->addWidget(checkBox[i0]);
	
	cBoxLayout->addWidget(setSemanticParams);
	mainLayout->addLayout(cBoxLayout);
	setLayout(mainLayout);
	//////////////////////////////////////////////////////////////////////////
	
	for(unsigned int i0 = 0; i0 < NO_SEMANTIC_PARAMS; i0++)
	{
		//slider[i0]->setValue(180 * STEP_SIZE);
		checkBox[i0]->setChecked(true);
	}	
	setWindowTitle(tr("Hello Beautiful Stranger"));		
}

QSlider * MainWindow::createSlider( paramMap::semanticParams param )
{
	QSlider *slider = new QSlider(Qt::Vertical);
	int low = mParamMap.getRange(param, true);
	int high = mParamMap.getRange(param, false);
	slider->setRange(low*STEP_SIZE, high*STEP_SIZE);
	slider->setSingleStep(1);
	slider->setPageStep(10*STEP_SIZE);
	slider->setTickInterval(10*STEP_SIZE);
	slider->setTickPosition(QSlider::TicksRight);
	int initVal = 0;//(high + low)/2 * STEP_SIZE;
	slider->setValue(initVal);
	return slider;
}

void MainWindow::setParamStates()
{
	//cout<<"\nClicked!!";
	std::vector<bool> checkBoxStates;
	for(unsigned int i0 = 0; i0 < NO_SEMANTIC_PARAMS; i0++)
	{
		int state = (int)checkBox[i0]->checkState();
		checkBoxStates.push_back((state == 0) ? false : true);
		if(checkBoxStates[i0] == false)
		{
			slider[i0]->setEnabled(false);
			slider[i0]->setValue(0);
			mSemanticParamValues[(paramMap::semanticParams)i0] = 0;
		}
		else
			slider[i0]->setEnabled(true);
	}	
	mParamMap.setSemanticParams(checkBoxStates);
	mParamMap.computeMap();
}


void MainWindow::slider1Changed(int angle)
{
	//std::cout<< angle<<std::endl;
	actionSliderChange(angle,paramMap::semanticParams::Height);	
}
void MainWindow::slider2Changed(int angle)
{
	actionSliderChange(angle,paramMap::semanticParams::Weight);	
}
void MainWindow::slider3Changed(int angle)
{
	actionSliderChange(angle,paramMap::semanticParams::BreastSize);	
}

void MainWindow::slider4Changed(int angle)
{
	actionSliderChange(angle,paramMap::semanticParams::WaistSize);	
}
void MainWindow::slider5Changed(int angle)
{
	actionSliderChange(angle,paramMap::semanticParams::HipSize);	
}
void MainWindow::slider6Changed(int angle)
{
	actionSliderChange(angle,paramMap::semanticParams::LegLength);	
}

void MainWindow::setInitGLFrame()
{
	glWidget->paintGL(&currMesh);
	glWidget->updateGL();
}

void MainWindow::actionSliderChange( int angle, paramMap::semanticParams param )
{
	mSemanticParamValues[param] = angle/STEP_SIZE;	
	mParamMap.mapSemanticToEigenSpace(mSemanticParamValues, shapeParams);

	currMesh = initMesh;
	currMesh.shapeChangesToMesh(shapeParams);
	currMesh.updateJntPos();
	glWidget->paintGL(&currMesh);
	glWidget->updateGL();
	imgViewerWidget->updateImgViewer(&currMesh);
}