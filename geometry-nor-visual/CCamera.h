#pragma once

#include "../glm/glm.hpp"

class CCamera
{
public:
	CCamera();
	~CCamera();
	//摄像机左移
	void left(float deltaTime);
	//摄像机右移
	void right(float deltaTime);
	//摄像机前移
	void up(float deltaTime);
	//摄像机后移
	void down(float deltaTime);
	void setMoveSpeed(float moveSpeed);
	void setMouseSensitivity(float sensitivity);
	glm::mat4 getViewMat();
	void adjustPitchYaw(float dX, float dY);
	void setPitch(float pitch);
	void setYaw(float yaw);
	glm::vec3 getPos() { return m_pos; }
	glm::vec3 getDir() { return m_direct; }
	glm::mat4 getProjectionMat() {
		return m_proj;
	}
	void setProjectionMat(glm::mat4 proj) {
		m_proj = proj;
	}
protected:
	void update();
private:
	//摄像机位置
	glm::vec3	m_pos;
	//摄像机方向
	glm::vec3	m_direct;
	//摄像机上向量
	glm::vec3	m_up;
	//摄像机右向量
	glm::vec3	m_right;
	//摄像机世界坐标上向量，用来计算右向量
	glm::vec3	m_worldUp;
	//摄像机移动速度
	float		m_moveSpeed;
	//鼠标灵敏度
	float		m_sensitivity;
	//仰俯角
	float		m_pitch;
	//偏航角
	float		m_yaw;
	glm::mat4	m_proj;
};

