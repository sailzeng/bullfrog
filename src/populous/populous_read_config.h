#pragma once

#include "populous_protobuf_reflect.h"
#include "populous_qt_excel_engine.h"

class Populous_Read_Config
{
public:


    struct TABLE_CONFIG
    {
        //!�������
        QString excel_table_name_;

        //!������ݴӵڼ��ж�ȡ
        long read_data_start_ = 3;

        //!����Ӧ��protobuf��message����
        QString pb_line_message_;

        //!���ĵڼ��������ֶζ�Ӧ��protobuf
        int pb_fieldname_line_ = 2;
		
		//���protobuf�������ݵĵ��ļ�����
		QString save_pb_config_;
		//!��Ӧ��repeat line message �ṹ�����ƣ�
		QString pb_list_message_;


        //!����ŵ����ݿ⣨SQLite���ļ�����
        QString save_sqlite3_db_;

        //!����Ӧ��table id
        unsigned int table_id_ = 0;
        //!����������ֶ�1���к�
        int index1_column_ = 0;
        //!����������ֶ�2���к�
		int index2_column_ = 0;


        //!Protobuf item���������
        std::vector<QString>  proto_field_ary_;

        //!����ṹ���£�record��һ��repeated ��message��
        //!phonebook.master
        //!phonebook.record.name
        //!phonebook.record.tele_number
        //!phonebook.record.email
        //!phonebook.record.name
        //!phonebook.record.tele_number
        //!phonebook.record.email
        //!��ôphonebook.record.name���ֵ�λ�ûᱻ��ʶΪitem_msg_firstshow_ Ϊ1
        std::vector<int> item_msg_firstshow_;

        //!�����������  �ᱻ��¼Ϊphonebook.record.name
        QString firstshow_field_;
        //!����������� �ᱻ��¼Ϊphonebook.record
        QString firstshow_msg_;

    };

    //!ö��ֵ�Ķ�Ӧ��ϵ��
    typedef std::map <QString, QString >  MAP_QSTRING_TO_QSTRING;

    //!
    typedef std::map <QString, TABLE_CONFIG> MAP_TABLE_TO_CONFIG;

    //!
    struct EXCEL_FILE_DATA
    {
        MAP_QSTRING_TO_QSTRING  xls_enum_;

        MAP_TABLE_TO_CONFIG  xls_table_cfg_;
    };

    typedef std::map <QString, EXCEL_FILE_DATA> MAP_FNAME_TO_CFGDATA;

protected: // �������л�����
    Populous_Read_Config();
protected:
    virtual ~Populous_Read_Config();


public:

    //!
    static Populous_Read_Config *instance();

    //!
    static void clean_instance();

public:

    /*!
    * @brief      ��ʼ����ȡ������׼������Ŀ¼������ת��
	* @return     int 
    * @param[in]  excel_dir ��ȡexcel_dirĿ¼�����е�EXCEL�ļ�
    * @param[in]  proto_dir ����proto_dirĿ¼�µ�meta�ļ����䣬
    * @param[in]  outer_dir ת����λ���ļ������outer_dirĿ¼�����ΪNULL�����ʾ�õ�ǰĿ¼���
    */
    int init_read_all(const QString &excel_dir,
                      const QString &proto_dir,
                      const QString *outer_dir,
					  QString &error_tips);

    //!���е�Ŀ¼����һ��Ŀ¼�µĿ�ݴ���ʽ
	int init_read_all2(const QString &allinone_dir,
					   QString &error_tips);

	/*!
	* @brief      ��ʼ����׼����ȡһ��EXCEL�ļ���ת��Ϊ�����ļ�
	* @return     int
	* @param      excel_file
	* @param      proto_dir ����proto_dirĿ¼�µ�meta�ļ����䣬
	* @param      outer_dir ת����λ���ļ������outer_dirĿ¼
	* @param      table_name ������ΪNULL��ʶ��ȫ�ļ�����ת��
	* @param[out] error_tips ������Ϣ���������
	*/
	int init_read_one(const QString &excel_file,
					  const QString *excel_table_name,
					  const QString &proto_dir,
					  const QString *outer_dir,
					  QString &error_tips);

	

    bool init_print_db3(const QString &db3_file,
                        int db3_table_id,
                        const QString &meta_struct_name,
                        const QString &proto_dir,
						const QString *outer_dir,
						QString &error_tips);

    bool init_print_pbc(const QString &pbc_file,
                        const QString &meta_struct_name,
                        const QString &proto_dir,
                        const QString *outer_dir);

