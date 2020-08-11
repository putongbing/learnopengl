#pragma once

#include "common.h"
#include <string>

class CTexture2D
{
public:
	CTexture2D();
	~CTexture2D();
	/*setFilter������������˷�ʽ
	param-filterType:��������
	param-filter:���˷���
	*/
	void setFilter(unsigned int filterType, unsigned int filter);
	/*setWrap�����������Ʒ�ʽ
	param-wrapType:��������
	param-wrap:���Ʒ���
	*/
	void setWrap(unsigned int wrapType, unsigned int wrap);
	/*
	useTexture����������Ԫ��������
	param-texUnit:����Ԫ
	*/
	void useTexture(unsigned int texUnit);
	/*
	loadTexture����ͼƬ����������������󶨵�GPU�Դ�
	param-path:����·��
	param-level:mipmap�ȼ�
	param-internalformat:�������ͨ��
	param-format:�������ݸ�ʽ
	param-pixelType:������������
	return:���سɹ�����true����֮����
	*/
	bool loadTexture(const char * path, int level, unsigned int internalformat, unsigned int format, unsigned int pixelType);
private:
	unsigned int m_texID;
};

