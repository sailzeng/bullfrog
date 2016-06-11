#include "populous_predefine.h"
#include "populous_read_config.h"


//����ĵ���ʵ��
Populous_Read_Config *Populous_Read_Config::instance_ = NULL;

//
Populous_Read_Config::Populous_Read_Config()
{
}


Populous_Read_Config::~Populous_Read_Config()
{
}


Populous_Read_Config *Populous_Read_Config::instance()
{
    if (instance_ == NULL)
    {
        instance_ = new Populous_Read_Config();
    }
    return instance_;
}

void Populous_Read_Config::clean_instance()
{
    if (instance_)
    {
        delete instance_;
        instance_ = NULL;
    }
}

//!���е�Ŀ¼����һ��Ŀ¼�µĿ�ݴ���ʽ
int Populous_Read_Config::init_read_all2(const QString &allinone_dir,
										 QString &error_tips)
{
	QString outer_dir = allinone_dir + "/outer";
	return init_read_all(allinone_dir + "/excel",
						 allinone_dir + "/proto",
						 &outer_dir,
						 error_tips);
}

//��ȡexcel_dirĿ¼�����е�EXCEL�ļ�������proto_dirĿ¼�µ�meta�ļ������䣬ת����λ���ļ������outer_dirĿ¼
int Populous_Read_Config::init_read_all(const QString &excel_dir,
									    const QString &proto_dir,
										const QString *outer_dir,
										QString &error_tips)
{
	int ret = 0;
	excel_path_.setPath(excel_dir);
	if (false == excel_path_.exists())
	{
		error_tips = QString::fromLocal8Bit("Ŀ¼[%1]�������ڣ����������").
			arg(excel_dir);
		return -1;
	}
	//��ȡ.xls , .xlsx �ļ�
	QStringList filters;
	filters << "*.xls" << "*.xlsx";
	excel_fileary_ = excel_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
	if (excel_fileary_.size() <= 0)
	{
		error_tips = QString::fromLocal8Bit("Ŀ¼[%1]��û���κ�Excel�ļ������������").
			arg(excel_dir);
		return -1;
	}

	ret = init_read(proto_dir,outer_dir,error_tips);
	if (ret != 0)
	{
		return -1;
	}
    return 0;
}

//��ʼ����׼����ȡһ��EXCEL�ļ���ת��Ϊ�����ļ�
int Populous_Read_Config::init_read_one(const QString &excel_file,
										const QString *excel_table_name,
										const QString &proto_dir,
										const QString *outer_dir,
										QString &error_tips)
{
	int ret = 0;

	QFileInfo file_info(excel_file);
	if (!file_info.exists())
	{
		error_tips = QString::fromLocal8Bit("EXCEL�ļ�[%1]�������ڣ����顣").
			arg(excel_file);
		return -1;
	}
	excel_fileary_ << file_info;
	excel_table_name_.clear();
	if (excel_table_name)
	{
		excel_table_name_ = *excel_table_name;
	}

	ret = init_read(proto_dir, outer_dir,error_tips);
	if (ret != 0)
	{
		return -1;
	}



	return 0;
}

//��ȡEXCEL�����������
int Populous_Read_Config::init_read(const QString &proto_dir, 
									const QString *outer_dir,
									QString &error_tips)
{
	int ret = 0;
	//
	ret = init_protodir(proto_dir, error_tips);
	if (ret != 0)
	{
		return -1;
	}

	ret = init_outdir(outer_dir, error_tips);
	if (ret != 0)
	{
		return -1;
	}

	bool bret = ils_excel_file_.initialize(false);
	if (false == bret)
	{
		error_tips = QString::fromLocal8Bit("OLE��������EXCEL��ʵ��OLE��ȡEXCEL���밲װ��EXCEL��");
		return -1;
	}


	return 0;
}

