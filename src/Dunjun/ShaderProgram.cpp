#include <Dunjun/ShaderProgram.hpp>

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

namespace Dunjun
{
INTERNAL std::string stringFromFile(const std::string& filename)
{
	std::ifstream file;
	// Filename, readmode
	file.open(filename.c_str(), std::ios::in | std::ios::binary);

	std::string output;
	std::string line;

	if (!file.is_open())
	{
		std::runtime_error(std::string("Failed to open file: ") + filename);
	}
	else
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}

	file.close();
	return output;
}

ShaderProgram::ShaderProgram()
: m_object(0)
, m_linked(false)
, m_errorLog()
{
	m_object = glCreateProgram();
}
ShaderProgram::~ShaderProgram()
{
	if (m_object)
	{
		glDeleteProgram(m_object);
	}
}

bool ShaderProgram::attachShaderFromFile(ShaderType type,
                                         const std::string& filename)
{
	std::string source = stringFromFile(filename);
	return attachShaderFromMemory(type, source);
}

bool ShaderProgram::attachShaderFromMemory(ShaderType type,
                                           const std::string& source)
{
	if (!m_object)
	{
		m_object = glCreateProgram();
	}

	const char* shaderSource = source.c_str();
	GLuint shader;

	if (type == ShaderType::Vertex)
	{
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == ShaderType::Fragment)
	{
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}

	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog);
		msg.append(strInfoLog);
		delete[] strInfoLog;

		msg.append("\n");

		m_errorLog.append(msg);

		glDeleteShader(shader);

		return false;
	}

	glAttachShader(m_object, shader);

	return true; // TODO: Change
}

void ShaderProgram::use() const
{
	if (!isInUse())
	{
		glUseProgram(m_object);
	}
}

bool ShaderProgram::isInUse() const
{
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	return (currentProgram == (GLint)m_object);
}

void ShaderProgram::stopUsing() const
{
	if (isInUse())
	{
		glUseProgram(0);
	}
}

bool ShaderProgram::link()
{
	if (!m_linked)
	{
		glLinkProgram(m_object);

		GLint status;
		glGetProgramiv(m_object, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			std::string msg("Program linking failure: \n");

			GLint infoLogLength;
			glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* strInfoLog = new char[infoLogLength + 1];
			glGetProgramInfoLog(m_object, infoLogLength, NULL, strInfoLog);
			msg.append(strInfoLog);
			delete[] strInfoLog;

			msg.append("\n");
			m_errorLog.append(msg);

			glDeleteProgram(m_object);
			m_object = 0;

			m_linked = false;
			return m_linked;
		}

		m_linked = true;
	}

	return m_linked;
}

bool ShaderProgram::isLinked() { return m_linked; }

void ShaderProgram::bindAttribLocation(GLuint location, const GLchar* name)
{
	glBindAttribLocation(m_object, location, name);
	m_attribLocations[name] = location;
}

GLint ShaderProgram::getAttribLocation(const GLchar* name)
{
	auto found = m_attribLocations.find(name);

	if (found != m_attribLocations.end())
	{
		return found->second;
	}

	GLint loc = glGetAttribLocation(m_object, name);
	m_attribLocations[name] = loc;
	return loc;
}

GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
	auto found = m_uniformLocations.find(name);

	if (found != m_uniformLocations.end())
	{
		return found->second;
	}

	GLint loc = glGetUniformLocation(m_object, name);
	m_uniformLocations[name] = loc;
	return loc;
}

void ShaderProgram::setUniform(const GLchar* name, f32 x)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform1f(loc, x);
}

void ShaderProgram::setUniform(const GLchar* name, f32 x, f32 y)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform2f(loc, x, y);
}

void ShaderProgram::setUniform(const GLchar* name, f32 x, f32 y, f32 z)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform3f(loc, x, y, y);
}

void ShaderProgram::setUniform(const GLchar* name, f32 x, f32 y, f32 z, f32 w)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform4f(loc, x, y, y, z);
}

void ShaderProgram::setUniform(const GLchar* name, u32 x)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform1ui(loc, x);
}

void ShaderProgram::setUniform(const GLchar* name, i32 x)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform1i(loc, x);
}

void ShaderProgram::setUniform(const GLchar* name, bool x)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform1i(loc, x);
}

void ShaderProgram::setUniform(const GLchar* name, const Vector2& v)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform2fv(loc, 1, v.data);
}

void ShaderProgram::setUniform(const GLchar* name, const Vector3& v)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform3fv(loc, 1, v.data);
}

void ShaderProgram::setUniform(const GLchar* name, const Vector4& v)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniform4fv(loc, 1, v.data);
}

void ShaderProgram::setUniform(const GLchar* name, const Matrix4& m)
{
	if (!isInUse())
	{
		use();
	}

	GLint loc = getUniformLocation(name);
	if (loc == -1)
	{
		return;
	}

	glUniformMatrix4fv(loc, 1, GL_FALSE, m[0].data);
}

} // namespace Dunjun
