#ifndef GOPRSHADER_H
#define GOPRSHADER_H


#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class GoPrGeometryBuffer;
class GoPrShader
{
public:
    GoPrShader(const QString &eglFile);

    void bind(const QMatrix4x4 &perspectiveMatrix);
    void drawBuffer(const GoPrGeometryBuffer &buffer, const QMatrix4x4 &modelView);

private:

    // These are variables passed into shaders
    GLuint vertexPosition;
    GLuint vertexNormal;
    GLuint vertexTexcoords;
    GLuint modelviewMatrix;
    GLuint normalviewMatrix;
    GLuint projectionMatrix;
    GLuint defaultTexture;

    QOpenGLShader         *mVertexShader;
    QOpenGLShader         *mFragmentShader;
    QOpenGLShaderProgram  *mShaderProgram;
};

#endif // GOPRSHADER_H
