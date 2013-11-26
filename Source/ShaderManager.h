#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <map>
#include <string>
#include <SFML\Graphics\Shader.hpp>

using namespace std;
using namespace sf;

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	/// Loads the name.vert and name.frag files as shaders
	/// from the shader directory (currently, from the current folder).
	bool loadShader(const string &name);

	/// Returns the specified shader.
	/// NOTE: For performance reasons it doesn't check
	///       if the shader exists. May raise an exception... I think.
	Shader* getShader(const string &name);

	/// Removes all shaders.
	void clear();

private:
	map<string, Shader*> m_shaders;
};

#endif
