#pragma once

#include <TritiumEngine/Core/Scriptable.hpp>

#include <memory>

namespace TritiumEngine::Core
{
  class NativeScript {
  public:
    NativeScript(std::unique_ptr<Scriptable> scriptable) : m_instance(std::move(scriptable)) {}

    Scriptable &getInstance() { return *m_instance; }

  private:
    std::unique_ptr<Scriptable> m_instance;
  };
} // namespace TritiumEngine::Core