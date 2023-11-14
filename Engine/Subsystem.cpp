#include "pch.h"
#include "Subsystem.h"
#include "SystemsLocator.h"

void Engine::Internal::Subsystem::ProvideSystem(Subsystem* subsystem) { Engine::SystemsAPI::Provide(subsystem); }
