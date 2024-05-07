#include "secdataqt5.h"
#include "ui_secdataqt5.h"
#include <mainwindow.h>

SecDataqt5::SecDataqt5(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDataqt5)
{
    ui->setupUi(this);

    datab_connection = QSqlDatabase::addDatabase("QMYSQL", "Connect");
    datab_connection.setDatabaseName("images_store");
    datab_connection.setHostName("localhost");
    datab_connection.setUserName("root");
    datab_connection.setPassword("");
    if(datab_connection.open()){

    } else{
         QMessageBox::information(this, "failed", "Connection failed to execute");
    }

}

SecDataqt5::~SecDataqt5()
{
    delete ui;

}

void SecDataqt5::setLabelTitle(const QString& title)
{
    ui->loggedusername->setText(title);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "MyConnection");
          db.setHostName("localhost");
          db.setUserName("root");
          db.setPassword("");
          db.setDatabaseName("qt5");

          if(db.open()){


              QSqlQuery query(QSqlDatabase::database("MyConnection"));

              query.prepare(QString("SELECT * FROM Users WHERE username= :username"));

              query.bindValue(":username", title);

              if(!query.exec()) {

                  QMessageBox::information(this, "failed", "Querry failet to execute");

              }
              else {
                  while (query.next()) {
                      QString usernameFROMDB = query.value(1).toString();
                      QString idFromDB = query.value(0).toString();

                      if(usernameFROMDB == title) {

                          ui->loggedID->setText(idFromDB);

                      }
                      else {
                          QMessageBox::information(this, "Failed", "username or password is incorrect");
                      }
                  }
              }
          }

}


void SecDataqt5::on_browse_image_clicked()
{
    QString ImagePath =  QFileDialog::getOpenFileName(this, tr("Select Image"),QCoreApplication::applicationFilePath(),tr("JPG Files (*.jpg)"));
    QPixmap Image(ImagePath);
    QBuffer ImageBufferData;

    if(ImageBufferData.open(QIODevice::ReadWrite)){
        Image.save(&ImageBufferData,"JPG");
    }

    QByteArray FinalDataToSave = ImageBufferData.buffer().toBase64();

    QFileInfo FileInfo(ImagePath);
    QString ImageName = FileInfo.fileName();

    QString uid = ui->loggedID->text();
    QSqlDatabase::database().transaction();
    datab_connection.open();
    QSqlQuery QuerySaveImage(datab_connection);
    QuerySaveImage.prepare("INSERT INTO Images(Image_Name,Image_Data,User_ID)VALUES(:Image_Name,:Image_Data,:User_ID)");
    QuerySaveImage.bindValue(":Image_Name", ImageName);
    QuerySaveImage.bindValue(":Image_Data", FinalDataToSave);
    QuerySaveImage.bindValue(":User_ID",uid);
    QuerySaveImage.exec();
    datab_connection.close();
    QSqlDatabase::database().commit();

    ui->lbl_image_Browse->setPixmap(Image);


}

void SecDataqt5::on_display_from_database_clicked()
{
    QByteArray ImageDataFromDatabase;
    QPixmap Image;
    QString ImageName;

    QString userid = ui->loggedID->text();

    datab_connection.open();
    QSqlQuery QueryGetImage(datab_connection);
     //WHERE User_ID = :User_ID
    QueryGetImage.prepare("SELECT * FROM Images WHERE User_ID = :User_ID ORDER BY id ");



     QueryGetImage.bindValue(":User_ID", userid);


    //Query For Get Last Saved Image From Database

     if(!QueryGetImage.exec()){
          QMessageBox::information(this, "Fail", "Dar ikeltu nuotrauku nera, prasome pirma ikelti");
     }
    else{


        ui->tableWidget_Image_From_Database->setRowCount(QueryGetImage.size());
        int Row_Number = 0;
        while (QueryGetImage.next()){
            ImageName = QueryGetImage.value("Image_Name").toString();
            ImageDataFromDatabase = QByteArray::fromBase64(QueryGetImage.value("Image_Data").toByteArray());
            if(ImageDataFromDatabase == NULL){
                QMessageBox::information(this, "Fail", "Dar ikeltu nuotrauku nera, prasome pirma ikelti");
            }
            Image.loadFromData(ImageDataFromDatabase, "JPG");

            ui->tableWidget_Image_From_Database->setItem(Row_Number,0,new QTableWidgetItem(ImageName));

            QTableWidgetItem *Image_Item = new QTableWidgetItem();

            Image_Item->setData(Qt::DecorationRole, Image.scaled(100,100));
             ui->tableWidget_Image_From_Database->setItem(Row_Number,1,Image_Item);
             ui->tableWidget_Image_From_Database->verticalHeader()->setDefaultSectionSize(30);

            Row_Number = Row_Number + 1;
        }
    }



    datab_connection.close();
    ui->tableWidget_Image_From_Database->resizeRowsToContents();

    Image.save(QCoreApplication::applicationDirPath() + "/Image_From_Database.jpg");



}
