#include <iostream>
#include "pKinect.h"

using  namespace std;
using  namespace   cv;
using  namespace Sample;
using  namespace DirectX;

static DWORD  framenumber = 0; 
static DWORD  colornumber = 0;
static DWORD  depthnumber = 0;			//����֡���
static DWORD  tempnumber  = 0;

int vDetection,HeightDetection;
float  SpineHeightin, SpineHeightout, SpineV;

// Constructor
CBodyBasics::CBodyBasics() :
	mySensor(NULL),
	myColorReader(NULL),
	myDepthReader(NULL),
	myBodyReader(NULL),
	myMapper(NULL)
{}

// Destructor
CBodyBasics::~CBodyBasics()
{
	// done with frame reader
	SafeRelease(myColorReader);
	SafeRelease(myDepthReader);
	SafeRelease(myBodyReader);
	SafeRelease(myInfraredReader);

	// done with coordinate mapper
	SafeRelease(myMapper);

	// close the Kinect Sensor
	if (mySensor)
	{
		mySensor->Close();
	}
	SafeRelease(mySensor);
}

//��ʼ��
HRESULT CBodyBasics::InitKinect()
{
	HRESULT hr;
	hr = GetDefaultKinectSensor(&mySensor);
	if (mySensor) 
	{
		mySensor->Open();
		IFrameDescription	* myDescription = nullptr;

		IColorFrameSource	* myColorSource = nullptr;
		int COLORHEIGHT = 0, COLORWIDTH = 0;
		mySensor->get_ColorFrameSource(&myColorSource);
		myColorSource->get_FrameDescription(&myDescription);
		myDescription->get_Height(&COLORHEIGHT);
		myDescription->get_Width(&COLORWIDTH);
		myColorSource->OpenReader(&myColorReader);			//����ΪColor֡��׼����ֱ�ӿ���Reader

		IDepthFrameSource	* myDepthSource = nullptr;
		int DEPTHHEIGHT = 0, DEPTHWIDTH = 0;
		mySensor->get_DepthFrameSource(&myDepthSource);
		myDepthSource->get_FrameDescription(&myDescription);
		myDescription->get_Height(&DEPTHHEIGHT);
		myDescription->get_Width(&DEPTHWIDTH);
		myDepthSource->OpenReader(&myDepthReader);			//����ΪDepth֡��׼����ֱ�ӿ���Reader

		IBodyFrameSource	* myBodySource = nullptr;
		mySensor->get_BodyFrameSource(&myBodySource);
		myBodySource->OpenReader(&myBodyReader);			//����ΪBody֡��׼����ֱ�ӿ���Reader
		
		IInfraredFrameSource* myInfraredSource = nullptr;
		mySensor->get_InfraredFrameSource(&myInfraredSource);
		myInfraredSource->OpenReader(&myInfraredReader);

		SafeRelease(myColorSource);
		SafeRelease(myDepthSource);
		SafeRelease(myBodySource);
		SafeRelease(myInfraredSource);

		mySensor->get_CoordinateMapper(&myMapper);
	}
	if (!mySensor || FAILED(hr))
	{
		cout << "Kinect initialization failed!" << endl;
		return E_FAIL;
	}
	return hr;
}

//�������ͼ
void CBodyBasics::SaveBodyImg()
{
	stringstream stream0, stream1;
	string str, filepath, str1;
	filepath = "E:\\temp\\";			//���ù̶�����·��

	if (filepath.empty())
		filepath = "E:\\temp\\";			//����Ĭ�Ϲ̶�����·��
	else
		filepath += "/";

	//��ѡ���ļ��������ȡ��ǰѡ���·��
	stream1 << framenumber;        //��long����������
	stream1 >> str;					//ת��Ϊ string
	str1 = "�ɹ������ " + str + " ֡����ͼ\r\n";

	if (BodyImg.data){
		imwrite(filepath + str + "body.bmp", BodyImg);
		cout << str1;
		cout << str + "depth.bmp" << endl;}
	else{
		cout << "û�����ݣ�����ʧ��" << endl;}
}

