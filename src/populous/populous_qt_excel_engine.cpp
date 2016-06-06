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
        //����ǰ���ȱ������ݣ�Ȼ��ر�workbook
        close();
    }
    ::OleUninitialize();
}


//��ʼ��EXCEL�ļ���
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
  *@brief ��sXlsFileָ����excel����
  *@return true : �򿪳ɹ�
  *        false: ��ʧ��
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
        /*���ָ����ļ������ڣ�����Ҫ�½�һ��*/
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
        work_books_ = excel_instance_->querySubObject("WorkBooks"); //��ȡ������
        active_book_ = work_books_->querySubObject("Open(QString, QVariant)", xls_file_, QVariant(0)); //��xls��Ӧ�Ĺ�����
    }
    else
    {
        //��ȡ������
        work_books_ = excel_instance_->querySubObject("WorkBooks");     
        //���һ���µĹ�����
        work_books_->dynamicCall("Add");                       
        //�½�һ��xls
        active_book_  = excel_instance_->querySubObject("ActiveWorkBook"); 
    }

    //
    work_sheets_ = active_book_->querySubObject("WorkSheets");
    //�򿪵�һ��sheet
    active_sheet_ = active_book_->querySubObject("WorkSheets(int)", curr_sheet_);

    //�����Ѵ򿪣���ʼ��ȡ��Ӧ����
    QAxObject *usedrange = active_sheet_->querySubObject("UsedRange");//��ȡ��sheet��ʹ�÷�Χ����
    QAxObject *rows = usedrange->querySubObject("Rows");
    QAxObject *columns = usedrange->querySubObject("Columns");

    //��Ϊexcel���Դ��������������ݶ���һ���Ǵ�0,0��ʼ�����Ҫ��ȡ�������±�
    start_row_    = usedrange->property("Row").toInt();    //��һ�е���ʼλ��
    start_column_ = usedrange->property("Column").toInt(); //��һ�е���ʼλ��

    row_count_    = rows->property("Count").toInt();       //��ȡ����
    column_count_ = columns->property("Count").toInt();    //��ȡ����

    is_open_  = true;
    return is_open_;
}

/**
  *@brief Open()�����غ���
  */
bool QtExcelEngine::open(QString xlsFile, UINT nSheet, bool visible)
{
    xls_file_ = xlsFile;
    curr_sheet_ = nSheet;
    is_visible_ = visible;

    return open(curr_sheet_, is_visible_);
}

/**
  *@brief ���������ݣ�������д���ļ�
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
        else     /*������ĵ����½������ģ���ʹ�����ΪCOM�ӿ�*/
        {
            active_book_->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                                      xls_file_, 56, QString(""), QString(""), false, false);

        }

        is_save_already_ = true;
    }
}

/**
  *@brief �ر�ǰ�ȱ������ݣ�Ȼ��رյ�ǰExcel COM���󣬲��ͷ��ڴ�
  */
void QtExcelEngine::close()
{
    //�ر�ǰ�ȱ�������
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
  *@brief ��tableWidget�е����ݱ��浽excel��
  *@param tableWidget : ָ��GUI�е�tablewidgetָ��
  *@return ����ɹ���� true : �ɹ�
  *                  false: ʧ��
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

    //��ȡ��ͷд����һ��
    for (int i = 0; i < tableC; i++)
    {
        if ( tableWidget->horizontalHeaderItem(i) != NULL )
        {
            this->SetCellData(1, i + 1, tableWidget->horizontalHeaderItem(i)->text());
        }
    }

    //д����
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

    //����
    save();

    return true;
}

/**
  *@brief ��ָ����xls�ļ��а����ݵ��뵽tableWidget��
  *@param tableWidget : ִ��Ҫ���뵽��tablewidgetָ��
  *@return ����ɹ���� true : �ɹ�
  *                   false: ʧ��
  */
bool QtExcelEngine::readTableData(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }

    //�Ȱ�table���������
    int tableColumn = tableWidget->columnCount();
    tableWidget->clear();
    for (int n = 0; n < tableColumn; n++)
    {
        tableWidget->removeColumn(0);
    }

    int rowcnt    = start_row_ + row_count_;
    int columncnt = start_column_ + column_count_;

    //��ȡexcel�еĵ�һ��������Ϊ��ͷ
    QStringList headerList;
    for (int n = start_column_; n < columncnt; n++ )
    {
        QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", start_row_, n);
        if ( cell )
        {
            headerList << cell->dynamicCall("Value2()").toString();
        }
    }

    //���´�����ͷ
    tableWidget->setColumnCount(column_count_);
    tableWidget->setHorizontalHeaderLabels(headerList);


    //����������
    for (int i = start_row_ + 1, r = 0; i < rowcnt; i++, r++ )   //��
    {
        tableWidget->insertRow(r); //��������
        for (int j = start_column_, c = 0; j < columncnt; j++, c++ )   //��
        {
            QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", i, j ); //��ȡ��Ԫ��

            //��r�����������������
            if ( cell )
            {
                tableWidget->setItem(r, c, new QTableWidgetItem(cell->dynamicCall("Value2()").toString()));
            }
        }
    }

    return true;
}

/**
  *@brief ��ȡָ����Ԫ�������
  *@param row : ��Ԫ����к�
  *@param column : ��Ԫ����к�
  *@return [row,column]��Ԫ���Ӧ������
  */
QVariant QtExcelEngine::GetCellData(UINT row, UINT column)
{
    QVariant data;

    QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column); //��ȡ��Ԫ�����
    if ( cell )
    {
        data = cell->dynamicCall("Value2()");
    }

    return data;
}

/**
  *@brief �޸�ָ����Ԫ�������
  *@param row : ��Ԫ����к�
  *@param column : ��Ԫ��ָ�����к�
  *@param data : ��Ԫ��Ҫ�޸�Ϊ��������
  *@return �޸��Ƿ�ɹ� true : �ɹ�
  *                   false: ʧ��
  */
bool QtExcelEngine::SetCellData(UINT row, UINT column, QVariant data)
{
    bool op = false;

    QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column); //��ȡ��Ԫ�����
    if ( cell )
    {
        QString strData = data.toString(); //excel ��Ȼֻ�ܲ����ַ��������ͣ��������޷�����
        cell->dynamicCall("SetValue(const QVariant&)", strData); //�޸ĵ�Ԫ�������
        op = true;
    }
    else
    {
        op = false;
    }

    return op;
}

/**
  *@brief ��ճ�����֮�������
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
  *@brief �ж�excel�Ƿ��ѱ���
  *@return true : �Ѵ�
  *        false: δ��
  */
bool QtExcelEngine::IsOpen()
{
    return is_open_;
}

/**
  *@brief �ж�excel COM�����Ƿ���óɹ���excel�Ƿ����
  *@return true : ����
  *        false: ������
  */
bool QtExcelEngine::IsValid()
{
    return is_valid_;
}

/**
  *@brief ��ȡexcel������
  */
UINT QtExcelEngine::GetRowCount()const
{
    return row_count_;
}

/**
  *@brief ��ȡexcel������
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
