#pragma once

#include "Rigidbody.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Core/Transform.hpp>

using namespace TritiumEngine::Core;

class Gravity : public System {
public:
  Gravity(float g = 0.05f) : System(), g(g) {}

  void update(float dt) override {
    m_app->registry.view<Rigidbody, Transform>().each(
        [&](auto entity, Rigidbody &rigidbody, Transform &transform) {
          float acceleration = -g * dt;
          rigidbody.velocity.y += acceleration;
          transform.setPosition(transform.getPosition() + rigidbody.velocity);
        });
  }

  float g;
};