#pragma once
namespace Grengine {
	/// <summary>
	/// Interface to the application layer
	/// This should be supplied by the Game project
	/// </summary>
	class Grapplication
	{
	public:
		/// <summary>
		/// Game logic
		/// </summary>
		/// <param name="dt">Time since the last Update</param>
		virtual void Update(double dt) = 0;
		/// <summary>
		/// Render logic
		/// </summary>
		virtual void Render() = 0;
	};
	
	extern Grapplication* CreateApp(int argc, char** argv);
}