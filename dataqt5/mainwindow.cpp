#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->username->setPlaceholderText("Enter Name");
    ui->password->setPlaceholderText("Enter password");
    ui->email->setPlaceholderText("Enter email");
    ui->phone->setPlaceholderText("Enter phone");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_registerBtn_clicked()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL", "MyConnect");
       database.setHostName("localhost");
       database.setUserName("root");
       database.setPassword("");
       database.setDatabaseName("qt5");
       if(database.open()) {


              QString username = ui->username->text();
              QString password = ui->password->text();
              QString email = ui->email->text();
              QString phone = ui->phone->text();

              // Run out Inster Query

              QSqlDatabase database = QSqlDatabase::database("MyConnect");
              QSqlQuery query(QString("SELECT email FROM Users WHERE email LIKE '%@gmail.com' ORDER BY username;"), database);
              while (query.next()) {
                  QString emailFROMDB = query.value(0).toString();
                  if(emailFROMDB != email) {
                      query.prepare("INSERT INTO users (username, password, email, phone)"
                                    "VALUES (:username, :password, :email, :phone)");
                      query.bindValue(":username", username);
                      query.bindValue(":password", password);
                      query.bindValue(":email", email);
                      query.bindValue(":phone", phone);
                      if(query.exec()) {
                          QMessageBox::information(this, "Congratulations", "Registration complected!!");
                          query.finish();
                          /*
                          this->close();
                          SecSmoothShare secSmooth;
                          secSmooth.setModal(true);
                          secSmooth.exec();
                          */



                      } else {
       QMessageBox::information(this, "Error", "error while executing query");
       }


                  } else {

                      QMessageBox::information(this,"Registration fail", "This email already taken!");
                      query.finish();
       }
              }







      }
         else {
      QMessageBox::information(this,"Not Connected", "Database not connected");
      }

}




void MainWindow::on_loginBtn_clicked()
{


  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "MyConn");
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("");
        db.setDatabaseName("qt5");


    QString usernamelogin = ui->usernamelogin->text();
       QString passwordlogin= ui->passwordlogin->text();

       if(db.open()) {
               //QMessageBox::information(this, "Database succses", "Database connection succses");

               QSqlQuery query(QSqlDatabase::database("MyConn"));

               query.prepare(QString("SELECT * FROM Users WHERE username= :username AND password = :password"));

               query.bindValue(":username", usernamelogin);
               query.bindValue(":password", passwordlogin);

               if(!query.exec()) {

                   QMessageBox::information(this, "failed", "Querry failet to execute");

               }
               else {
                   while (query.next()) {
                       QString usernameFROMDB = query.value(1).toString();
                       QString passwordFROMDB = query.value(2).toString();

                       if(usernameFROMDB == usernamelogin && passwordFROMDB == passwordlogin) {
                           QMessageBox::information(this, "Succses", "Login succses");
                           this->close();
                           SecDataqt5 secWindow;
                          secWindow.setLabelTitle(usernamelogin);
                           secWindow.setModal(true);
                           secWindow.exec();

                       }
                       else {
                           QMessageBox::information(this, "Failed", "username or password is incorrect");
                       }
                   }
               }

   }
      else {
   QMessageBox::information(this,"Not Connected", "Database not connected");
   }
}
