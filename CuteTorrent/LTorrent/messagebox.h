#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QDialog>
#include <QMessageBox>
#include <QAbstractButton>
#include <QMouseEvent>
namespace Ui {
class MessageBox;
}

#define PIXELS_TO_ACT 2
class MyMessageBox : public QDialog
{
    Q_OBJECT
private:

    QPixmap standardIcon(QMessageBox::Icon icon);
    QAbstractButton *clickedButton;
    QMessageBox::StandardButton _clickedButton();
    void setupCustomWindow();
    bool isClosed;
    QMessageBox::StandardButton cancelButton;
    QPoint dragPosition;
    /**
         * @brief moveWidget Specifies if the window is in move action.
         */
    bool moveWidget;
    /**
         * @brief inResizeZone Specifies if the mouse is in resize zone.
         */
    bool inResizeZone;
    /**
         * @brief allowToResize Specifies if the mouse is allowed to resize.
         */
    bool allowToResize;
    /**
         * @brief resizeVerSup Specifies if the resize is in the top of the window.
         */
    bool resizeVerSup;
    /**
         * @brief resizeHorEsq Specifies if the resize is in the left of the window.
         */
    bool resizeHorEsq;
    /**
         * @brief resizeDiagSupEsq Specifies if the resize is in the top left of the window.
         */
    bool resizeDiagSupEsq;
    /**
         * @brief resizeDiagSupDer Specifies if the resize is in the top right of the window.
         */
    bool resizeDiagSupDer;

    /**
         * @brief mouseMoveEvent Overloaded member that moves of resizes depending of the
         * configuration sotred at mousePressEvent().
         * @param e The mouse event.
         */
    void mouseMoveEvent(QMouseEvent *e);
    /**
         * @brief mousePressEvent Overloaded member that stores that changes the cursor and
         * configures the side that is changing.
         * @param e The mouse event.
         */
    void mousePressEvent(QMouseEvent *e);
    /**
         * @brief mouseReleaseEvent Overloaded member that removes the configuration set in mousePressEvent().
         * @param e The mouse event.
         */
    void mouseReleaseEvent(QMouseEvent *e);
    /**
         * @brief mouseDoubleClickEvent Overloadad member that maximize/restore the window if is
         * doubleclicked and the position of the mouse is not the top left of the window (menu zone).
         * @param e The mouse event.
         */
    void mouseDoubleClickEvent(QMouseEvent *e);
    /**
         * @brief paintEvent Overloaded method that allows to customize the styles of the window.
         */


    void moveWindow(QMouseEvent *e);
    void setupWindowIcons();
protected:


public:
    explicit MyMessageBox(QWidget *parent = 0);
    MyMessageBox(QMessageBox::Icon icon, const QString &title, const QString &text,
                QMessageBox::StandardButtons buttons = QMessageBox::NoButton, QWidget *parent = 0,
                Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ~MyMessageBox();

    static QMessageBox::StandardButton critical ( QWidget * parent, const QString & title,
                                     const QString & text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                     QMessageBox::StandardButton defaultButton = QMessageBox::NoButton );
    static QMessageBox::StandardButton information ( QWidget * parent, const QString & title,
                                 const QString & text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                 QMessageBox::StandardButton defaultButton = QMessageBox::NoButton );
    static QMessageBox::StandardButton question ( QWidget * parent, const QString & title,
                                           const QString & text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                           QMessageBox::StandardButton defaultButton = QMessageBox::NoButton );
    static QMessageBox::StandardButton warning ( QWidget * parent, const QString & title, const QString & text,
                             QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton );
    static void about(QWidget *parent, const QString &title, const QString &text);
    static QMessageBox::StandardButton showNewMessageBox(QWidget *parent,
                   QMessageBox::Icon icon, const QString& title, const QString& text,
                   QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton);
private:
    void showDialog();
    Ui::MessageBox *ui;
private slots:
    void buttonClicked(QAbstractButton*);
    void accept();
    void reject();
};

#endif // MYMESSAGEBOX_H
