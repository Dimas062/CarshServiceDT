#ifndef QPICLABEL_H
#define QPICLABEL_H

#include <QLabel>

class QPicLabel : public QLabel
{
    Q_OBJECT
    public:
        explicit QPicLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
        void SetCurrentImage(QImage *);
        void SetWidth(int w);
        void SetHeight(int h);
        ~QPicLabel();
    signals:
        void clicked();
    protected:
        void mousePressEvent(QMouseEvent* event) override;

        void paintEvent(QPaintEvent *event) override;

        QImage * m_pCurrentImage;
};

#endif // QPICLABEL_H
