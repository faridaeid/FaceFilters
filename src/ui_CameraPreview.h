/********************************************************************************
** Form generated from reading UI file 'CameraPreview.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERAPREVIEW_H
#define UI_CAMERAPREVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraPreview
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *webcamPreview;
    QVBoxLayout *verticalLayout;
    QPushButton *filterButtonOne;
    QPushButton *filterButtonTwo;
    QPushButton *filterButtonThree;
    QPushButton *filterButtonFour;
    QPushButton *filterButtonFive;
    QPushButton *captureButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CameraPreview)
    {
        if (CameraPreview->objectName().isEmpty())
            CameraPreview->setObjectName(QStringLiteral("CameraPreview"));
        CameraPreview->resize(800, 530);
        centralwidget = new QWidget(CameraPreview);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(9, 9, 781, 471));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        webcamPreview = new QLabel(horizontalLayoutWidget);
        webcamPreview->setObjectName(QStringLiteral("webcamPreview"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(webcamPreview->sizePolicy().hasHeightForWidth());
        webcamPreview->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(webcamPreview);
        
        QSizePolicy sizePolicyButton(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicyButton.setVerticalStretch(1);
//        sizePolicyButton.setHeightForWidth(webcamPreview->sizePolicy().hasHeightForWidth());

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        filterButtonOne = new QPushButton(horizontalLayoutWidget);
        filterButtonOne->setObjectName(QStringLiteral("filterButtonOne"));
        filterButtonOne->setSizePolicy(sizePolicyButton);

        verticalLayout->addWidget(filterButtonOne);

        filterButtonTwo = new QPushButton(horizontalLayoutWidget);
        filterButtonTwo->setObjectName(QStringLiteral("filterButtonTwo"));
        filterButtonTwo->setSizePolicy(sizePolicyButton);

        verticalLayout->addWidget(filterButtonTwo);

        filterButtonThree = new QPushButton(horizontalLayoutWidget);
        filterButtonThree->setObjectName(QStringLiteral("filterButtonThree"));
        filterButtonThree->setSizePolicy(sizePolicyButton);

        verticalLayout->addWidget(filterButtonThree);

        filterButtonFour = new QPushButton(horizontalLayoutWidget);
        filterButtonFour->setObjectName(QStringLiteral("filterButtonFour"));
        filterButtonFour->setSizePolicy(sizePolicyButton);

        verticalLayout->addWidget(filterButtonFour);

        filterButtonFive = new QPushButton(horizontalLayoutWidget);
        filterButtonFive->setObjectName(QStringLiteral("filterButtonFive"));
        filterButtonFive->setSizePolicy(sizePolicyButton);

        verticalLayout->addWidget(filterButtonFive);
        
        captureButton = new QPushButton(horizontalLayoutWidget);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setSizePolicy(sizePolicyButton);
        
        verticalLayout->addWidget(captureButton);


        horizontalLayout->addLayout(verticalLayout);

        CameraPreview->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CameraPreview);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 804, 22));
        CameraPreview->setMenuBar(menubar);
        statusbar = new QStatusBar(CameraPreview);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        CameraPreview->setStatusBar(statusbar);

        retranslateUi(CameraPreview);

        QMetaObject::connectSlotsByName(CameraPreview);
    } // setupUi

    void retranslateUi(QMainWindow *CameraPreview)
    {
        CameraPreview->setWindowTitle(QApplication::translate("CameraPreview", "Welcome to Face Filter App", Q_NULLPTR));
        webcamPreview->setText(QString());
        filterButtonOne->setText(QApplication::translate("CameraPreview", "Filter 1", Q_NULLPTR));
        filterButtonTwo->setText(QApplication::translate("CameraPreview", "Filter 2", Q_NULLPTR));
        filterButtonThree->setText(QApplication::translate("CameraPreview", "Filter 3", Q_NULLPTR));
        filterButtonFour->setText(QApplication::translate("CameraPreview", "Filter 4", Q_NULLPTR));
        filterButtonFive->setText(QApplication::translate("CameraPreview", "Filter 5", Q_NULLPTR));
        captureButton->setText(QApplication::translate("CameraPreview", "", Q_NULLPTR));

    } // retranslateUi

};

namespace Ui {
    class CameraPreview: public Ui_CameraPreview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERAPREVIEW_H
