#include "populous_predefine.h"
#include "populous_qt_excel_engine.h"


QtExcelEngine::QtExcelEngine()
{
    excel_instance_     = NULL;
    work_books_ = NULL;
    active_book_  = NULL;
    active_sheet_ = NULL;

    xls_file_     = "";
    row_count_    = 0;
    column_count_ = 0;
    start_row_    = 0;
    start_column_ = 0;

    is_open_     = false;
    is_valid_    = false;
    is_a_newfile_ = false;
    is_save_already_ = false;

    HRESULT r = ::OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
}

QtExcelEngine::~QtExcelEngine()
{
    if ( is_open_ )
    {
        //析构前，先保存数据，然后关闭workbook
        close();
    }
    ::OleUninitialize();
}


//初始化EXCEL文件，
bool QtExcelEngine::init_excel(bool visible)
{

    HRESULT r = ::OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
    is_visible_ = visible;
    //
    if (NULL == excel_instance_)
    {
        excel_instance_ = new QAxObject("Excel.Application");
        if (excel_instance_)
        {
            is_valid_ = true;
        }
        else
        {
            is_valid_ = false;
            is_open_ = false;
            return is_open_;
        }

        excel_instance_->dynamicCall("SetVisible(bool)", is_visible_);
    }
    return TRUE;
}


/**
  *@brief 打开sXlsFile指定的excel报表
  *@return true : 打开成功
  *        false: 打开失败
  */
bool QtExcelEngine::open(UINT nSheet)
{

    if ( is_open_ )
    {
        //return bIsOpen;
        close();
    }

    curr_sheet_ = nSheet;
    

    if ( NULL == excel_instance_ )
    {
        excel_instance_ = new QAxObject("Excel.Application");
        if ( excel_instance_ )
        {
            is_valid_ = true;
        }
        else
        {
            is_valid_ = false;
            is_open_  = false;
            return is_open_;
        }

        excel_instance_->dynamicCall("SetVisible(bool)", is_visible_);
    }

    if ( !is_valid_ )
    {
        is_open_  = false;
        return is_open_;
    }

    if ( xls_file_.isEmpty() )
    {
        is_open_  = false;
        is_a_newfile_ = true;
        //return bIsOpen;
    }
    else
    {
        /*如果指向的文件不存在，则需要新建一个*/
        QFile f(xls_file_);
        if (!f.exists())
        {
            is_a_newfile_ = true;
        }
        else
        {
            is_a_newfile_ = false;
        }
    }


    if (!is_a_newfile_)
    {
        work_books_ = excel_instance_->querySubObject("WorkBooks"); //获取工作簿
        active_book_ = work_books_->querySubObject("Open(QString, QVariant)", xls_file_, QVariant(0)); //打开xls对应的工作簿
    }
    else
    {
        //获取工作簿
        work_books_ = excel_instance_->querySubObject("WorkBooks");     
        //添加一个新的工作薄
        work_books_->dynamicCall("Add");                       
        //新建一个xls
        active_book_  = excel_instance_->querySubObject("ActiveWorkBook"); 
    }

    //
    work_sheets_ = active_book_->querySubObject("WorkSheets");
    //打开第一个sheet
    active_sheet_ = active_book_->querySubObject("WorkSheets(int)", curr_sheet_);

    //至此已打开，开始获取相应属性
    QAxObject *usedrange = active_sheet_->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject *rows = usedrange->querySubObject("Rows");
    QAxObject *columns = usedrange->querySubObject("Columns");

    //因为excel可以从任意行列填数据而不一定是从0,0开始，因此要获取首行列下标
    start_row_    = usedrange->property("Row").toInt();    //第一行的起始位置
    start_column_ = usedrange->property("Column").toInt(); //第一列的起始位置

    row_count_    = rows->property("Count").toInt();       //获取行数
    column_count_ = columns->property("Count").toInt();    //获取列数

    is_open_  = true;
    return is_open_;
}

/**
  *@brief Open()的重载函数
  */
bool QtExcelEngine::open(QString xlsFile, UINT nSheet, bool visible)
{
    xls_file_ = xlsFile;
    curr_sheet_ = nSheet;
    is_visible_ = visible;

    return open(curr_sheet_, is_visible_);
}

/**
  *@brief 保存表格数据，把数据写入文件
  */
void QtExcelEngine::save()
{
    if ( active_book_ )
    {
        if (is_save_already_)
        {
            return ;
        }

        if (!is_a_newfile_)
        {
            active_book_->dynamicCall("Save()");
        }
        else     /*如果该文档是新建出来的，则使用另存为COM接口*/
        {
            active_book_->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                                      xls_file_, 56, QString(""), QString(""), false, false);

        }

        is_save_already_ = true;
    }
}

/**
  *@brief 关闭前先保存数据，然后关闭当前Excel COM对象，并释放内存
  */