//�������ͼ
void CBodyBasics::SaveDepthImg()
{
	stringstream stream0, stream1;
	string str, filepath, str1;
	filepath = "E:\\temp\\";			//���ù̶�����·��

	if (filepath.empty())
		filepath = "E:\\temp\\";			//����Ĭ�Ϲ̶�����·��
	else
		filepath += "/";

	stream1 << depthnumber;        //��long����������
	stream1 >> str;					//ת��Ϊ string
	str1 = "�ɹ������ " + str + " ֡���ͼ\r\n";

	if (DepthImg.data)
	{
		imwrite(filepath + str + "depth.bmp", DepthImg);
		cout << str1;
		cout << str + "depth.bmp" << endl;
	}
	else
	{
		cout << "û�����ݣ�����ʧ��" << endl;
	}
	/*if (InfraredImg.data)
	{
		imwrite(filepath + str + "Infrared.bmp", InfraredImg);
		cout << str1;
		cout << str + "Infrared.bmp" << endl;
	}
	else
	{
		cout << "û�����ݣ�����ʧ��" << endl;
	}*/
}

//�����ɫͼ
void CBodyBasics::SaveColorImg()
{
	stringstream stream0, stream1;
	string str, filepath, str1;
	//CMFC_DEMO01Dlg *pDlg0 = CMFC_DEMO01Dlg::s_pDlg;
	filepath = "E:\\temp\\";			//���ù̶�����·��
									//��ѡ���ļ��������ȡ��ǰѡ���·��
									//filepath = (pDlg0->m_edit).GetBuffer(0);
	if (filepath.empty())
		filepath = "E:\\temp\\";			//����Ĭ�Ϲ̶�����·��
	else
		filepath += "/";

	//��ѡ���ļ��������ȡ��ǰѡ���·��
	stream1 << colornumber;        //��long����������
	stream1 >> str;					//ת��Ϊ string
	str1 = "�ɹ������ " + str + " ֡��ɫͼ\r\n";
	
	if (ColorImg.data){
		imwrite(filepath + str + "color.bmp", ColorImg);
		cout << str1;
		cout << str + "color.bmp" << endl;}
	else{
		cout << "û�����ݣ�����ʧ��" << endl;}
}

//����ѯ��
void CBodyBasics::SpeechDetection()
{

}

//�����ʼ�����
void CBodyBasics::SendEmail()
{
	CSmtp smtp(
		25,								/*smtp�˿�*/
		"smtp.163.com",					/*smtp��������ַ*/
		"panet96@163.com",	/*��������ַ*/
		"PANCAST96",					/*��������*/
		"774834035@qq.com",	/*Ŀ�������ַ*/
		"WARNING",							/*����*/
		"�������˵�����������������"		/*�ʼ�����*/
		);

	//��Ӹ���ʱע��,\һ��Ҫд��\\����Ϊת���ַ���Ե��
	//string filePath("E:\\pic\\318body.bmp");
	//smtp.AddAttachment(filePath);

	//string filePath("E:\\pic\\318color.bmp");
	//smtp.AddAttachment(filePath);

	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "����1: �������粻��ͨ������ʧ��!" << endl;
		if (err == 2)
			cout << "����2: �û�������,��˶�!" << endl;
		if (err == 3)
			cout << "����3: �û����������˶�!" << endl;
		if (err == 4)
			cout << "����4: ���鸽��Ŀ¼�Ƿ���ȷ���Լ��ļ��Ƿ����!" << endl;
	}
	//system("pause");
}

