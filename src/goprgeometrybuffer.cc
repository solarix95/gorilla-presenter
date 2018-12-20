
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include "goprgeometrybuffer.h"
#include "streams/goprimagestream.h"

#ifndef Scalar
#define Scalar float
#endif

typedef struct Vector { Scalar x,y,z; } Vector;

// Vertices, for use later
typedef struct vertex_t {
    Vector p;
    float w;
    Vector n;
    float s; // status
    float t; // texture-coordinate
} vertex;

// These are vertices and normals for a cube.
// They're interleaved [array-of-struct], which isn't ideal for processing
// on the CPU, but nice to read.
/*
vertex vertices[] = {
    { {  1.0,  1.0,  1.0 }, 1.0, {  0.0,  0.0,  1.0 }, 0.0, 0.0 },
    { { -1.0,  1.0,  1.0 }, 1.0, {  0.0,  0.0,  1.0 }, 1.0, 0.0 },
    { { -1.0, -1.0,  1.0 }, 1.0, {  0.0,  0.0,  1.0 }, 1.0, 1.0 },
    { {  1.0, -1.0,  1.0 }, 1.0, {  0.0,  0.0,  1.0 }, 0.0, 1.0 }
};
*/

vertex vertices[] = {
    { {  1.0,  1.0,  0.0 }, 1.0, {  0.0,  0.0,  1.0 }, 1.0, 0.0 },
    { { -1.0,  1.0,  0.0 }, 1.0, {  0.0,  0.0,  1.0 }, 0.0, 0.0 },
    { { -1.0, -1.0,  0.0 }, 1.0, {  0.0,  0.0,  1.0 }, 0.0, 1.0 },
    { {  1.0, -1.0,  0.0 }, 1.0, {  0.0,  0.0,  1.0 }, 1.0, 1.0 }
};

// Element data - just draw all vertices in order
GLushort elementData[] = {
    0,  1,  2,  3};

// Simple helper to make a single buffer object.
GLuint makeBO( QOpenGLFunctions *f, GLenum type, void* data, GLsizei size, int accessFlags ) {
    GLuint bo;
    f->glGenBuffers( 1, &bo );
    f->glBindBuffer( type, bo );
    f->glBufferData( type, size, data, accessFlags );
    return( bo );
}

GLuint loadTexture(const QImage &image) {

    QOpenGLTexture *texture = new QOpenGLTexture(image);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    return texture->textureId();
}

//-------------------------------------------------------------------------------------------------
GoPrGeometryBuffer::GoPrGeometryBuffer()
 : mVertexBufferId(0), mElementBufferId(0), mTextureId(0), mStream(NULL), mCurrentUpdated(false)
{

}

//-------------------------------------------------------------------------------------------------
void GoPrGeometryBuffer::init(const QString &fname)
{
    mCurrentImage = QImage(fname.isEmpty() ? QString("lba.jpg"):fname);
    initBuffer();
}

//-------------------------------------------------------------------------------------------------
void GoPrGeometryBuffer::init(const QImage &slide)
{
    mCurrentImage = slide;
    initBuffer();
}

//-------------------------------------------------------------------------------------------------
void GoPrGeometryBuffer::init(GoPrImageStream *stream)
{
    Q_ASSERT(stream);
    mStream = stream;
    connect(mStream, SIGNAL(frame(QImage)), this, SLOT(updateBuffer(QImage)));
    mStream->init();
    initBuffer();
}

//-------------------------------------------------------------------------------------------------
bool GoPrGeometryBuffer::animate()
{
    if (!mStream)
        return false;
    if (mCurrentImage.isNull() || !mCurrentUpdated)
        return false;

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (mTextureId > 0)
        f->glDeleteTextures(1,&mTextureId);

    mTextureId      = loadTexture(mCurrentImage);
    mCurrentUpdated = false;
    return true;
}

//-------------------------------------------------------------------------------------------------
void GoPrGeometryBuffer::destroy()
{
   releaseGlIds();
   if (mStream) {
       mStream->stop();
       mStream->deleteLater();
       mStream = NULL;
   }
}

//-------------------------------------------------------------------------------------------------
void GoPrGeometryBuffer::updateBuffer(const QImage &image)
{
    mCurrentImage   = image;
    mCurrentUpdated = true;
}

//-------------------------------------------------------------------------------------------------
void GoPrGeometryBuffer::initBuffer()
{
    releaseGlIds();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    mVertexBufferId  = makeBO(f,GL_ARRAY_BUFFER,vertices,sizeof(vertices),GL_STATIC_DRAW);
    mElementBufferId = makeBO(f, GL_ELEMENT_ARRAY_BUFFER, elementData, sizeof( elementData ), GL_STATIC_DRAW);
    mTextureId       = loadTexture(mCurrentImage);
}

//-------------------------------------------------------------------------------------------------
void GoPrGeometryBuffer::releaseGlIds()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (mVertexBufferId > 0)
        f->glDeleteBuffers(1,&mVertexBufferId);
    if (mElementBufferId > 0)
        f->glDeleteBuffers(1,&mElementBufferId);
    if (mTextureId > 0)
        f->glDeleteTextures(1,&mTextureId);
}



