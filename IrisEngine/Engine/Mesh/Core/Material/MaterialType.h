#pragma once

enum EMaterialType
{
	//�ֲ�����ģ��
	Lambert = 0,			//ֽ��
	HalfLambert,			//�ֲڱ���
	Phong,					//������Ч��
	BinnPhong,				//������Ч��
	Wrap,					//ģ��Ƥ��Ч��
	Minnaert,				//������� ����
	Banded,					//������ͨЧ��
	GradualBanded,			//����Ŀ�ͨ����
	FinalBanded,			//�ں��˺ܶ�Ч���Ŀ�ͨ����
	Back,					//������Ч��
	AnisotropyKajiyaKay,	//GDC 2004�� KajiyaKay ͷ����Ⱦģ��
	OrenNayar,				//GDC ģ��ֲڱ���

	//������ʾ��ӦЧ��
	BaseColor,				//�������ɫ
	Normal,					//��ʾģ�Ϳռ䷨��
	WorldNormal,			//��ʾ����ռ䷨��

	PBR = 20,				//�����������ʾ

	Fresnel = 100,			//����ȥ����Ч�����������
	Max,
};

enum EMaterialDisplayStatusType
{
	DefaultDisplay,//Ĭ����ʾ
	PointDisplay,//��ʾ��
	WireframeDisplay,//�߿�
	TriangleDisplay = 4,//��������ʾ
};