//��ȡϵͳʱ��
string CBodyBasics::getTime()
{
	time_t timep;
	time (&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

void CBodyBasics::curl_test()
{
	curl_global_init(CURL_GLOBAL_ALL); // ����ȫ�ֳ�ʼ��CURL
	CURL* curl = curl_easy_init(); // ��ʼ��CURL���
	if (NULL == curl) {
		printf("(curl is NULL!\r\n");}
	// ����Ŀ��URL
	curl_easy_setopt(curl, CURLOPT_URL, "https://sc.ftqq.com/SCU25177T14196ec9449d9ea644db9ace6b4b91ba5adc30fcf2afa.send?text=warning&desp=SomeoneFellDownAtHome!!");
	// ִ��һ��URL����
	CURLcode res = curl_easy_perform(curl);
	// ����ɾ�
	curl_easy_cleanup(curl);	
}

//������
void CBodyBasics::DrawBody(const Joint * joints, const DepthSpacePoint*depthSpacePosition)
{
	// Draw the bones
	//---------------------------body-------------------------------
	DrawBone(joints, depthSpacePosition, JointType_Head, JointType_Neck);
	DrawBone(joints, depthSpacePosition, JointType_Neck, JointType_SpineShoulder);
	DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_SpineMid);
	DrawBone(joints, depthSpacePosition, JointType_SpineMid, JointType_SpineBase);
	DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_ShoulderRight);
	DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_ShoulderLeft);
	DrawBone(joints, depthSpacePosition, JointType_SpineBase, JointType_HipRight);
	DrawBone(joints, depthSpacePosition, JointType_SpineBase, JointType_HipLeft);

	// -----------------------Right Arm ------------------------------------ 
	DrawBone(joints, depthSpacePosition, JointType_ShoulderRight, JointType_ElbowRight);
	DrawBone(joints, depthSpacePosition, JointType_ElbowRight, JointType_WristRight);
	DrawBone(joints, depthSpacePosition, JointType_WristRight, JointType_HandRight);
	DrawBone(joints, depthSpacePosition, JointType_HandRight, JointType_HandTipRight);
	DrawBone(joints, depthSpacePosition, JointType_WristRight, JointType_ThumbRight);

	//----------------------------------- Left Arm--------------------------
	DrawBone(joints, depthSpacePosition, JointType_ShoulderLeft, JointType_ElbowLeft);
	DrawBone(joints, depthSpacePosition, JointType_ElbowLeft, JointType_WristLeft);
	DrawBone(joints, depthSpacePosition, JointType_WristLeft, JointType_HandLeft);
	DrawBone(joints, depthSpacePosition, JointType_HandLeft, JointType_HandTipLeft);
	DrawBone(joints, depthSpacePosition, JointType_WristLeft, JointType_ThumbLeft);

	// ----------------------------------Right Leg--------------------------------
	DrawBone(joints, depthSpacePosition, JointType_HipRight, JointType_KneeRight);
	DrawBone(joints, depthSpacePosition, JointType_KneeRight, JointType_AnkleRight);
	DrawBone(joints, depthSpacePosition, JointType_AnkleRight, JointType_FootRight);

	// -----------------------------------Left Leg---------------------------------
	DrawBone(joints, depthSpacePosition, JointType_HipLeft, JointType_KneeLeft);
	DrawBone(joints, depthSpacePosition, JointType_KneeLeft, JointType_AnkleLeft);
	DrawBone(joints, depthSpacePosition, JointType_AnkleLeft, JointType_FootLeft);

	// Draw the joints
	for (int j = 0; j < JointType_Count; ++j)
	{
		if (joints[j].TrackingState == TrackingState_Inferred)//��ȷ���Ĺؽڵ� ����ɫ
		{
			circle(BodyImg, CvPoint(depthSpacePosition[j].X, depthSpacePosition[j].Y), 3, cvScalar(0, 255, 255), -1, 8, 0);
		}
		else if (joints[j].TrackingState == TrackingState_Tracked) //ȷ���Ĺؽڵ� ����ɫ
		{
			circle(BodyImg, CvPoint(depthSpacePosition[j].X, depthSpacePosition[j].Y), 3, cvScalar(255, 0, 255), -1, 8, 0);
		}
	}
}

