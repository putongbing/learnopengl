#pragma once

#include "common.h"
#include <string>

class CTexture2D
{
public:
	CTexture2D();
	~CTexture2D();
	/*setFilter：设置纹理过滤方式
	param-filterType:过滤类型
	param-filter:过滤方法
	*/
	void setFilter(unsigned int filterType, unsigned int filter);
	/*setWrap：设置纹理环绕方式
	param-wrapType:环绕类型
	param-wrap:环绕方法
	*/
	void setWrap(unsigned int wrapType, unsigned int wrap);
	/*
	useTexture：生成纹理单元并绑定纹理
	param-texUnit:纹理单元
	*/
	void useTexture(unsigned int texUnit);
	/*
	loadTexture：从图片加载纹理，并将纹理绑定到GPU显存
	param-path:纹理路径
	param-level:mipmap等级
	param-internalformat:纹理组件通道
	param-format:像素数据格式
	param-pixelType:像素数据类型
	return:加载成功返回true，反之成立
	*/
	bool loadTexture(const char * path, int level, unsigned int internalformat, unsigned int format, unsigned int pixelType);
private:
	unsigned int m_texID;
};

