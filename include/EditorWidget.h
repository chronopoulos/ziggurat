#include <QFrame>
#include <QGraphicsView>
#include <QTransform>

#include "Canvas.h"

class EditorWidget : public QFrame
{
    Q_OBJECT

    public:
        EditorWidget();
        void setDefaultCanvas(void);

    private:
        QVBoxLayout *layout;
        QGraphicsView *graphicsView;
        QTransform *transform;
        Canvas *defaultCanvas;

    public slots:
        void setCanvas(Canvas*);

    protected:
        void focusInEvent(QFocusEvent*);
        void focusOutEvent(QFocusEvent*);
        void resizeEvent(QResizeEvent*);

};