void QtExcelEngine::close()
{
    //关闭前先保存数据
    save();

    if ( excel_instance_ && active_book_ )
    {
        active_book_->dynamicCall("Close(bool)", true);
        excel_instance_->dynamicCall("Quit()");

        delete excel_instance_;
        excel_instance_ = NULL;

        is_open_     = false;
        is_valid_    = false;
        is_a_newfile_ = false;
        is_save_already_ = true;
    }
}

/**
  *@brief 把tableWidget中的数据保存到excel中
  *@param tableWidget : 指向GUI中的tablewidget指针
  *@return 保存成功与否 true : 成功
  *                  false: 失败
  */
bool QtExcelEngine::saveTableData(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }
    if ( !is_open_ )
    {
        return false;
    }

    int tableR = tableWidget->rowCount();
    int tableC = tableWidget->columnCount();

    //获取表头写做第一行
    for (int i = 0; i < tableC; i++)
    {
        if ( tableWidget->horizontalHeaderItem(i) != NULL )
        {
            this->SetCellData(1, i + 1, tableWidget->horizontalHeaderItem(i)->text());
        }
    }

    //写数据
    for (int i = 0; i < tableR; i++)
    {
        for (int j = 0; j < tableC; j++)
        {
            if ( tableWidget->item(i, j) != NULL )
            {
                this->SetCellData(i + 2, j + 1, tableWidget->item(i, j)->text());
            }
        }
    }

    //保存
    save();

    return true;
}

/**
  *@brief 从指定的xls文件中把数据导入到tableWidget中
  *@param tableWidget : 执行要导入到的tablewidget指针
  *@return 导入成功与否 true : 成功
  *                   false: 失败
  */
bool QtExcelEngine::readTableData(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }

    //先把table的内容清空
    int tableColumn = tableWidget->columnCount();
    tableWidget->clear();
    for (int n = 0; n < tableColumn; n++)
    {
        tableWidget->removeColumn(0);
    }

    int rowcnt    = start_row_ + row_count_;
    int columncnt = start_column_ + column_count_;

    //获取excel中的第一行数据作为表头
    QStringList headerList;
    for (int n = start_column_; n < columncnt; n++ )
    {
        QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", start_row_, n);
        if ( cell )
        {
            headerList << cell->dynamicCall("Value2()").toString();
        }
    }

    //重新创建表头
    tableWidget->setColumnCount(column_count_);
    tableWidget->setHorizontalHeaderLabels(headerList);


    //插入新数据
    for (int i = start_row_ + 1, r = 0; i < rowcnt; i++, r++ )   //行
    {
        tableWidget->insertRow(r); //插入新行
        for (int j = start_column_, c = 0; j < columncnt; j++, c++ )   //列
        {
            QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", i, j ); //获取单元格

            //在r新行中添加子项数据
            if ( cell )
            {
                tableWidget->setItem(r, c, new QTableWidgetItem(cell->dynamicCall("Value2()").toString()));
            }
        }
    }

    return true;
}

/**
  *@brief 获取指定单元格的数据
  *@param row : 单元格的行号
  *@param column : 单元格的列号
  *@return [row,column]单元格对应的数据
  */
QVariant QtExcelEngine::GetCellData(UINT row, UINT column)
{
    QVariant data;

    QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column); //获取单元格对象
    if ( cell )
    {
        data = cell->dynamicCall("Value2()");
    }

    return data;
}

/**
  *@brief 修改指定单元格的数据
  *@param row : 单元格的行号
  *@param column : 单元格指定的列号
  *@param data : 单元格要修改为的新数据
  *@return 修改是否成功 true : 成功
  *                   false: 失败
  */
bool QtExcelEngine::SetCellData(UINT row, UINT column, QVariant data)
{
    bool op = false;

    QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column); //获取单元格对象
    if ( cell )
    {
        QString strData = data.toString(); //excel 居然只能插入字符串和整型，浮点型无法插入
        cell->dynamicCall("SetValue(const QVariant&)", strData); //修改单元格的数据
        op = true;
    }
    else
    {
        op = false;
    }

    return op;
}

/**
  *@brief 清空除报表之外的数据
  */
void QtExcelEngine::Clear()
{
    xls_file_     = "";
    row_count_    = 0;
    column_count_ = 0;
    start_row_    = 0;
    start_column_ = 0;
}

/**
  *@brief 判断excel是否已被打开
  *@return true : 已打开
  *        false: 未打开
  */
bool QtExcelEngine::IsOpen()
{
    return is_open_;
}

/**
  *@brief 判断excel COM对象是否调用成功，excel是否可用
  *@return true : 可用
  *        false: 不可用
  */
bool QtExcelEngine::IsValid()
{
    return is_valid_;
}

/**
  *@brief 获取excel的行数
  */
UINT QtExcelEngine::GetRowCount()const
{
    return row_count_;
}

/**
  *@brief 获取excel的列数
  */
UINT QtExcelEngine::GetColumnCount()const
{
    return column_count_;
}

void QtExcelEngine::insertSheet(const QString &sheetName)
{
    work_sheets_->querySubObject("Add()");
    QAxObject *a = work_sheets_->querySubObject("Item(int)", 1);
    a->setProperty("Name", sheetName);
    active_sheet_ = a;
}
