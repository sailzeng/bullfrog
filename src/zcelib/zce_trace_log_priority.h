#ifndef ZCE_LIB_TRACE_LOG_PRIORITY_H_
#define ZCE_LIB_TRACE_LOG_PRIORITY_H_

///RS����д�����ڼ���Richard steven
///��־�������,��־��¼ʱʹ�õ��Ǽ���,
enum ZCE_LOG_PRIORITY
{
    ///������Ϣ,Ĭ��Mask�������,Ĭ��ȫ�����
    RS_TRACE   = 1,
    ///������Ϣ
    RS_DEBUG   = 2,
    ///��ͨ��Ϣ
    RS_INFO    = 3,
    ///������Ϣ
    RS_ERROR   = 4,
    ///�澯���͵Ĵ���
    RS_ALERT   = 5,
    ///��������
    RS_FATAL   = 6,
};


#endif //ZCE_LIB_TRACE_LOG_PRIORITY_H_

