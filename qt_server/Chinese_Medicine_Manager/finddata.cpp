#include "finddata.h"
#include "ui_finddata.h"

FindData::FindData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindData)
{
}

FindData::FindData(QWidget *parent,QList<QPair<QString, QString>> headers, QString currentTable,QSqlDatabase db)
    :QDialog(parent)
    , ui(new Ui::FindData){
    ui->setupUi(this);
    m_headerLists=headers;
    m_currentTable=currentTable;
    m_db=db;
    if(!LoadHeaderComboBox())
        QMessageBox::warning(this,"警告","加载类别出错",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    myConnect();
}

FindData::~FindData()
{
    delete ui;
}

/*!
 * \brief 连接槽
 */
void FindData::myConnect(){
    connect(ui->QuitButton,&QPushButton::clicked,this,[]{QApplication::quit();});
    connect(ui->FindButton,&QPushButton::clicked,this,&FindData::FindButton_clicked);
}

/*!
 * \brief 为ComboBox添加条目
 * \return 成功添加条目返回true，否则false
 */
bool FindData::LoadHeaderComboBox(){
    if(m_headerLists.size()==0)
        return false;
    for(const QPair<QString,QString>& header:m_headerLists){
        ui->CategoriesBox->addItem(header.first);
    }
    return true;
}

void FindData::FindButton_clicked(){
    QSqlQuery query(m_db);
    QString sqlCommand;
    sqlCommand.append("SELECT * FROM ");
    sqlCommand.append(m_currentTable);
    sqlCommand.append(" WHERE ");
    sqlCommand.append(ui->CategoriesBox->currentText());
    query.prepare(sqlCommand+"= ? ");
    query.addBindValue(ui->FindInfoEdit->text());
    if(!query.exec())
        //qDebug()<<Q_FUNC_INFO << query.lastError().text() << query.lastQuery();
        QMessageBox::warning(this,"警告","查询出错",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    if(query.size()==0)
        QMessageBox::information(this,"提示","没有数据",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    QSqlRecord rec=query.record();
    int column=rec.count();
    QString result;
    while(query.next()){
        for(int i=0;i<column;i++){
            result.append(m_headerLists.at(i).first);
            result.append("：");
            result.append(query.value(i).toString());
            result.append("\n");
        }
        result.append("----------------\n");
    }
    ui->ResultEdit->setText(result);
}