//��ʼ��.proto�ļ�Ŀ¼����ȡ�������е�proto�ļ�
int Populous_Read_Config::init_protodir(const QString &proto_dir,
										QString &error_tips)
{
	int ret = 0;
	proto_path_.setPath(proto_dir);;
	if (false == proto_path_.exists())
	{
		error_tips = QString::fromLocal8Bit("Ŀ¼[%1]�������ڣ����������").
			arg(proto_dir);
		return -1;
	}
	//��ȡ.proto �ļ�
	QStringList filters;
	filters << "*.proto";
	proto_fileary_ = proto_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
	if (proto_fileary_.size() <= 0)
	{
		error_tips = QString::fromLocal8Bit("Ŀ¼[%1]��û���κ�protobuf meta(.proto)�ļ������������").
			arg(proto_dir);
		return -1;
	}
	ils_proto_reflect_.map_path(proto_path_.path().toStdString());

	//�������е�.proto �ļ�
	for (int i = 0; i < proto_fileary_.size(); ++i)
	{
		ret = ils_proto_reflect_.import_file(proto_fileary_[i].fileName().toStdString());
		if (ret != 0)
		{
			return -1;
		}
	}
	return 0;
}

int Populous_Read_Config::init_outdir(const QString *outer_dir,
									  QString &error_tips)
{
	QString path_str;
	if (outer_dir)
	{
		path_str = *outer_dir;
	}
	else
	{
		path_str = ".";
	}

	path_str += "/log";
	out_log_path_.setPath(path_str);
	if (false == out_log_path_.exists())
	{
		if (false == out_log_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	//db3��·��û�п��Դ���
	path_str += "/db3";
	out_db3_path_.setPath(path_str);
	if (false == out_db3_path_.exists())
	{
		if (false == out_db3_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	//pbc��·��û�п��Դ���
	path_str += "/pbc";
	out_pbc_path_.setPath(path_str);
	if (false == out_pbc_path_.exists())
	{
		if (false == out_pbc_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	return 0;
}

//
void Populous_Read_Config::finalize()
{
    clear();

    if (ils_excel_file_.is_open())
    {
        ils_excel_file_.close();
        ils_excel_file_.finalize();
    }

    return;
}

//�������еĶ�ȡ����
void Populous_Read_Config::clear()
{
    file_cfg_map_.clear();
}

//��ɨ����߲�����EXCEL�ļ�������һ�ζ�ȡ
int Populous_Read_Config::read_excel(QString &error_tips)
{
	//
	int ret = 0;
	if (!excel_table_name_.isEmpty() && excel_fileary_.size()== 1)
	{
		return read_one_excel(excel_fileary_[0].filePath(),
							  &excel_table_name_,
							  error_tips);
	}
	else
	{
		//����д���ˣ�
		Q_ASSERT(excel_table_name_.isEmpty());
		for (int i = 0; i < excel_fileary_.size(); ++i)
		{
			ret = read_one_excel(excel_fileary_[i].filePath(),
								 NULL,
								 error_tips);
			if (0 != ret)
			{
				return ret;
			}
		}
	}
	return 0;
}

//��ȡһ��EXCEL�ļ�������ƶ��˱��ֻ��ȡ�ض����
int Populous_Read_Config::read_one_excel(const QString &open_file,
									     const QString *excel_table_name,
									     QString &error_tips)
{
    clear();

    BOOL bret = ils_excel_file_.open(open_file);
    //Excel�ļ���ʧ��
    if (bret != TRUE)
    {
        return -1;
    }
    //
    qDebug() << "Dream excecl file have sheet num["
             << ils_excel_file_.sheets_count()
             << "].\n";

    //������
    if (ils_excel_file_.has_sheet("TABLE_CONFIG") == false ||
        ils_excel_file_.has_sheet("ENUM_CONFIG") == false)
    {
        //
        error_tips = QString::fromLocal8Bit("��ѡ�������EXCEL�����ܶ�ȡ�����ñ�[TABLE_CONFIG]"
											" or [ENUM_CONFIG]"
											"�������ּ���򿪡�!");
        return -1;
    }

    //file_cfg_map_[open_file] = excel_data;
    EXCEL_FILE_DATA excel_data;
    auto result = file_cfg_map_.insert(std::make_pair(open_file, excel_data));
    if (!result.second)
    {
        return -1;
    }

    //
    EXCEL_FILE_DATA &xls_data = (*result.first).second;
    int ret = read_table_enum(xls_data);
    if (0 != ret)
    {
        error_tips = QString::fromLocal8Bit("��ѡ�������EXCEL�ļ��е�[ENUM_CONFIG]����ȷ�������ּ���򿪡�!");
        return ret;
    }

    //
    ret = read_table_config(xls_data);
    if (0 != ret)
    {
        error_tips = QString::fromLocal8Bit("��ѡ�������EXCEL�ļ��е�TABLE_CONFIG����ȷ�������ּ���򿪡�!");
        return ret;
    }

	if (excel_table_name && ils_excel_file_.has_sheet(*excel_table_name) == false)
	{
		error_tips = QString::fromLocal8Bit("û��һ�ű�񱻶�ȡ��!�����õĶ�ȡ���[%1]Ӧ�ò�����").
			arg(*excel_table_name);
		return -1;
	}

    //!
    ARRARY_OF_AI_IIJIMA_BINARY  fandaoai_ary;

	bool already_read = false;
    auto iter_tmp = xls_data.xls_table_cfg_.begin();
    for (; iter_tmp != xls_data.xls_table_cfg_.end(); ++iter_tmp)
    {
		//�����ʶ��table name��ֻ��ȡ���table
		if (excel_table_name && *excel_table_name != iter_tmp->second.excel_table_name_)
		{
			continue;
		}
		already_read = true;
        ret = read_table_cfgdata(iter_tmp->second, &fandaoai_ary);
        if (0 != ret)
        {
            return ret;
        }

        ret = save_to_sqlitedb(iter_tmp->second, &fandaoai_ary);
        if (0 != ret)
        {
            return ret;
        }
    }
	//���û�ж�ȡ,������ǰ�����˱���Ƿ����
	if (!already_read)
	{
		error_tips = QString::fromLocal8Bit("û��һ�ű�񱻶�ȡ��!ԭ�������ҵ�ȷ������");
		return -1;
	}
    return 0;
}


//��ȡ���е�ö��ֵ
int Populous_Read_Config::read_table_enum(EXCEL_FILE_DATA &file_cfg_data)
{
    //ǰ�������
    BOOL bret =  ils_excel_file_.load_sheet("ENUM_CONFIG");
    if (bret == FALSE)
    {
        return -1;
    }

    //��Ӧ����
    int row_count = ils_excel_file_.row_count();
    int col_count = ils_excel_file_.column_count();
    qDebug() << "ENUM_CONFIG table have col_count = "
             << col_count
             << " row_count ="
             << row_count
             << "\n";

    //ע�����е��±궼�Ǵ�1��ʼ��
    const long COL_ENUM_KEY = 1;
    const long COL_ENUM_VALUE = 2;
    const QChar ENUM_FIRST_CHAR = '[';

    size_t read_enum = 0;
    //��ȡ���е���
    for (long i = 1; i <= row_count; ++i)
    {

        long   row_no = i;
        //�ַ���
        QString enum_key = ils_excel_file_.get_cell(row_no, COL_ENUM_KEY).toString();

        //�����һ���ַ���[
        if (enum_key[0] == ENUM_FIRST_CHAR )
        {
            QString enum_vlaue = ils_excel_file_.get_cell(row_no, COL_ENUM_VALUE).toString();
            file_cfg_data.xls_enum_[enum_key] = enum_vlaue;

            ++read_enum;
        }
    }
    return 0;
}


//��ȡ�������
int Populous_Read_Config::read_table_config(EXCEL_FILE_DATA &file_cfg_data)
{
    //ǰ�������
    bool bret = ils_excel_file_.load_sheet("TABLE_CONFIG");
    if (bret == false)
    {
        return -1;
    }

    long row_count = ils_excel_file_.row_count();
    long col_count = ils_excel_file_.column_count();
    qDebug() << "TABLE_CONFIG table have col_count = " << col_count << " row_count =" << row_count << "\n";

    //ע�����е��±궼�Ǵ�1��ʼ��
    const long COL_TC_KEY = 1;
    const long COL_TC_VALUE = 2;

    for (long row_no = 1; row_no <= row_count; ++row_no)
    {

        QString tc_key = ils_excel_file_.get_cell(row_no, COL_TC_KEY).toString();

        QString temp_value;
        TABLE_CONFIG tc_data;

        if (tc_key == QString::fromLocal8Bit("�������"))
        {

            tc_data.excel_table_name_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
            if (tc_data.excel_table_name_.isEmpty())
            {
                continue;
            }

            ++row_no;
            if (row_no > row_count)
            {
                return -1;
            }
            //table id
            tc_data.table_id_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            if (tc_data.table_id_ <= 0)
            {
                return -1;
            }
            ++row_no;
            if (row_no > row_count)
            {
                return -1;
            }
            tc_data.read_data_start_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            if (tc_data.read_data_start_ <= 0)
            {
                return -1;
            }

            ++row_no;
            if (row_no > row_count)
            {
                return -1;
            }

            tc_data.pb_msg_name_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
            if (tc_data.pb_msg_name_.isEmpty())
            {
                return -1;
            }

            ++row_no;
            if (row_no > row_count)
            {
                return -1;
            }
            tc_data.pb_fieldname_line_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            if (tc_data.pb_fieldname_line_ <= 0)
            {
                return -1;
            }

            ++row_no;
            if (row_no > row_count)
            {
                return -1;
            }
            tc_data.sqlite3_db_name_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
            if (tc_data.sqlite3_db_name_.isEmpty())
            {
                return -1;
            }

            ++row_no;
            if (row_no > row_count)
            {
                return -1;
            }

            tc_data.index1_column_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            if (tc_data.index1_column_ <= 0)
            {
                return -1;
            }

            ++row_no;
            if (row_no > row_count)
            {
                return -1;
            }
            tc_data.index2_column_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            //INDEX 2����Ϊ0
            //if (tc_data.index2_column_ <= 0)

            auto result = file_cfg_data.xls_table_cfg_.insert(std::make_pair(tc_data.excel_table_name_, tc_data));
            if (false == result.second)
            {
                return -2;
            }
        }
        else
        {
            continue;
        }
    }

    return 0;
}


//��ȡ�������read_table_data
int Populous_Read_Config::read_table_cfgdata(TABLE_CONFIG &tc_data,
                                             ARRARY_OF_AI_IIJIMA_BINARY *aiiijma_ary)
{
    int ret = 0;
    //���EXCEL�ļ����Ƿ���������
    if (ils_excel_file_.load_sheet(tc_data.excel_table_name_) == FALSE)
    {
        return -3;
    }

    google::protobuf::Message *tmp_msg = NULL;
    ret = ils_proto_reflect_.new_mesage(tc_data.pb_msg_name_.toStdString(), tmp_msg);
    if (ret != 0)
    {
        return ret;
    }
    std::shared_ptr <google::protobuf::Message> new_msg(tmp_msg);

    int line_count = ils_excel_file_.row_count();
    int col_count = ils_excel_file_.column_count();
    qDebug() << tc_data.excel_table_name_ <<
             " table have col_count = "
             << col_count
             << " row_count ="
             << line_count
             << "\n";

    QString field_name_string;
    if (tc_data.pb_fieldname_line_ > line_count || tc_data.read_data_start_ > line_count )
    {
        return -4;
    }

    for (long col_no = 1; col_no <= col_count; ++col_no)
    {
        field_name_string = ils_excel_file_.get_cell(tc_data.pb_fieldname_line_, col_no).toString();
        tc_data.proto_field_ary_.push_back(field_name_string);


        int find_pos = tc_data.proto_field_ary_[col_no - 1].lastIndexOf('.');
        if (find_pos != -1)
        {
            if (tc_data.firstshow_field_ == field_name_string)
            {
                tc_data.item_msg_firstshow_.push_back(true);
            }
            else
            {
                if (tc_data.firstshow_msg_.length() > 0 &&
                    true == field_name_string.startsWith(tc_data.firstshow_msg_))
                {
                    tc_data.item_msg_firstshow_.push_back(false);
                }
                else
                {
                    tc_data.firstshow_field_ = field_name_string;
                    tc_data.firstshow_msg_.append(field_name_string.unicode(), find_pos + 1);
                    tc_data.item_msg_firstshow_.push_back(true);
                }
            }
        }
        else
        {
            tc_data.item_msg_firstshow_.push_back(false);
        }

    }

    std::vector<google::protobuf::Message *> field_msg_ary;
    std::vector<const google::protobuf::FieldDescriptor *> field_desc_ary;

    google::protobuf::Message *field_msg = NULL;
    const google::protobuf::FieldDescriptor *field_desc = NULL;
    for (int col_no = 1; col_no <= col_count; ++col_no)
    {
        //���Ϊ�ձ�ʾ����Ҫ��ע����
        if (tc_data.proto_field_ary_[col_no - 1].length() == 0)
        {
            field_msg_ary.push_back(NULL);
            field_desc_ary.push_back(NULL);
            continue;
        }

        //ȡ���ֶε�����
        ret = Illusion_Protobuf_Reflect::get_fielddesc(new_msg.get(),
                                                       tc_data.proto_field_ary_[col_no - 1].toStdString(),
                                                       tc_data.item_msg_firstshow_[col_no - 1] == 1 ? true : false,
                                                       field_msg,
                                                       field_desc);
        if (0 != ret)
        {
            ZCE_LOG(RS_ERROR, "Message [%s] don't find field_desc [%s] field_desc name define in Line/Column[%d/%d(%s)]",
                    tc_data.pb_msg_name_.unicode(),
                    tc_data.proto_field_ary_[col_no - 1].unicode(),
                    tc_data.pb_fieldname_line_,
                    col_no,
                    QtExcelEngine::column_name(col_no)
                   );
            return ret;
        }
        field_msg_ary.push_back(field_msg);
        field_desc_ary.push_back(field_desc);
    }

    //��������������������������������Ά��µĴ���ֻ��Ϊ�˸����־������,EXCEFILENAE_TABLENAME.log
    QString xls_file_name;
    xls_file_name = ils_excel_file_.open_filename();
    QFileInfo xls_fileinfo(xls_file_name);
    QString file_basename = xls_fileinfo.baseName();

    QString log_file_name = file_basename;
    log_file_name += "_";
    log_file_name += tc_data.excel_table_name_;
    log_file_name += ".log";
    QString outlog_filename = out_log_path_.path();
    outlog_filename += "/";
    outlog_filename += log_file_name;

    QFile read_table_log(outlog_filename);
    read_table_log.open(QIODevice::ReadWrite);
    if (!read_table_log.isWritable())
    {
        ZCE_LOG(RS_ERROR, "Read excel file data log file [%s] open fail.", outlog_filename.unicode());
        return -1;
    }
    std::stringstream sstr_stream;

    //ʲô��Ϊɶ����google pb ��debugstringֱ�������Ϊɶ���Լ�����
    sstr_stream << "Read excel file:" << xls_file_name.unicode() << " line count" << line_count
                << "column count " << col_count << std::endl;
    sstr_stream << "Read table:" << tc_data.excel_table_name_.unicode() << std::endl;

    ZCE_LOG(RS_INFO, "Read excel file:%s table :%s start. line count %u column %u.",
            xls_file_name.unicode(),
            tc_data.excel_table_name_.unicode(),
            line_count,
            col_count);

    int index_1 = 0, index_2 = 0;
    QString read_data;
    std::string set_data;

    //��ȡÿһ�е����� ,+1����Ϊread_data_start_ҲҪ��ȡ
    aiiijma_ary->resize(line_count - tc_data.read_data_start_ + 1);
    for (long line_no = tc_data.read_data_start_; line_no <= line_count; ++line_no)
    {
        //new_msg->Clear();

        ZCE_LOG(RS_ERROR, "Read line [%d] ", line_no);
        sstr_stream << "Read line:" << line_no << std::endl << "{" << std::endl;

        for (long col_no = 1; col_no <= col_count; ++col_no)
        {
            //���Ϊ�ձ�ʾ����Ҫ��ע����
            if (tc_data.proto_field_ary_[col_no - 1].length() ==  0)
            {
                continue;
            }

            //����EXCEL���ݣ�ע������ط��Ǹ���MFC�ı������CString���ݵı���
            read_data = ils_excel_file_.get_cell(line_no, col_no).toString();

            //ȡ���ֶε�����
            field_msg = field_msg_ary[ col_no - 1 ];
            field_desc = field_desc_ary[col_no - 1];

            //�����string ���ͣ�Google PB֧֮��UTF8
            if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING )
            {
                set_data = read_data.toStdString();
            }
            //����BYTES��
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
            {
                set_data = read_data.toLatin1();
            }
            //�����ֶ�����ͳһת��ΪUTF8�ı���
            else
            {
                set_data = read_data.toStdString();
            }
            //���������������ֶε�����
            ret = Illusion_Protobuf_Reflect::set_fielddata(field_msg, field_desc, set_data);
            if (0 != ret)
            {
                ZCE_LOG(RS_ERROR, "Message [%s] field_desc [%s] type [%d][%s] set_fielddata fail. Line,Colmn[%d|%d(%s)]",
                        tc_data.pb_msg_name_.unicode(),
                        field_desc->full_name().c_str(),
                        field_desc->type(),
                        field_desc->type_name(),
                        line_no,
                        col_no,
                        QtExcelEngine::column_name(col_no)
                       );
                return ret;
            }

            //��ȡ����
            if (col_no == tc_data.index1_column_)
            {
                index_1 = std::stol(set_data, 0, 10 );
            }
            if (tc_data.index2_column_ != 0 && col_no == tc_data.index2_column_)
            {
                index_2 = std::stol(set_data, 0, 10);
            }

            sstr_stream << "\t" << tc_data.proto_field_ary_[col_no - 1].unicode() << ":" << set_data.c_str()
                        << std::endl;
        }

        //���û�г�ʼ��
        if (!new_msg->IsInitialized())
        {
            ZCE_LOG(RS_ERROR, "Read line [%d] message [%s] is not IsInitialized, please check your excel or proto file.",
                    line_no,
                    tc_data.pb_msg_name_.toStdString().c_str());

            ZCE_LOG(RS_ERROR, "Read line [%d] message [%s] InitializationErrorString :%s;",
                    line_no,
                    tc_data.pb_msg_name_.toStdString().c_str(),
                    new_msg->InitializationErrorString().c_str());
            return -1;
        }

        sstr_stream << "} index_1 :" << index_1 << "index_2" << index_2 << std::endl;

        ret = (*aiiijma_ary)[line_no - tc_data.read_data_start_].protobuf_encode(
                  index_1, index_2, new_msg.get());
        if (ret != 0)
        {
            return -1;
        }
        std::cout << new_msg->DebugString() << std::endl;
    }

    std::string out_string;
    out_string.reserve(64 * 1024 * 1024);
    out_string = sstr_stream.str();

    ZCE_LOG(RS_INFO, "\n%s", out_string.c_str());
    read_table_log.write(out_string.c_str(), out_string.length());

    ZCE_LOG(RS_INFO, "Read excel file:%s table :%s end.", xls_file_name.unicode(),
            tc_data.excel_table_name_.unicode());

    return 0;
}


int Populous_Read_Config::save_to_sqlitedb(const TABLE_CONFIG &table_cfg,
                                           const ARRARY_OF_AI_IIJIMA_BINARY *aiiijma_ary)
{
    int ret = 0;
    QString db3_file = out_db3_path_.path();
    db3_file += table_cfg.sqlite3_db_name_;

    ZCE_General_Config_Table sqlite_config;
    ret = sqlite_config.open_dbfile(db3_file.toStdString().c_str(), false, true);
    if (ret != 0)
    {
        return ret;
    }

    ret = sqlite_config.create_table(table_cfg.table_id_);
    if (ret != 0)
    {
        return ret;
    }

    //�������ݿ�
    ret = sqlite_config.replace_array(table_cfg.table_id_, aiiijma_ary);
    if (ret != 0)
    {
        return ret;
    }

    return 0;
}




//!��DB3�ļ������ȡĳ�����ñ������
int Populous_Read_Config::read_db3_conftable(const std::string &db3_fname,
                                             const std::string &conf_message_name,
                                             unsigned int table_id,
                                             unsigned int index_1,
                                             unsigned int index_2)
{
    //
    ZCE_LOG(RS_INFO, "Read sqlite db3 file:%s message :%s table id :%u index 1:%u index2: %u start.",
            db3_fname.c_str(),
            conf_message_name.c_str(),
            table_id,
            index_1,
            index_2);

    int ret = 0;
    QString db3_file = out_db3_path_.path();
    db3_file += "/";
    db3_file += db3_fname.c_str();

    //!
    ZCE_General_Config_Table sqlite_config;
    ret = sqlite_config.open_dbfile(db3_file.toStdString().c_str(), true, false);
    if (ret != 0)
    {
        return ret;
    }

    std::string log_file_name = db3_fname;
    log_file_name += "_";

    char table_id_buf[32];
    snprintf(table_id_buf, 31 , "%u", table_id);
    log_file_name += table_id_buf;
    log_file_name += ".log";
    std::string out_log_file = out_log_path_.path().toStdString();
    out_log_file += "/";
    out_log_file += log_file_name.c_str();

    std::ofstream read_db3_log;
    read_db3_log.open(out_log_file.c_str());
    if (!read_db3_log.good())
    {
        ZCE_LOG(RS_ERROR, "Read excel file data log file [%s] open fail.", out_log_file.c_str());
        return -1;
    }
    google::protobuf::Message *temp_msg = NULL;
    ret = ils_proto_reflect_.new_mesage(conf_message_name, temp_msg);
    std::shared_ptr<google::protobuf::Message> new_msg(temp_msg);
    if (ret != 0)
    {
        return ret;
    }

    std::stringstream sstr_stream;

    //���ƶ���ѯ���󣬲�ѯ���е��б�
    if (index_1 == 0 && index_2 == 0)
    {

        ARRARY_OF_AI_IIJIMA_BINARY aiiijma_ary;
        //�������ݿ�
        ret = sqlite_config.select_array(table_id, 0, 0, &aiiijma_ary);
        if (ret != 0)
        {
            return ret;
        }

        for (size_t i = 0; i < aiiijma_ary.size(); ++i)
        {
            sstr_stream << "index1:" << aiiijma_ary[i].index_1_ << " index2:"
                        << aiiijma_ary[i].index_2_ << " " << std::endl << "{" << std::endl;
            new_msg->Clear();
            ret = aiiijma_ary[i].protobuf_decode(&index_1, &index_2, new_msg.get());
            if (ret != 0)
            {
                return ret;
            }

            Illusion_Protobuf_Reflect::protobuf_output(new_msg.get(), &sstr_stream);
            sstr_stream << "}" << std::endl;
        }
    }
    else
    {
        AI_IIJIMA_BINARY_DATA aiiijma_data;
        aiiijma_data.index_1_ = index_1;
        aiiijma_data.index_2_ = index_2;
        ret = sqlite_config.select_one(table_id, &aiiijma_data);
        if (ret != 0)
        {
            return ret;
        }

        new_msg->Clear();

        sstr_stream << "index 1:" << aiiijma_data.index_1_ << " index 2:"
                    << aiiijma_data.index_2_ << " " << std::endl << "{" << std::endl;

        ret = aiiijma_data.protobuf_decode(&index_1, &index_2, new_msg.get());
        if (ret != 0)
        {
            return ret;
        }
        Illusion_Protobuf_Reflect::protobuf_output(new_msg.get(), &sstr_stream);

        sstr_stream << "}" << std::endl;
    }

    std::string out_string;
    out_string.reserve(64 * 1024 * 1024);
    out_string = sstr_stream.str();

    //��ӡ��־����Ļ�����
    ZCE_LOG(RS_INFO, "\n%s", out_string.c_str());
    read_db3_log << out_string;

    ZCE_LOG(RS_INFO, "Read sqlite db3 file:%s message :%s table id :%u index1:%u index2: %u end.",
            db3_fname.c_str(),
            conf_message_name.c_str(),
            table_id,
            index_1,
            index_2);

    return 0;
}
