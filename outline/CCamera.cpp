#include "CCamera.h"
#include "../glm/gtc/matrix_transform.hpp"

CCamera::CCamera() {
	m_pos = glm::vec3(0.0, 0.0, 10.0);
	m_worldUp = glm::vec3(0.0, 1.0, 0.0);
	m_sensitivity = 0.05;
	m_moveSpeed = 2.5;
	m_pitch = 0.0;
	m_yaw = -90.0;
	update();
}

CCamera::~CCamera() {

}

void CCamera::left(float deltaTime) {
	m_pos -= m_right * m_moveSpeed * deltaTime;
}

void CCamera::right(float deltaTime) {
	m_pos += m_right * m_moveSpeed * deltaTime;
}

void CCamera::up(float deltaTime) {
	m_pos += m_direct * m_moveSpeed * deltaTime;
}

void CCamera::down(float deltaTime) {
	m_pos -= m_direct * m_moveSpeed * deltaTime;
}

void CCamera::setMoveSpeed(float moveSpeed) {
	m_moveSpeed = moveSpeed;
}

void CCamera::setMouseSensitivity(float sensitivity) {
	m_sensitivity = sensitivity;
}

glm::mat4 CCamera::getViewMat() {
	return glm::lookAt(m_pos, m_pos + m_direct, m_up);
}

void CCamera::adjustPitchYaw(float dX, float dY) {
	dX *= m_sensitivity;
	m_yaw += dX;
	dY *= m_sensitivity;
	if (m_pitch + dY > 89.0)
		m_pitch = 89.0;
	else if (m_pitch + dY < -89.0)
		m_pitch = -89.0;
	else
		m_pitch += dY;
	update();
}

void CCamera::update() {
	glm::vec3 tmp;
	tmp.x = glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
	tmp.z = glm::cos(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw));
	tmp.y = glm::sin(glm::radians(m_pitch));
	m_direct = glm::normalize(tmp);
	m_right = glm::cross(m_direct, m_worldUp);
	m_up = glm::cross(m_right, m_direct);
}

void CCamera::setPitch(float pitch) {
	m_pitch = pitch;
}

void CCamera::setYaw(float yaw) {
	m_yaw = yaw;
}