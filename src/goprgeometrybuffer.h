#ifndef GOPRGEOMETRYBUFFER_H
#define GOPRGEOMETRYBUFFER_H

#include <QOpenGLFunctions>
#include <QImage>
#include <QList>
#include <QObject>

class GoPrImageStream;
class GoPrGeometryBuffer : public QObject
{
   Q_OBJECT
public:
    GoPrGeometryBuffer();

    void init(const QString &fname);
    void init(const QImage  &slide);
    void init(GoPrImageStream *slide);
    bool animate();
    void destroy();

    inline QSize  imageSize() const       { return mCurrentImage.size(); }
    inline float  imageRatio() const      { return mCurrentImage.height() != 0 ? (mCurrentImage.width()/(float)mCurrentImage.height()) : 0.0f; }
    inline GLuint vertexBufferId()  const { return mVertexBufferId;  }
    inline GLuint elementBufferId() const { return mElementBufferId; }
    inline GLuint textureId() const       { return mTextureId;       }

private slots:
    void updateBuffer(const QImage &image);

private:
    void initBuffer();
    void releaseGlIds();

    GLuint mVertexBufferId;
    GLuint mElementBufferId;
    GLuint mTextureId;

    QImage           mCurrentImage;
    GoPrImageStream *mStream;
    bool             mCurrentUpdated;
};

typedef QList<GoPrGeometryBuffer*>  GoPrGeometryBuffers;

#endif // GOPRGEOMETRYBUFFER_H
