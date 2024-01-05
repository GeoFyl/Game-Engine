#include "pch.h"
#include "Subsystem.h"
#include "SystemsLocator.h"

void Toffee::Internal::Subsystem::ProvideSystem(Subsystem* subsystem) { Toffee::ToffeeAPI::Provide(subsystem); }
