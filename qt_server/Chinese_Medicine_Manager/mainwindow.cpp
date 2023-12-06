#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    myConnect();
    if(!readConfig())
        QMessageBox::warning(this,"警告","读取配置文件失败",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    if(!connectDB())
        QMessageBox::warning(this,"警告","连接数据库失败",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    if(!createModel())
        QMessageBox::warning(this,"警告","查询数据库失败",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    updateButtonState();
}

/*!
 * \brief 用来链接UI上的信号和槽
 */
void MainWindow::myConnect(){
    connect(ui->Quitaction,&QAction::triggered,this,&MainWindow::QuitAction_triggered);
    connect(ui->DBaction,&QAction::triggered,this,&MainWindow::DBAction_triggered);
    connect(ui->ReLoadButton,&QPushButton::clicked,this,&MainWindow::ReLoadButton_clicked);
    connect(ui->RevertButton,&QPushButton::clicked,this,&MainWindow::RevertButton_clicked);
    connect(ui->SubmitButton,&QPushButton::clicked,this,&MainWindow::SubmitButton_clicked);
    connect(ui->AddButton,&QPushButton::clicked,this,&MainWindow::AddButton_clicked);
    connect(ui->DeleteButton,&QPushButton::clicked,this,&MainWindow::DeleteButton_clicked);
    connect(ui->FindDataButton,&QPushButton::clicked,this,&MainWindow::FindDataButton_clicked);
}

/*!
 * \brief 关闭程序
 */
void MainWindow::QuitAction_triggered(){
    QApplication::quit();
}

/*!
 * \brief 读取数据库配置文件
 * \return 成功读取返回true，否则false
 */
bool MainWindow::readConfig(){
    QFile configFile(QDir::currentPath()+"/configure.json");
    if(!configFile.open(QIODevice::ReadOnly))
        return false;
    QJsonParseError parseError;
    QJsonDocument doc(QJsonDocument::fromJson(configFile.readAll(),&parseError));
    if(doc.isNull()||parseError.error!=QJsonParseError::NoError)
        return false;

    QJsonObject configInfo=doc.object();
    QJsonObject dbInfo=configInfo.value("DataBaseInfo").toObject();

    m_databaseInfo->databaseName=dbInfo.value("HostName").toString();
    m_databaseInfo->port=dbInfo.value("Port").toInt();
    m_databaseInfo->databaseName=dbInfo.value("DataBaseName").toString();
    m_databaseInfo->username=dbInfo.value("UserName").toString();
    m_databaseInfo->password=dbInfo.value("Password").toString();
    configFile.close();
    return true;
}

/*!
 * \brief 设置连接数据库的信息
 */
void MainWindow::DBAction_triggered(){
    DataBaseConfig* dialog=new DataBaseConfig();
    dialog->setInfo(m_databaseInfo);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog,&DataBaseConfig::sendConfig,this,&MainWindow::dialog_getConfig);
    dialog->show();
}

/*!
 * \brief 连接数据库
 * \return 连接数据库成功返回true，否则false
 */
bool MainWindow::connectDB(){
    if(!m_db.isOpen()){
        m_db=QSqlDatabase::addDatabase("QMYSQL");
        m_db.setHostName(m_databaseInfo->hostname);
        m_db.setPort(m_databaseInfo->port);
        m_db.setDatabaseName(m_databaseInfo->databaseName);
        m_db.setUserName(m_databaseInfo->username);
        m_db.setPassword(m_databaseInfo->password);
        bool ok=m_db.open();
        if(!ok)
            return false;
        return true;
    }
    return true;
}

/*!
 * \brief 查询表头的名称
 * \return QStringList 查询到的表头的名称
 */
QList<QPair<QString, QString>> MainWindow::getHeaderNames(){
    if(!m_db.isOpen())
        return QList<QPair<QString, QString>>();
    QSqlQuery query("DESCRIBE "+m_currentTable,m_db);
    if(!query.exec())
        return QList<QPair<QString, QString>>();
    m_headerLists.clear();
    while(query.next()){
        QPair<QString,QString> header(query.value(0).toString(),query.value(1).toString());
        m_headerLists.append(header);
    }
    return m_headerLists;
}

/*!
 * \brief 读取sql的数据，制作成Model，并且显示在View上
 * \return 制作成功返回true，否则返回false
 */
bool MainWindow::createModel(){
    QSqlQuery query("show tables",m_db);
    m_isConnected=false;
    if(!query.exec())
        return false;
    while(query.next())
        ui->DBcomboBox->addItem(query.value(0).toString());

    //这里并没有出错，只是没有数据表而已，所以返回true
    if(ui->DBcomboBox->count()==0){
        QMessageBox::information(this,"提示","数据库中没有数据表",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
        return true;
    }

    m_currentTable=ui->DBcomboBox->itemText(0);
    QList<QPair<QString,QString>> headers=getHeaderNames();
    if(headers.length()==0)
        return false;
    m_sqlTableModel=new QSqlTableModel(this,m_db);
    m_sqlTableModel->setTable(m_currentTable);
    m_sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if(!m_sqlTableModel->select()){
        delete m_sqlTableModel;
        m_sqlTableModel=nullptr;
        return false;
    }
    for(const QPair<QString,QString>& header:headers)
        m_sqlTableModel->setHeaderData(m_sqlTableModel->fieldIndex(header.first),Qt::Horizontal,header.first);
    ReadOnlyDelegate* readonlyDelegate=new ReadOnlyDelegate(nullptr);
    ui->tableView->setItemDelegateForColumn(0,readonlyDelegate);
    ui->tableView->setModel(m_sqlTableModel);
    m_isConnected=true;
    return true;
}

/*!
 * \brief 下拉框内容改变后的重新加载table
 * \param currentItem
 */
void MainWindow::ComcoBox_itemChanged(QString currentItem){
    if(currentItem==m_currentTable)
        return;
    if(m_sqlTableModel==nullptr)
        return;
    m_currentTable=currentItem;
    QSqlTableModel* oldmodel=m_sqlTableModel;
    if(!createModel())
        QMessageBox::warning(this,"提示","切换数据表失败",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    delete oldmodel;
}

/*!
 * \brief 重新连接数据库
 */
void MainWindow::ReLoadButton_clicked(){
    if(!connectDB())
        QMessageBox::warning(this,"警告","连接数据库失败",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    if(!createModel())
        QMessageBox::warning(this,"警告","查询数据库失败",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    updateButtonState();
}

/*!
 * \brief 从配置窗口获得用户输入的配置
 */
void MainWindow::dialog_getConfig(DatabaseInfo configInfo){
    m_databaseInfo->hostname=configInfo.hostname;
    m_databaseInfo->port=configInfo.port;
    m_databaseInfo->databaseName=configInfo.databaseName;
    m_databaseInfo->username=configInfo.username;
    m_databaseInfo->password=configInfo.password;
}

/*!
 * \brief 用于添加一行数据
 */
void MainWindow::AddButton_clicked(){
    int row=m_sqlTableModel->rowCount();
    m_sqlTableModel->insertRow(row);
    m_sqlTableModel->setData(m_sqlTableModel->index(row,0),row+1);
}

/*!
 * \brief 用于删除选中的数据
 */
void MainWindow::DeleteButton_clicked(){
    int selectedRow=ui->tableView->currentIndex().row();
    m_sqlTableModel->removeRow(selectedRow);
}

/*!
 * \brief 用于撤销更改
 */
void MainWindow::RevertButton_clicked(){
    QMessageBox::StandardButton result=QMessageBox::warning(this,"警告","是否撤销修改？",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    if(result!=QMessageBox::Ok)
        return;
    m_sqlTableModel->revertAll();
}

/*!
 * \brief 用于提交更改
 */
void MainWindow::SubmitButton_clicked(){
    QMessageBox::StandardButton result=QMessageBox::warning(this,"警告","是否提交修改到数据库？",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    if(result!=QMessageBox::Ok)
        return;
    m_sqlTableModel->submitAll();
}

/*!
 * \brief 弹出查找信息的窗口
 */
void MainWindow::FindDataButton_clicked(){
    FindData* dialog=new FindData(nullptr,m_headerLists,m_currentTable,m_db);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

/*!
 * \brief 更新界面按钮状态
 */
void MainWindow::updateButtonState(){
    if(!m_isConnected){
        ui->AddButton->setDisabled(true);
        ui->DeleteButton->setDisabled(true);
        ui->RevertButton->setDisabled(true);
        ui->SubmitButton->setDisabled(true);
    }
    else{
        ui->AddButton->setEnabled(true);
        ui->DeleteButton->setEnabled(true);
        ui->RevertButton->setEnabled(true);
        ui->SubmitButton->setEnabled(true);
    }
}
