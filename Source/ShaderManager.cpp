#include "ShaderManager.h"


ShaderManager::ShaderManager(){
}


ShaderManager::~ShaderManager(){
	m_shaders.clear();
}

bool ShaderManager::loadShader(const string &name){
	if(!Shader::isAvailable())
		return false;

	Shader *current = new Shader;

	bool success = current->loadFromFile(name+".vert",name+".frag");

	if(!success){
		delete current;
		return false;
	}

	// If a shader with the same name already exists
	// we delete it and replace it.
	if(m_shaders.find(name) != m_shaders.end()){
		delete m_shaders[name];
	}

	m_shaders[name] = current;
}


Shader* ShaderManager::getShader(const string &name){
	return m_shaders[name];
}


void ShaderManager::clear(){
	map<string, Shader*>::iterator it;
	for(it = m_shaders.begin(); it!=m_shaders.end(); ++it){
		delete it->second;
	}

	m_shaders.clear();
}