    //
    void finalize();


	int read_excel(QString &error_tips);



    //�������еĶ�ȡ����
    void clear();



    //!��DB3�ļ������ȡĳ�����ñ������
    int read_db3_conftable(const std::string &db3_fname,
                           const std::string &conf_message_name,
                           unsigned int table_id,
                           unsigned int index_1 = 0,
                           unsigned int index_2 = 0);

protected:

    //��ö��ֵ
    int read_table_enum(EXCEL_FILE_DATA &file_cfg_data);

    //!
	/*!
	* @brief      ��ȡsheet [TABLE_CONFIG] ������
	* @return     int ���سɹ���� == 0��ʶ�ɹ�
	* @param      file_cfg_data
	* @param      error_tips
	* @note       
	*/
	int read_table_config(EXCEL_FILE_DATA &file_cfg_data,
						  QString &error_tips);

    /*!
    * @brief      ��ȡ�������
    * @return     int
    * @param      table_cfg   sheet������
	* @param      list_msg    List Message�����ڴ洢Proto�������ļ�
    * @param      aiiijma_ary
    * @param      error_tips
    * @note       
    */
    int read_sheet_db3data(TABLE_CONFIG &table_cfg,
                           ARRARY_OF_AI_IIJIMA_BINARY *aiiijma_ary,
						   QString &error_tips);


	int read_sheet_pbcdata(TABLE_CONFIG &table_cfg,
						   google::protobuf::Message *&list_msg,
						   QString &error_tips);


    
    /*!
    * @brief      �����ݱ��浽SQLite3 DB�ļ�����
    * @return     int
    * @param      table_cfg
    * @param      aiiijma_ary
    * @param      error_tips
    */
    int save_to_sqlitedb(const TABLE_CONFIG &table_cfg,
                         const ARRARY_OF_AI_IIJIMA_BINARY *aiiijma_ary,
						 QString &error_tips);


	/*!
	* @brief      �����ݱ��浽Proto buf config �����ļ�����
	* @return     int
	* @param      table_cfg
	* @param      line_msg
	* @param      error_tips
	* @note       
	*/
	int save_to_protocfg(const TABLE_CONFIG &table_cfg,
						 const google::protobuf::Message *line_msg,
						 QString &error_tips);

	/*!
	* @brief      ��ȡEXCEL��ʼ�����ڲ�ʵ�֣��ԽӼ�����ȡ��ʼ���ӿ�
	* @return     int
	*/
	int init_read(const QString &proto_dir,
				  const QString *outer_dir,
				  QString &error_tips);


	/*!
	* @brief      
	* @return     int
	* @param      outer_dir
	* @param[out] error_tips ������Ϣ���������
	*/
	int init_outdir(const QString *outer_dir,
					QString &error_tips);


	/*!
	* @brief      map proto �ļ���Ŀ¼��ͬʱ�����������е�.proto�ļ�
	* @return     int
	* @param      proto_dir
	* @param[out] error_tips ������Ϣ���������
	*/
	int init_protodir(const QString &proto_dir,
					  QString &error_tips);


	/*!
	* @brief
	* @return     int
	* @param      open_file �򿪵�EXCEL�ļ����ƣ�
	* @param[out] error_tips ������Ϣ���������
	*/
	int read_one_excel(const QString &open_file,
					   const QString *excel_table_name,
					   QString &error_tips);

protected:

    //����ʵ��
    static Populous_Read_Config  *instance_;
	
	//
	static char REPEATED_STRING[];
protected:

	//!EXCEL���ô�ŵ�Ŀ¼
	QDir excel_path_;

	//!proto�ļ���ŵ�·��
	QDir proto_path_;

    //!��־�����Ŀ¼
    QDir out_log_path_;	
	//!PBC�ļ������Ŀ¼·����PBC protobuf config
	QDir out_pbc_path_;
    //!DB3�ļ������Ŀ¼·����
    QDir out_db3_path_;


    //!Excel�Ĵ������,EXCEL�Ĵ�����
    PopulousQtExcelEngine ils_excel_file_;

    //!
    Illusion_Protobuf_Reflect ils_proto_reflect_;

    //!�ļ���Ӧ���������ݣ������ҵĲ�ѯ
    MAP_FNAME_TO_CFGDATA   file_cfg_map_;


	//!EXCEL�ļ��б�
	QFileInfoList excel_fileary_;

	//!proto meta�ļ��б�
	QFileInfoList proto_fileary_;
	
	//!EXCEL��������
	QString excel_table_name_;

};


