#include "qpictureswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "qfullscreenpicdialog.h"
#include "qimagereader.h"
#include "../common.h"

double iButtonSize = 40;

QPicturesWidget::QPicturesWidget(QWidget *parent , bool bViewOnly, bool bDesktop)
    : QWidget{parent}
{
    m_bIsDesktop = bDesktop;
    m_bViewOnly = bViewOnly;

    //TODO: Сделать нормальные размеры
    //iButtonSize = 70;


    QVBoxLayout * mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    // setStyleSheet("QPicturesWidget {"
    //               "border-style: solid;"
    //               "border-width: 1px;"
    //               "border-color: black;"
    //               "border-radius: 7px;"
    //               "}");

    setStyleSheet("border: 1px solid black");


    m_pCurrentPicLabel = new QPicLabel(this);
    m_pCurrentPicLabel->setStyleSheet("QLabel {"
                                 "border-style: solid;"
                                 "border-width: 1px;"
                                 "border-color: black;"
                                 "border-radius: 7px;"
                                 "}");
    m_pCurrentPicLabel->setAlignment(Qt::AlignCenter);
    connect(m_pCurrentPicLabel,SIGNAL(clicked()), this , SLOT(OnPictureClik()));
    mainLayout->addWidget(m_pCurrentPicLabel);

    QHBoxLayout * buttonsLayout = new QHBoxLayout;

    m_pLeftButton = new QPushButton(QIcon(":/icons/arrow_left_icon.png"),"",this);
    m_pLeftButton->setMinimumHeight(iButtonSize);
    m_pLeftButton->setMinimumWidth(iButtonSize);
    connect(m_pLeftButton , SIGNAL(clicked()) , this , SLOT(OnLeftButton()));
    buttonsLayout->addWidget(m_pLeftButton);

    m_pRightButton = new QPushButton(QIcon(":/icons/arrow_right_icon.png"),"",this);
    m_pRightButton->setMinimumHeight(iButtonSize);
    m_pRightButton->setMinimumWidth(iButtonSize);
    connect(m_pRightButton , SIGNAL(clicked()) , this , SLOT(OnRightButton()));
    buttonsLayout->addWidget(m_pRightButton);

    mainLayout->addLayout(buttonsLayout);

    m_pRemoveButton = new QPushButton("Удалить фото");
    if(!bViewOnly)
    {

        m_pRemoveButton->setIcon(QIcon(":/icons/picture_remove_icon.png"));
        m_pRemoveButton->setIconSize(QSize(iButtonSize*0.75 , iButtonSize*0.75));
        mainLayout->addWidget(m_pRemoveButton);
        connect(m_pRemoveButton,SIGNAL(pressed()),this,SLOT(OnRemoveCurrFotoPressedSlot()));
    }

    m_currentPictureIt = m_Pictures.begin();

    UpdatePictures();
}

void QPicturesWidget::OnRemoveCurrFotoPressedSlot()
{
    RemovePicture();
}

void QPicturesWidget::OnLeftButton()
{
    m_currentPictureIt--;
    UpdatePictures();
}

void QPicturesWidget::OnRightButton()
{
    m_currentPictureIt++;
    UpdatePictures();
}

bool QPicturesWidget::AddImage(QImage& img)
{
    qDebug()<<"QPicturesWidget::AddImage";
    m_Pictures.push_back(img);
    m_currentPictureIt = m_Pictures.end()-1;
    UpdatePictures();
    return true;
}

bool QPicturesWidget::AddPicturePath(QString path , bool bUpdatePicture)
{
    qDebug()<<"QPicturesWidget::AddPicturePath";
    QImageReader reader(path);
    QImage CurrentImage;

    reader.setAutoTransform(true);
    if (!reader.canRead()) {
        return false;
    }


    if (!reader.read(&CurrentImage)) {
        return false;
    }

    m_Pictures.push_back(CurrentImage);
    m_currentPictureIt = m_Pictures.end()-1;
    if(bUpdatePicture) UpdatePictures();
    return true;
}

bool QPicturesWidget::RemovePicture()
{
    if(m_Pictures.empty()) return false;
    m_currentPictureIt = m_Pictures.erase(m_currentPictureIt);
    if(((!m_Pictures.empty())&&(m_currentPictureIt == m_Pictures.end()))) m_currentPictureIt--;
    UpdatePictures();
    return (true);
}

void QPicturesWidget::UpdatePictures()
{
    if (m_currentPictureIt==m_Pictures.end())
    {
        m_pCurrentPicLabel->setEnabled(false);
        m_pLeftButton->setEnabled(false);
        m_pRightButton->setEnabled(false);
        m_pRemoveButton->setEnabled(false);
        return;
    }
    else
    {
        m_pCurrentPicLabel->setEnabled(true);
        m_pLeftButton->setEnabled(true);
        m_pRightButton->setEnabled(true);
        m_pRemoveButton->setEnabled(true);
    }



    m_pCurrentPicLabel->SetCurrentImage(&(*m_currentPictureIt));

    if(m_currentPictureIt == m_Pictures.begin()) m_pLeftButton->setEnabled(false);
    else m_pLeftButton->setEnabled(true);

    if(m_currentPictureIt == m_Pictures.end()-1) m_pRightButton->setEnabled(false);
    else m_pRightButton->setEnabled(true);
}

void QPicturesWidget::OnPictureClik()
{
    QFullScreenPicDialog dlg(&(*m_currentPictureIt) , this);

    dlg.exec();
}

