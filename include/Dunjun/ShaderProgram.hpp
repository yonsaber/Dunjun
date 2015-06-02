#ifndef DUNJUN_SHADERPROGRAM_HPP
#define DUNJUN_SHADERPROGRAM_HPP

#include <Dunjun/OpenGL.hpp>
#include <Dunjun/NonCopyable.hpp>

#include <Dunjun/Common.hpp>

#include <Dunjun/Math/Matrix4.hpp>

#include <map>

#include <string>

namespace Dunjun
{
enum class ShaderType
{
	Vertex,
	Fragment
};

class ShaderProgram : public NonCopyable
{
public:
	ShaderProgram();          // Constructor
	virtual ~ShaderProgram(); // Destructor (virtual to prevent inheiritance)

	bool attachShaderFromFile(ShaderType type, const std::string& filename);
	bool attachShaderFromMemory(ShaderType type, const std::string& filename);

	void use() const;       // Use the shaders
	bool isInUse() const;   // Check if a shader is in use
	void stopUsing() const; // Stop using a shader

	bool link();     // Link a shader
	bool isLinked(); // Link a shader

	void bindAttribLocation(GLuint location, const GLchar* name);

	GLint getAttribLocation(const GLchar* name);
	GLint getUniformLocation(const GLchar* name);

	void setUniform(const GLchar* name, f32 x);
	void setUniform(const GLchar* name, f32 x, f32 y);
	void setUniform(const GLchar* name, f32 x, f32 y, f32 z);
	void setUniform(const GLchar* name, f32 x, f32 y, f32 z, f32 w);
	void setUniform(const GLchar* name, u32 x);
	void setUniform(const GLchar* name, i32 x);
	void setUniform(const GLchar* name, bool x);
	void setUniform(const GLchar* name, const Vector2& v);
	void setUniform(const GLchar* name, const Vector3& v);
	void setUniform(const GLchar* name, const Vector4& v);
	void setUniform(const GLchar* name, const Matrix4& m);

	inline GLuint getObject() const { return m_object; }
	inline const std::string& getErrorLog() const { return m_errorLog; }

private:
	GLuint m_object;
	bool m_linked;
	std::string m_errorLog;

	std::map<const GLchar*, GLint> m_attribLocations;
	std::map<const GLchar*, GLint> m_uniformLocations;
};
} // namespace Dunjun

#endif
