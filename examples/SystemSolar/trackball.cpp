#include "trackball.hpp"

#include <algorithm>
#include <limits>

void TrackBall::resizeViewport(glm::ivec2 const &size) {
  m_viewportSize = size;
}

glm::mat4 TrackBall::getRotation() const {
  if (m_mouseTracking)
    return m_rotation;

  // Rotate by velocity when not tracking to simulate an inertia-free rotation
  auto const angle{m_velocity * gsl::narrow_cast<float>(m_lastTime.elapsed())};

  return glm::rotate(glm::mat4(1.0f), angle, m_axis) * m_rotation;
}
