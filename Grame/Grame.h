#pragma once
#include <Grengine.h>
#include <string>

class Grame : public Grengine::Grapplication
{
public:
	Grame();
	void Update(double dt) override final;
	void Render() override final;

private:

	std::string lorem[10] = { " Lorem", " ipsum", " dolor", " sit", " amet", " consectetur", " adipiscing", " elit", " sed", " do"};
	int word = 0;
};
