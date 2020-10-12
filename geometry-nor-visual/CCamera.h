#pragma once

#include "../glm/glm.hpp"

class CCamera
{
public:
	CCamera();
	~CCamera();
	//���������
	void left(float deltaTime);
	//���������
	void right(float deltaTime);
	//�����ǰ��
	void up(float deltaTime);
	//���������
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
	//�����λ��
	glm::vec3	m_pos;
	//���������
	glm::vec3	m_direct;
	//�����������
	glm::vec3	m_up;
	//�����������
	glm::vec3	m_right;
	//�����������������������������������
	glm::vec3	m_worldUp;
	//������ƶ��ٶ�
	float		m_moveSpeed;
	//���������
	float		m_sensitivity;
	//������
	float		m_pitch;
	//ƫ����
	float		m_yaw;
	glm::mat4	m_proj;
};