//������״̬
void CBodyBasics::DrawHandState(HandState handState, const DepthSpacePoint handPosition)
{
	//����ͬ�����Ʒ��䲻ͬ��ɫ
	CvScalar color;
	switch (handState)
	{
	case HandState_Open:
		color = cvScalar(255, 0, 0);	//�����ſ���״̬������ɫ��ʾ
		break;
	case HandState_Closed:
		color = cvScalar(0, 255, 0);	//���Ǳպϵ�״̬������ɫ��ʾ
		break;
	case HandState_Lasso:
		color = cvScalar(0, 0, 255);	//���ǽ����ſ��ͱպϵ�״̬���ú�ɫ��ʾ
		break;
	default:							//���û��ȷ�������ƣ��Ͳ�Ҫ��
		return;
	}

	circle(BodyImg,cvPoint(handPosition.X, handPosition.Y),20, color, -1);
}

//���Ǽ�
void CBodyBasics::DrawBone(const Joint * pJoints, const DepthSpacePoint * depthSpacePosition, JointType joint0, JointType joint1)
{
	TrackingState joint0State = pJoints[joint0].TrackingState;
	TrackingState joint1State = pJoints[joint1].TrackingState;

	// If we can't find either of these joints, exit
	if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
	{
		return;
	}

	// Don't draw if both points are inferred
	if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
	{
		return;
	}

	CvPoint p1 = cvPoint(depthSpacePosition[joint0].X, depthSpacePosition[joint0].Y),
		p2 = cvPoint(depthSpacePosition[joint1].X, depthSpacePosition[joint1].Y);

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
	{
		//�ǳ�ȷ���ĹǼܣ��ð�ɫֱ��
		line(BodyImg, p1, p2, cvScalar(255, 255, 255), 6);
	}
	else
	{
		//��ȷ���ĹǼܣ��ú�ɫֱ��
		line(BodyImg, p1, p2, cvScalar(0, 0, 255), 1);
	}
}

//�ǶȲ���
DOUBLE CBodyBasics::Angle(const DirectX::XMVECTOR* vec, JointType jointA, JointType jointB, JointType jointC)
{
	double angle = 0.0;

	XMVECTOR vBA = XMVectorSubtract(vec[jointB], vec[jointA]);
	XMVECTOR vBC = XMVectorSubtract(vec[jointB], vec[jointC]);

	XMVECTOR vAngle = XMVector3AngleBetweenVectors(vBA, vBC);

	angle = XMVectorGetX(vAngle) * 180.0 * XM_1DIVPI;    // XM_1DIVPI: An optimal representation of 1 / ��

	return angle;
}

//�������
double CBodyBasics::Distance(Joint p1, Joint p2)
{
	double dist = 0;
	dist = sqrt(pow(p2.Position.X - p1.Position.X, 2) +
		pow(p2.Position.Y - p1.Position.Y, 2) + pow(p2.Position.Z - p1.Position.Z, 2));
	return dist;
}

