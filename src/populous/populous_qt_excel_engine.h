/**
*@brief 这是一个便于Qt读写excel封装的类，同时，便于把excel中的数据
*显示到界面上，或者把界面上的数据写入excel中，同GUI进行交互，关系如下：
*Qt tableWidget <--> ExcelEngine <--> xls file.
*
*@note ExcelEngine类只负责读/写数据，不负责解析，做中间层
*@author yaoboyuan 254200341@qq.com
*@date 2012-4-12
*/

#ifndef EXCELENGINE_H
#define EXCELENGINE_H



typedef unsigned int UINT;



///
///代码风格我打算全部采用Qt的代码风格。
///

class QtExcelEngine : protected QObject
{
public:
    QtExcelEngine();
    QtExcelEngine(QString xlsFile);
    ~QtExcelEngine();

public:

    //打开xls文件
    bool open(UINT nSheet = 1, bool visible = false);
    bool open(QString xlsFile, UINT nSheet = 1, bool visible = false);

    ///
    void insertSheet(const QString &sheetName);

    //保存xls报表
    void save();
    //关闭xls报表
    void close();

    //保存数据到xls
    bool saveTableData(QTableWidget *tableWidget);
    //从xls读取数据到ui
    bool readTableData(QTableWidget *tableWidget);

    //获取指定单元数据
    QVariant GetCellData(UINT row, UINT column);
    //修改指定单元数据
    bool     SetCellData(UINT row, UINT column, QVariant data);

    UINT GetRowCount()const;
    UINT GetColumnCount()const;

    bool IsOpen();
    bool IsValid();

protected:
    void Clear();

private:

    ///指向整个excel应用程序
    QAxObject *excel_instance_ = NULL;

    ///指向工作簿集,excel有很多工作簿，你可以简单理解EXCEL会打开很多个文件
    QAxObject *work_books_ = NULL;

    ///指向sXlsFile对应的工作簿
    QAxObject *active_book_ = NULL;

    ///
    QAxObject *work_sheets_ = NULL;

    //指向工作簿中的某个sheet表单
    QAxObject *active_sheet_ = NULL;

    ///xls文件路径
    QString   xls_file_;

    ///当前打开的第几个sheet
    UINT      curr_sheet_ = 1;
    ///excel是否可见
    bool      is_visible_ = false;
    //行数
    int       row_count_ = 0;
    ///列数
    int       column_count_ = 0;

    ///开始有数据的行下标值
    int       start_row_ = 0;
    ///开始有数据的列下标值
    int       start_column_ = 0;

    ///是否已打开
    bool      is_open_ = false;
    ///是否有效
    bool      is_valid_ = false;
    ///是否是一个新建xls文件，用来区分打开的excel是已存在文件还是有本类新建的
    bool      is_a_newfile_ = false;
    ///防止重复保存
    bool      is_save_already_ = false;

};

#endif // EXCELENGINE_H
