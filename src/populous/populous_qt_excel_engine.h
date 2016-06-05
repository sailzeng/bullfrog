/**
*@brief ����һ������Qt��дexcel��װ���࣬ͬʱ�����ڰ�excel�е�����
*��ʾ�������ϣ����߰ѽ����ϵ�����д��excel�У�ͬGUI���н�������ϵ���£�
*Qt tableWidget <--> ExcelEngine <--> xls file.
*
*@note ExcelEngine��ֻ�����/д���ݣ���������������м��
*@author yaoboyuan 254200341@qq.com
*@date 2012-4-12
*/

#ifndef EXCELENGINE_H
#define EXCELENGINE_H



typedef unsigned int UINT;



///
///�������Ҵ���ȫ������Qt�Ĵ�����
///

class QtExcelEngine : protected QObject
{
public:
    QtExcelEngine();
    QtExcelEngine(QString xlsFile);
    ~QtExcelEngine();

public:

    //��xls�ļ�
    bool open(UINT nSheet = 1, bool visible = false);
    bool open(QString xlsFile, UINT nSheet = 1, bool visible = false);

    ///
    void insertSheet(const QString &sheetName);

    //����xls����
    void save();
    //�ر�xls����
    void close();

    //�������ݵ�xls
    bool saveTableData(QTableWidget *tableWidget);
    //��xls��ȡ���ݵ�ui
    bool readTableData(QTableWidget *tableWidget);

    //��ȡָ����Ԫ����
    QVariant GetCellData(UINT row, UINT column);
    //�޸�ָ����Ԫ����
    bool     SetCellData(UINT row, UINT column, QVariant data);

    UINT GetRowCount()const;
    UINT GetColumnCount()const;

    bool IsOpen();
    bool IsValid();

protected:
    void Clear();

private:

    ///ָ������excelӦ�ó���
    QAxObject *excel_instance_ = NULL;

    ///ָ��������,excel�кܶ๤����������Լ����EXCEL��򿪺ܶ���ļ�
    QAxObject *work_books_ = NULL;

    ///ָ��sXlsFile��Ӧ�Ĺ�����
    QAxObject *active_book_ = NULL;

    ///
    QAxObject *work_sheets_ = NULL;

    //ָ�������е�ĳ��sheet��
    QAxObject *active_sheet_ = NULL;

    ///xls�ļ�·��
    QString   xls_file_;

    ///��ǰ�򿪵ĵڼ���sheet
    UINT      curr_sheet_ = 1;
    ///excel�Ƿ�ɼ�
    bool      is_visible_ = false;
    //����
    int       row_count_ = 0;
    ///����
    int       column_count_ = 0;

    ///��ʼ�����ݵ����±�ֵ
    int       start_row_ = 0;
    ///��ʼ�����ݵ����±�ֵ
    int       start_column_ = 0;

    ///�Ƿ��Ѵ�
    bool      is_open_ = false;
    ///�Ƿ���Ч
    bool      is_valid_ = false;
    ///�Ƿ���һ���½�xls�ļ����������ִ򿪵�excel���Ѵ����ļ������б����½���
    bool      is_a_newfile_ = false;
    ///��ֹ�ظ�����
    bool      is_save_already_ = false;

};

#endif // EXCELENGINE_H
