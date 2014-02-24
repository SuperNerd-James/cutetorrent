#include "messagebox.h"
#include "ui_messagebox.h"
#include <QPointer>
#include <QDebug>
#include "StyleEngene.h"
MyMessageBox::MyMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageBox)
{
    ui->setupUi(this);
}

MyMessageBox::MyMessageBox(QMessageBox::Icon icon, const QString &title, const QString &text,
                       QMessageBox::StandardButtons buttons, QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent),
    ui(new Ui::MessageBox)
{
    ui->setupUi(this);
    setupCustomWindow();
    setupWindowIcons();
    if (icon != QMessageBox::NoIcon)
    {
        ui->icon->setPixmap(standardIcon(icon));
    }
    else
    {

        ui->icon->setPixmap(*(ui->tbMenu->pixmap()));
    }
    ui->LTitle->setText(title);
    ui->text->setText(text);
    if (buttons == QMessageBox::NoButton) {
        buttons = QMessageBox::Ok | QMessageBox::Cancel;
    }
    clickedButton = NULL;
    uint mask = QMessageBox::FirstButton;
    while (mask <= QMessageBox::LastButton) {
        uint sb = buttons & mask;
        mask <<= 1;
        if (!sb)
            continue;
        QPushButton *button = ui->buttonBox->addButton((QDialogButtonBox::StandardButton)sb);
        QMessageBox::ButtonRole  role =  (QMessageBox::ButtonRole)ui->buttonBox->buttonRole(button);

        if ( role == QMessageBox::RejectRole || role == QMessageBox::NoRole ) {
            cancelButton = (QMessageBox::StandardButton)ui->buttonBox->standardButton(button);

        }
    }

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonClicked(QAbstractButton*)));
}
void MyMessageBox::setupWindowIcons(){
    StyleEngene* style = StyleEngene::getInstance();
    ui->pbClose->setIcon(style->getIcon("app_close"));
}

QPixmap MyMessageBox::standardIcon(QMessageBox::Icon icon)
{
    QStyle *style =  QApplication::style();
    int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize);
    QIcon tmpIcon;
    switch (icon) {
    case QMessageBox::Information:
        tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation );
        break;
    case QMessageBox::Warning:
        tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning);
        break;
    case QMessageBox::Critical:
        tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical);
        break;
    case QMessageBox::Question:
        tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion);
    default:
        break;
    }
    if (!tmpIcon.isNull())
        return tmpIcon.pixmap(iconSize, iconSize);
    return QPixmap();
}

QMessageBox::StandardButton MyMessageBox::_clickedButton()
{
    return  clickedButton != NULL ? (QMessageBox::StandardButton)ui->buttonBox->standardButton(clickedButton) : cancelButton;
}

void MyMessageBox::setupCustomWindow()
{
    //setWindowFlags(Qt::CustomizeWindowHint);
    this->setWindowFlags(Qt::Window | Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    setMouseTracking(true);
    ui->titleBar->setMouseTracking(true);
    ui->LTitle->setMouseTracking(true);
    ui->pbClose->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);

    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(reject()));


    moveWidget = false;
    inResizeZone = false;
    allowToResize = false;
    resizeVerSup = false;
    resizeHorEsq = false;
    resizeDiagSupEsq = false;
    resizeDiagSupDer = false;
}
MyMessageBox::~MyMessageBox()
{
    delete ui;
}

QMessageBox::StandardButton MyMessageBox::critical(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return showNewMessageBox(parent,QMessageBox::Critical,title,text,buttons,defaultButton);
}

QMessageBox::StandardButton MyMessageBox::information(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return showNewMessageBox(parent,QMessageBox::Information,title,text,buttons,defaultButton);
}

QMessageBox::StandardButton MyMessageBox::question(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return showNewMessageBox(parent,QMessageBox::Question,title,text,buttons,defaultButton);
}

QMessageBox::StandardButton MyMessageBox::warning(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return showNewMessageBox(parent,QMessageBox::Warning,title,text,buttons,defaultButton);
}

void MyMessageBox::about(QWidget *parent, const QString &title, const QString &text)
{
   showNewMessageBox(parent,QMessageBox::NoIcon,title,text,QMessageBox::Ok,QMessageBox::Ok);
}


void MyMessageBox::moveWindow(QMouseEvent *e)
{

        if (e->buttons() & Qt::LeftButton)
        {
            move(e->globalPos() - dragPosition);
            e->accept();
        }

}

void MyMessageBox::mouseMoveEvent(QMouseEvent *e)
{
    if (moveWidget)
    {
        inResizeZone = false;
        moveWindow(e);
    }

    e->accept();
}

void MyMessageBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (e->pos().x() >= PIXELS_TO_ACT&&e->pos().x() < ui->titleBar->geometry().width()
                 &&e->pos().y() >= PIXELS_TO_ACT&&e->pos().y() < ui->titleBar->geometry().height())
        {
            moveWidget = true;
            dragPosition = e->globalPos() - frameGeometry().topLeft();
    /*        if (isMaximized())
            {
                QRect desktopRect = QApplication::desktop()->availableGeometry();

                dragPosition.setX((desktopRect.width()-normalGeometry().width())/2);
            }*/
        }
    }

    e->accept();
}

void MyMessageBox::mouseReleaseEvent(QMouseEvent *e)
{
    moveWidget = false;
    allowToResize = false;
    resizeVerSup = false;
    resizeHorEsq = false;
    resizeDiagSupEsq = false;
    resizeDiagSupDer = false;

    e->accept();
}

void MyMessageBox::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->pos().x() < ui->tbMenu->geometry().right()&&e->pos().y() < ui->tbMenu->geometry().bottom()
            &&e->pos().x() >=  ui->tbMenu->geometry().x()&&e->pos().y() >= ui->tbMenu->geometry().y()
            && ui->tbMenu->isVisible())
        done(QDialog::Rejected);
    e->accept();
}

QMessageBox::StandardButton MyMessageBox::showNewMessageBox(QWidget *parent, QMessageBox::Icon icon,
                                                          const QString &title, const QString &text,
                                                          QMessageBox::StandardButtons buttons,
                                                          QMessageBox::StandardButton defaultButton)
{
    MyMessageBox msgBox  =  MyMessageBox(icon,title,text,buttons);
    msgBox.showDialog();
    return msgBox._clickedButton();
}

void MyMessageBox::showDialog()
{
    isClosed = false;
    show();
    while(!isClosed)
    {
        QApplication::processEvents(QEventLoop::AllEvents,1000);
    }
}

void MyMessageBox::buttonClicked(QAbstractButton *button)
{
    clickedButton = button;
}

void MyMessageBox::accept()
{
    isClosed = true;
    done(QDialog::Accepted);
}

void MyMessageBox::reject()
{
    isClosed = true;
    done(QDialog::Rejected);
}
