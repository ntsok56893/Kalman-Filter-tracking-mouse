#pragma once

/**
* Created by 05 on 2018/8/20.
*/

#include <iostream>
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

CvPoint mousePosition = cvPoint(10, 10);

//mouse event callback  
void mouseEvent(int event, int x, int y, int flags, void *param)
{
	if (event == CV_EVENT_MOUSEMOVE) {
		mousePosition = cvPoint(x, y);
	}
}

namespace IP_HW1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnAvg;
	protected:

	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnAvg = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// btnAvg
			// 
			this->btnAvg->Location = System::Drawing::Point(96, 56);
			this->btnAvg->Name = L"btnAvg";
			this->btnAvg->Size = System::Drawing::Size(75, 23);
			this->btnAvg->TabIndex = 0;
			this->btnAvg->Text = L"Kalman";
			this->btnAvg->UseVisualStyleBackColor = true;
			this->btnAvg->Click += gcnew System::EventHandler(this, &MyForm::btnAvg_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->btnAvg);
			this->Name = L"MyForm";
			this->Text = L"IP_HW";
			this->ResumeLayout(false);

		}

		
#pragma endregion
	private: System::Void btnAvg_Click(System::Object^  sender, System::EventArgs^  e) {

		Mat img(500, 500, CV_8UC3);
		const int stateNum = 4;
		const int measureNum = 2;
		KalmanFilter KF(stateNum, measureNum, 0);       //state(x, y, detaX, detaY) 測量座標XY，總共4個
		Mat processNoise(stateNum, 1, CV_32F);
		Mat measurement = Mat::zeros(measureNum, 1, CV_32F); //measurement(x, y)
		KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); //transition matrix [1, 0, 1, 0 ; 0, 1, 0, 1 ; 0, 0, 1, 0 ; 0, 0, 0, 1]

		//將以下矩陣設置成對角陣
		setIdentity(KF.measurementMatrix);
		setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
		setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
		setIdentity(KF.errorCovPost, Scalar::all(1));

		//randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));

		cvNamedWindow("Kalman");
		setMouseCallback("Kalman", mouseEvent, NULL);

		while (1) {
			//卡曼濾波預測
			Mat prediction = KF.predict(); //計算預測值，返回x',y'
			cv::Point predictPt = cv::Point((int)prediction.at<float>(0), (int)prediction.at<float>(1));

			//更新測量座標
			measurement.at<float>(0) = (float)mousePosition.x;
			measurement.at<float>(1) = (float)mousePosition.y;

			//更新
			KF.correct(measurement);

			cout << measurement <<"\n";

			//畫出來
			img = Scalar::all(0);
			circle(img, predictPt, 5, CV_RGB(0, 255, 0), 3);//predicted point with green  
			circle(img, mousePosition, 5, CV_RGB(255, 0, 0), 3);//current position with red 

			imshow( "Kalman", img );
			int key = cvWaitKey(3);
			if (key == 27) {//esc     
				break;
			}
		}
		

	}
	};
}
