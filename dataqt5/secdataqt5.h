#ifndef SECDATAQT5_H
#define SECDATAQT5_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPixmap>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <QBuffer>
#include <QByteArray>
#include <QTableWidgetItem>
namespace Ui {
class SecDataqt5;
}

class SecDataqt5 : public QDialog
{
    Q_OBJECT

public:
    explicit SecDataqt5(QWidget *parent = 0);
    ~SecDataqt5();
public:
    void setLabelTitle(const QString& title);

private slots:
    void on_browse_image_clicked();

    void on_display_from_database_clicked();

private:
    Ui::SecDataqt5 *ui;
    QSqlDatabase datab_connection;
};

#endif // SECDATAQT5_H