//���ݸ���
void CBodyBasics::Update()
{
	//��ʾϵͳʱ��
	string time = getTime();
	//*********************Color
	ColorImg.setTo(0);
	ColorImg.create(cColorHeight, cColorWidth, CV_8UC4);
	IColorFrame* myColorFrame = nullptr;
	while (myColorReader->AcquireLatestFrame(&myColorFrame) != S_OK);
	myColorFrame->CopyConvertedFrameDataToArray(cColorHeight * cColorWidth * 4, ColorImg.data, ColorImageFormat_Bgra);

	putText(ColorImg, time, cvPoint(0, 1000), CV_FONT_HERSHEY_COMPLEX, 2, cvScalar(0, 0, 255), 2);

	//��ɫ���ݴ�����ʾ�����д
	//OpenCV��ʾ
	namedWindow("ColorImg", 0);
	resizeWindow("ColorImg", 640, 480);
	imshow("ColorImg", ColorImg);
	waitKey(5);
	SafeRelease(myColorFrame);

	//*********************Depth
	DepthImg.setTo(0);
	DepthImg.create(cDepthHeight, cDepthWidth, CV_16UC1);
	Mat iDepthImg(cDepthHeight, cDepthWidth, CV_8UC1);
	IDepthFrame* myDepthFrame = nullptr;
	while (myDepthReader->AcquireLatestFrame(&myDepthFrame) != S_OK);
	myDepthFrame->CopyFrameDataToArray(cDepthHeight * cDepthWidth, reinterpret_cast<UINT16*>(DepthImg.data));
	DepthImg.convertTo(iDepthImg, CV_8U, 255.0f / 4500);//converto()��һ��������������󣬵ڶ�����ת�����ͣ����������������ӣ�����4500��������ݵ�������
	putText(DepthImg, time, cvPoint(0, 400), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
	//������ݴ�����ʾ�����д  
	//OpenCV��ʾ
	namedWindow("DepthImg", 0);
	resizeWindow("DepthImg", 640, 480);
	imshow("DepthImg",iDepthImg);
	waitKey(5);
	SafeRelease(myDepthFrame);

	//**********************Infrared
	/*int        iWidth = 0;
	int        iHeight = 0;
	IFrameDescription* pFrameDescription = nullptr;
	pFrameDescription->get_Width(&iWidth);
	pFrameDescription->get_Height(&iHeight);
	pFrameDescription->Release();
	pFrameDescription = nullptr;
	InfraredImg.setTo(0);
	InfraredImg.create(iHeight,iWidth,CV_16UC1);
	IInfraredFrame* myInfraredFrame = nullptr;
	while(myInfraredReader->AcquireLatestFrame(&myInfraredFrame) == S_OK);
	myInfraredFrame->CopyFrameDataToArray(iWidth * iHeight,reinterpret_cast<UINT16*>(InfraredImg.data));//ǿ��ת����������
	putText(InfraredImg, time, cvPoint(0, 400), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
	namedWindow("InfraredImage");
	imshow("InfraredImage", InfraredImg);*/

	//*********************Body
	BodyImg.setTo(0);
	BodyImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
	IBodyFrame* myBodyFrame = NULL;
	while (myBodyReader->AcquireLatestFrame(&myBodyFrame) != S_OK);
	IBody* myBodies[BODY_COUNT] = { 0 };
	while (myBodyFrame->GetAndRefreshBodyData(_countof(myBodies), myBodies) != S_OK);
	
	putText(BodyImg, time, cvPoint(0, 400), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
	
	ProcessBody(BODY_COUNT, myBodies);

	//Opencv��ʾ
	namedWindow("BodyImg", 0);
	resizeWindow("BodyImg", 640, 480);
	imshow("BodyImg", BodyImg);
	waitKey(5);
	for (int i = 0; i < _countof(myBodies); ++i)
	{
		SafeRelease(myBodies[i]);
	}
	SafeRelease(myBodyFrame);
}

//�������ݴ���
void CBodyBasics::ProcessBody(int nBodyCount, IBody ** aBodyData)
{
	HRESULT hr;
	FilterDoubleExponential filterKinect;	//������˫ָ���˲���

	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* myBody = aBodyData[i];
		if (myBody)
		{
			BOOLEAN bTracked = false;
			hr = myBody->get_IsTracked(&bTracked);  //ȷ��׷��״̬
			filterKinect.Update(myBody);			//ƽ�����ȶ�����Ǽ�

			if (SUCCEEDED(hr) && bTracked)
			{
				//�洢�ؽڵ���
				Joint joints[JointType::JointType_Count];
				HandState leftHandState = HandState_Unknown;//����״̬
				HandState rightHandState = HandState_Unknown;//����״̬

				//��ȡ������״̬
				myBody->get_HandLeftState(&leftHandState);
				myBody->get_HandRightState(&rightHandState);
				//�洢�������ϵ�йؽڵ�λ��
				DepthSpacePoint *depthSpacePosition = new DepthSpacePoint[_countof(joints)];
				
				//��ȡ�ؽڵ���
				if (myBody->GetJoints(JointType::JointType_Count, joints) == S_OK)
				{
					filterKinect.Update(joints);		//ƽ������ÿ��������
					
					const DirectX::XMVECTOR* vec = filterKinect.GetFilteredJoints();    // Retrive Filtered Joints

					double angle = Angle(vec, JointType_WristRight, JointType_ElbowRight, JointType_ShoulderRight); // Get ElbowRight joint angle
					char s[20];
					sprintf_s(s, "Angle:%.0f", angle);
					string strAngleInfo = s;
					putText(BodyImg, strAngleInfo, cvPoint(0, 50), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255)); // ��Ļ����ʾ�Ƕ���Ϣ
					
					for (int j = 0; j < _countof(joints); ++j)
					{
							float x = 0.0f, y = 0.0f, z = 0.0f;
							// Retrieve the x/y/z component of an XMVECTOR Data and storing that component's value in an instance of float referred to by a pointer
							XMVectorGetXPtr(&x, vec[j]);
							XMVectorGetYPtr(&y, vec[j]);
							XMVectorGetZPtr(&z, vec[j]); 

							CameraSpacePoint cameraSpacePoint = { x, y, z };
							myMapper->MapCameraPointToDepthSpace(cameraSpacePoint, &depthSpacePosition[j]);
							
							//���ؽڵ���������������ϵ��-1~1��ת���������ϵ��424*512��
							//myMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePosition[j]);
					}	

					DrawBody(joints, depthSpacePosition);

					DrawHandState(leftHandState, depthSpacePosition[JointType_HandLeft]);
					DrawHandState(rightHandState, depthSpacePosition[JointType_HandRight]);

					// ��ȡ�ؽڷ���
					JointOrientation aOrientations[JointType::JointType_Count];
					if (myBody->GetJointOrientations(JointType::JointType_Count, aOrientations) != S_OK)
					{
						cerr << "Get joints fail" << endl;
					}

					//savephoto
					if (tempnumber > 0) {
						if (framenumber == (tempnumber + 2)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
						if (framenumber == (tempnumber + 5)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
						if (framenumber == (tempnumber + 10)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
						if (framenumber == (tempnumber + 15)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
						if (framenumber == (tempnumber + 20)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
					}

					//falldetection
					if (joints[JointType_SpineBase].Position.Z > 0.5&&joints[JointType_SpineBase].Position.Z<3.5)
						Detection(joints);
					else
						cout << "Ϊ�˼���׼ȷ�ȣ��뾡��վ����Kinect 0.5--3.5 ��֮�䣬лл��ϣ�" << endl;
					
				}
				delete[] depthSpacePosition;
			}
		}
	}
	framenumber++;
	depthnumber++;
	colornumber++;
}

//������
void CBodyBasics::Detection(Joint joints[])
{
	
	//����ٶ�����
	static double tin, tout;
	//double tframe;
	//��Լ30֡Ϊ1s,����ÿ����30֡�ĸ߶Ȳ�Ӷ������ٶȣ�1��31��62....
	if (framenumber % 11 == 1)		//framenumber��֡���кţ��Լ������
	{
		tin = static_cast<double>(GetTickCount());
		//cout << "tin��" << tin << endl;
		SpineHeightin = joints[JointType_SpineMid].Position.Y;
		//-cout << "��ǰ֡��Ϊ��" << framenumber << endl;
		//cout << "��ǰSpineHeightin�ĸ߶�Ϊ" << SpineHeightin << "  m"<<endl;
	}
	if (!(framenumber % 11))
	{
		tout = static_cast<double>(GetTickCount());
		cout << framenumber << endl;
		//cout <<"tout��"<< tout << endl;
		cout << "ÿ10֡����һ���½����ٶ�" << endl;
		SpineHeightout = joints[JointType_SpineMid].Position.Y;
		cout << "��ǰ֡��Ϊ��" << framenumber << endl;
		 cout << "***********************************" << endl;
		// cout << "��ǰSpineHeightin�ĸ߶�Ϊ" << SpineHeightin << "  m" << endl;
		//tframe = (tout - tin) / getTickFrequency();
		// cout <<tframe << endl;
		// cout << getTickFrequency()<<endl;
		//cout << "��ǰSpineHeightout�ĸ߶�Ϊ" << SpineHeightout << "  m" << endl;
		//SpineV = (SpineHeightin - SpineHeightout) / tframe;
		SpineV = SpineHeightin - SpineHeightout;
		cout << "SpineV�Ƕ��٣���" << SpineV << endl;

		// ��Ļ����ʾ�ٶ���Ϣ
		char v[20];
		sprintf_s(v, "downSpeed:%.4f", SpineV);
		string strSpeedInfo = v;
		putText(BodyImg, strSpeedInfo, cvPoint(0, 100), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255)); 

		if ((SpineV) > 0.35)	//�����и�����������1.35m/s���������Ҫ����ʵ��������е���	
		{
			vDetection = true;
			tempnumber = framenumber;
			stringstream stream0;
			string str, str1;
			stream0 << SpineV;
			stream0 >> str;
			str1 = "�����������µ��ٶ��ǣ� " + str + " m/s\r\n";
			cout << "�����������µ��ٶ��ǣ�   " << (SpineV) << "    m/s" << endl;
		}
		else vDetection = false;
	}

	//���߶��������Ե���ļ�⣬�����ľ��롣����ת��Ϊspine��foot֮��ĸ߶ȡ�
	if ((joints[JointType_SpineBase].Position.Y - joints[JointType_FootRight].Position.Y) <= 0.5)//0.22
	{
		waitKey(15);//��ʱ��⣬��ֹ����             waitkey()�����������ǲ���ˢ��ͼ��Ƶ��ʱ����delay����λ��ms
		if ((joints[JointType_SpineBase].Position.Y - joints[JointType_FootRight].Position.Y) <= 0.5)
		{
			if (vDetection)
			{
				HeightDetection = TRUE;
				vDetection = false;

				stringstream stream0, stream1;
				string str, str1, str2;
				stream0 << joints[JointType_SpineBase].Position.Y;
				stream0 >> str;
				str1 = "JointType_SpineBase�ĸ߶��ǣ� " + str + " m\r\n";
				stream1 << joints[JointType_FootRight].Position.Y;
				stream1 >> str;
				str2 = "JointType_FootRight�ĸ߶��ǣ� " + str + " m\r\n";

				cout << "JointType_SpineBase�ĸ߶���  " << joints[JointType_SpineBase].Position.Y << "\tm" << endl;
				cout << "***************" << endl;
				cout << "JointType_FootRight�ĸ߶���  " << joints[JointType_FootRight].Position.Y << "\tm" << endl;
				cout << "***************" << "�ɹ�������" << "***************" << endl;
				
				// ��Ļ����ʾ�߶���Ϣ
				char h1[20];
				sprintf_s(h1, "SpineBaseH:%.4f", joints[JointType_SpineBase].Position.Y);
				string strSpineBaseHInfo = h1;
				putText(BodyImg, strSpineBaseHInfo, cvPoint(0, 150), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
				char h2[20];
				sprintf_s(h2, "RightFootH:%.4f", joints[JointType_FootRight].Position.Y);
				string strRightFootHInfo = h2;
				putText(BodyImg, strRightFootHInfo, cvPoint(0, 170), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));

				SaveBodyImg();	//��⵽�����¼������浱ǰͼƬ��Ϣ
				SaveColorImg();
				SaveDepthImg();
				//SpeechDetection();		//����ѯ��
				SendEmail();
				curl_test();
			}
		}
		else
		{
			HeightDetection = FALSE;
		}
	}
} 