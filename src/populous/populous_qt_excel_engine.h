/**
*@brief ����һ������Qt��дexcel��װ���࣬ͬʱ�����ڰ�excel�е�����
*��ʾ�������ϣ����߰ѽ����ϵ�����д��excel�У�ͬGUI���н�������ϵ���£�
*Qt tableWidget <--> ExcelEngine <--> xls file.
*
*@note ExcelEngine��ֻ�����/д���ݣ���������������м��
*@author yaoboyuan 254200341@qq.com
*@date 2012-4-12
*@note ��ԭ�еĴ�������һЩ�Ķ������ӷ���Qt�ı����ʽ�ȡ�
*@data 2016-6-7 �����ǰϦ
*/

#ifndef EXCELENGINE_H
#define EXCELENGINE_H




///
///�������Ҵ���ȫ������Qt�Ĵ�����
///

class QtExcelEngine : protected QObject
{
public:

    //���캯������������
    QtExcelEngine();
    ~QtExcelEngine();

public:

    ///��ʼ��EXCEL
    bool init_excel(bool visible);

    ///�ͷ��˳�
    void release_excel();

    //
    bool open(const QString &xls_file, int  sheet_index = 1);

    /*!
    * @brief      ����һ��sheet��ͬʱ����������
    * @return     void
    * @param      sheet_name sheet�ı���
    */
    void insert_sheet(const QString &sheet_name);

    //����xls����
    void save();

    //�ر�xls����
    void close();

    //�õ�sheet������
    int sheets_count();

    //�õ�ĳ��sheet������
    bool get_sheet_name(int sheet_index, QString &sheet_name);

    /*!
    * @brief      ������������sheet��
    * @return     bool �����Ƿ�ɹ�����
    * @param      sheet_index sheet��������1��ʼ
    */
    bool load_sheet(int sheet_index);
    
    /*!
    * @brief      ����sheet����������sheet��
    * @return     bool �����Ƿ�ɹ�����
    * @param      sheet_name Ҫ���ص�sheet ������
    */
    bool load_sheet(const QString &sheet_name);

	
	/*!
	* @brief      ����Ƿ���������ֵ�sheet
	* @return     bool �з���true�����򷵻�false
	* @param      sheet_name
	*/
	bool has_sheet(const QString &sheet_name);

    //�������ݵ�xls
    bool write_tabledata(QTableWidget *tableWidget);
    //��xls��ȡ���ݵ�ui
    bool read_tabledata(QTableWidget *tableWidget);

	/*!
    * @brief      ��ȡָ����Ԫ����
    * @return     QVariant  ��Ԫ���Ӧ������
    * @param      row  ��Ԫ����к�
    * @param      column ��Ԫ����к�
    * @note       
    */
    QVariant get_cell(int  row, int  column);

    //�޸�ָ����Ԫ����
    bool  set_cell(int  row, int  column,const QVariant &data);

	///�򿪵�xls�ļ�����
	QString open_filename() const;

	///
    int  row_count()const;
	///
    int  column_count()const;

    bool IsOpen();
    bool IsValid();

protected:
    void Clear();

	///���أ��ڲ��������Ժ���Կ�������һ��Ԥ���أ��ӿ��ȡ�ٶȡ�
	void load_sheet_internal();

public:

	static char *QtExcelEngine::column_name(int column_no);

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
    int       curr_sheet_ = 1;
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
