#include <QOpenGLFunctions>
#include "goprshader.h"
#include "goprgeometrybuffer.h"

static inline void MatrixAsUniform(QOpenGLFunctions *f, GLuint location, QMatrix4x4 m) { f->glUniformMatrix4fv(location,1,GL_TRUE,m.data()); }

//-------------------------------------------------------------------------------------------------
GoPrShader::GoPrShader(const QString &eglFile)
{
    mVertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    mVertexShader->compileSourceFile(QString(":/%1.vert").arg(eglFile));
    Q_ASSERT(mVertexShader->isCompiled());

    mFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    mFragmentShader->compileSourceFile(QString(":/%1.frag").arg(eglFile));
    Q_ASSERT(mFragmentShader->isCompiled());


    mShaderProgram = new QOpenGLShaderProgram();
    mShaderProgram->addShader(mVertexShader);
    mShaderProgram->addShader(mFragmentShader);

    bool done = mShaderProgram->link();
    Q_ASSERT_X(done,"Shader Linker", mShaderProgram->log().toUtf8().data());

    // Get locations of attributes and uniforms used inside.
    vertexPosition   = mShaderProgram->attributeLocation("vertex" );
    vertexNormal     = mShaderProgram->attributeLocation("vnormal" );
    vertexTexcoords  = mShaderProgram->attributeLocation("texcoords" );

    modelviewMatrix  = mShaderProgram->uniformLocation("modelview" );
    normalviewMatrix = mShaderProgram->uniformLocation("normalview" );
    projectionMatrix = mShaderProgram->uniformLocation("projection" );
    defaultTexture   = mShaderProgram->uniformLocation("texture" );
}

//-------------------------------------------------------------------------------------------------
void GoPrShader::bind(const QMatrix4x4 &perspectiveMatrix)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Activate shader.
    // f->glUseProgram( mShaderProgram->programId() );
    mShaderProgram->bind();

    MatrixAsUniform(f,projectionMatrix,perspectiveMatrix);
}

//-------------------------------------------------------------------------------------------------
void GoPrShader::drawBuffer(const GoPrGeometryBuffer &buffer, const QMatrix4x4 &modelView)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Vertices
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.vertexBufferId() );
    f->glVertexAttribPointer(
                vertexPosition,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 9,
                (void*)0
                );
    f->glEnableVertexAttribArray( vertexPosition );

    // Normals
    f->glVertexAttribPointer(
                vertexNormal,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 9,
                (void*)(sizeof(GLfloat) * 4)
                );
    f->glEnableVertexAttribArray( vertexNormal );

    // Texture coordinates
    f->glVertexAttribPointer(
                vertexTexcoords,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 9,
                (void*)(sizeof(GLfloat) * 7)
                );
    f->glEnableVertexAttribArray( vertexTexcoords );

    QMatrix4x4 ratioView = modelView;
    ratioView.scale(buffer.imageRatio(), 1, buffer.imageRatio());
    MatrixAsUniform( f, modelviewMatrix, ratioView );

    // Normal view matrix - inverse transpose of modelview.
    QMatrix4x4 normalView = modelView.inverted().transposed();
    MatrixAsUniform(f, normalviewMatrix, normalView );

    // Textures
    f->glActiveTexture( GL_TEXTURE0 );
    f->glBindTexture( GL_TEXTURE_2D, buffer.textureId() );
    f->glUniform1i( defaultTexture, 0 );

    // Send element buffer to GPU and draw.
    f->glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer.elementBufferId() );
    f->glDrawElements(
                GL_QUADS,
                4,
                GL_UNSIGNED_SHORT,
                (void*)0
                );

    // Clean up
    f->glDisableVertexAttribArray( vertexPosition );
    f->glDisableVertexAttribArray( vertexTexcoords );
}

