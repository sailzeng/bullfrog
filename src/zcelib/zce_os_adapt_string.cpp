//
//Author:ZengXing
//����ASNI string�����Ĳ���
//ʼ��2002-06-22����Ȼ�ڲ�������д��
//


#include "zce_predefine.h"
#include "zce_os_adapt_process.h"
#include "zce_trace_debugging.h"
#include "zce_socket_addr_in.h"
#include "zce_socket_addr_in6.h"
#include "zce_time_value.h"
#include "zce_lock_thread_mutex.h"
#include "zce_os_adapt_string.h"


//==========================================================================================================
//ȡ��һ��Ψһ������,����һЩ��ҪȡΨһ���ֵĵط���objectһ��ѡȡһЩָ�뿼��
char *ZCE_LIB::object_unique_name (const void *object_ptr,
                                   char *name,
                                   size_t length)
{
    snprintf (name,
              length,
              "%u.%p",
              static_cast <int> (ZCE_LIB::getpid ()),
              object_ptr
             );

    return name;
}


//ͨ��ǰ׺ʽ���õ�һ��Ψһ������,
char *ZCE_LIB::prefix_unique_name(const char *prefix_name,
                                  char *name,
                                  size_t length)
{


    ZCE_Thread_Light_Mutex id_lock;
    ZCE_Lock_Guard<ZCE_Thread_Light_Mutex> id_guard(id_lock);

    static unsigned int uniqueid_builder = 0;
    ++uniqueid_builder;

    snprintf (name,
              length,
              "%s.%u.%x",
              prefix_name,
              static_cast <int> (ZCE_LIB::getpid ()),
              uniqueid_builder
             );

    return name;
}



//==========================================================================================================
//���ַ���ȫ��ת��Ϊ��д�ַ�
char *ZCE_LIB::strupr(char *str)
{

    assert(str);
    char *lstr = str;

    while (*lstr != '\0')
    {
        *lstr = static_cast<char> (::toupper(*lstr));
        ++lstr;
    }

    return str;
}

//���ַ���ȫ��ת��ΪСд�ַ�
char *ZCE_LIB::strlwr(char *str)
{

    assert(str);
    char *lstr = str;

    while (*lstr++ != '\0')
    {
        *lstr = static_cast<char>(::tolower(*lstr));
    }

    return str;
}

//�ַ����Ƚϣ����Ӵ�Сд
//��Ч�汾
int ZCE_LIB::strcasecmp(const char *string1, const char *string2)
{
#if defined (ZCE_OS_WINDOWS)
    return ::strcasecmp(string1, string2);
#elif defined (ZCE_OS_LINUX)
    return ::strcasecmp(string1, string2);
#endif

}

//�ַ��������Ƚϣ����Ӵ�Сд
//��Ч�汾
int ZCE_LIB::strncasecmp(const char *string1, const char *string2, size_t maxlen)
{
#if defined (ZCE_OS_WINDOWS)
    return ::strncasecmp(string1, string2, maxlen);
#elif defined (ZCE_OS_LINUX)
    return ::strncasecmp(string1, string2, maxlen);
#endif
}



//�滻src�ַ����е�sub�ַ���Ϊreplace�����浽dst�ַ�����
const char *ZCE_LIB::str_replace(const char *src, char *dst, const char *sub, const char *replace)
{
    ZCE_ASSERT(src && dst && sub && replace);

    //��¼��ǰָ��λ��,ƫ��
    size_t  dst_offset = 0, src_offset = 0;

    //��ø��ַ�������
    size_t src_len = strlen(src);
    size_t sub_len = strlen(sub);
    size_t replace_len = strlen(replace);

    const char *find_pos = NULL;
    //strstr����sub�ַ������ֵ�ָ��
    while ( 0 != (find_pos = strstr(src + src_offset, sub)) )
    {

        //����src�ַ��������׵�ַ��ʼ��pos���ַ���
        memcpy(dst + dst_offset, src + src_offset, find_pos - (src + src_offset));

        dst_offset += find_pos - (src + src_offset);
        src_offset = find_pos - src + sub_len;

        memcpy(dst + dst_offset, replace, replace_len);
        dst_offset += replace_len;
    }

    //��sub�ַ���������ַ�����dst��
    memcpy(dst + dst_offset, src + src_offset, src_len - src_offset);
    dst_offset += src_len - src_offset;

    //��������ַ�����β���'\0'
    *(dst + dst_offset) = '\0';

    //����dst
    return dst;
}


void str_split_into2(const std::string &src_str,
                     const std::string &search_str,
                     std::string &str_1,
                     std::string &str_2)
{
    // current position old_str in str
    auto pos = std::search(src_str.begin(),
                           src_str.end(),
                           search_str.begin(),
                           search_str.end());

    if (pos == src_str.end())
    {
        str_1.assign(src_str.begin(), src_str.end());
        return;
    }

    size_t old_size = search_str.length();

    if ( pos != src_str.end())
    {
        // append src string
        str_1.assign(src_str.begin(), pos );
    }

    if (pos + old_size != src_str.end())
    {
        str_2.assign(pos + old_size, src_str.end());
    }
}

//-------------------------------------------------------------------------------------------------------------------
//��Խ�հ׷���ָ�ո�ˮƽ�Ʊ�����ֱ�Ʊ�����ҳ���س��ͻ��з��������ַ�����Խ��
const char *ZCE_LIB::skip_whitespace (const char *str)
{
    while (::isspace(static_cast<unsigned char>(*str)))
    {
        ++str;
    }

    return str;
}

//��Խĳ��token
const  char *ZCE_LIB::skip_token(const char *str)
{
    while (::isspace(static_cast<unsigned char>(*str)))
    {
        ++str;
    }

    while ( *str && 0 == ::isspace(static_cast<unsigned char>(*str)))
    {
        ++str;
    }

    //����Ŀո�Ҫ��Ҫ���������ˣ�������һ����
    //while (::isspace(static_cast<unsigned char>(*str)))
    //{
    //    ++str;
    //}
    return str;
}

//��Խһ��
const char *ZCE_LIB::skip_line(const char *str)
{
    while ( ('\n' != (*str)) && ('\0' != (*str)) )
    {
        ++str;
    }

    //����ǻ��з���ǰ��һ��
    if ('\n' == (*str) )
    {
        ++str;
    }

    return str;
}



//��Խı���ָ�����
const char *ZCE_LIB::skip_separator(const char *str, char separator_char)
{
    while ( (separator_char != (*str)) && ('\0' != (*str)) )
    {
        ++str;
    }

    //����ǻ��з���ǰ��һ��
    if (separator_char == (*str) )
    {
        ++str;
    }

    return str;
}




//==========================================================================================================

//���Դ�ӡ�ڴ���Ϣ�����Ǽ򵥵��ڴ淭��Ϊ16�����ַ���
void ZCE_LIB::memory_debug(FILE *stream, const unsigned char *mem, size_t mem_len)
{
    fprintf(stream, "DEBUG memory[%p][%lu]", mem, mem_len);
    for (size_t i = 0; i < mem_len; ++i)
    {
        fprintf(stream, "%02x", mem[i]);
    }
    fprintf(stream, "\n");
}


//�� 11 02 03 0E E0         ..... ��ʽ�������ָ����Ϣ�����Դ�ӡ�ڴ���Ϣ
void ZCE_LIB::memory_debug_ex(FILE *stream, const unsigned char *mem, size_t mem_len)
{
    //60���ַ�����
    const unsigned int LINE_OUT_NUM = 60;

    unsigned char ascii_str[LINE_OUT_NUM + 1];
    ascii_str[LINE_OUT_NUM] = '\0';
    size_t j = 0;
    for (size_t i = 0; i < mem_len ; ++i, ++j)
    {
        //����
        if (i % LINE_OUT_NUM == 0 && i != 0  )
        {
            fprintf(stream, "  %s\n", ascii_str);
            //��ͷ��ʼ��¼
            j = 0;
        }
        unsigned char bytmp = *(mem + i);
        fprintf(stream, "%02X ", bytmp);


        //ֻ��������ʾ���ַ��������ַ�����Ϊ'.'
        if (bytmp <= 0x20 || bytmp >= 0xFA )
        {
            bytmp = '.';
        }
        ascii_str [j] = bytmp;
    }

    //�������LINE_OUT_NUM ����������Ҫ���룬��������ַ���
    if (mem_len % LINE_OUT_NUM != 0 )
    {
        //Ϊ�˶��룬��ӡ�ո�
        for (size_t k = 0; k < LINE_OUT_NUM - mem_len % LINE_OUT_NUM; k++)
        {
            fprintf(stream, "%s", "   ");
        }

        ascii_str[j] = '\0';
        fprintf(stream, "  %s\n", ascii_str);
    }
}


//==========================================================================================================

//�����ڴ濽������Ȼ��ʵ��������飬
//�����������ģ��������ԣ�����ʵ��û��memcpy�죬���Բ�����ʹ��
void *ZCE_LIB::fast_memcpy(void *dst, const void *src, size_t sz)
{
    void *r = dst;

    //�Ƚ���uint64_t���ȵĿ�����һ����ԣ��ڴ��ַ���Ƕ���ģ�
    size_t n = sz & ~(sizeof(uint64_t) - 1);
    uint64_t *src_u64 = (uint64_t *) src;
    uint64_t *dst_u64 = (uint64_t *) dst;

    while (n)
    {
        *dst_u64++ = *src_u64++;
        n -= sizeof(uint64_t);
    }

    //��û�з�8�ֽ��ֳ�ȡ���Ĳ���copy
    n = sz & (sizeof(uint64_t) - 1);
    uint8_t *src_u8 = (uint8_t *) src;
    uint8_t *dst_u8 = (uint8_t *) dst;
    while (n-- )
    {
        (*dst_u8++ = *src_u8++);
    }

    return r;
}

//�����ڴ濽���ĵڶ����汾����ʵ�����ڸ��Ƶ�ʱ��������һ�θ��ƣ������Ż�һ��
//���Ҳû��memcpy��
void *ZCE_LIB::fast_memcpy2(void *dst, const void *src, size_t sz)
{
    void *r = dst;

    //�Ƚ���uint64_t���ȵĿ�����һ����ԣ��ڴ��ַ���Ƕ���ģ�
    size_t n = sz & ~((sizeof(uint64_t) << 1)  - 1);
    uint64_t *src_u64 = (uint64_t *) src;
    uint64_t *dst_u64 = (uint64_t *) dst;

    while (n)
    {
        *dst_u64++ = *src_u64++;
        *dst_u64++ = *src_u64++;
        n -= sizeof(uint64_t) * 2;
    }

    //��û�з�8�ֽ��ֳ��Ĳ���copy
    n = sz & ((sizeof(uint64_t) << 1) - 1);
    uint8_t *src_u8 = (uint8_t *) src;
    uint8_t *dst_u8 = (uint8_t *) dst;
    while (n-- )
    {
        (*dst_u8++ = *src_u8++);
    }
    return r;
}

//==========================================================================================================

//GCC �����ʱ���ܴ���template<> int8_t ZCE_LIB::str_to_value(const char *str)
//��д����ֻ�ܰ����ֿռ�ŵ��������ˣ������Ǻ�class���׻���
namespace ZCE_LIB
{ 

//��c str ת��Ϊ�ܶ���ֵ���ͣ���Ϊ����ֵ����
template<>
int8_t str_to_value(const char *str)
{
    //10��ʾ10���ƣ�
    char   *stopstring = NULL;
    return static_cast<int8_t>(::strtol(str, &stopstring, 10));
}

template<>
uint8_t str_to_value(const char *str)
{
    char   *stopstring = NULL;
    return static_cast<uint8_t>(::strtol(str, &stopstring, 10));
}

template<>
int16_t str_to_value(const char *str)
{
    //10��ʾ10���ƣ�
    char   *stopstring = NULL;
    return static_cast<int16_t>(::strtol(str, &stopstring, 10));
}

template<>
uint16_t str_to_value(const char *str)
{
    char   *stopstring = NULL;
    return static_cast<uint16_t>(::strtol(str, &stopstring, 10));
}

template<>
int32_t str_to_value(const char *str)
{
    char   *stopstring = NULL;
    return static_cast<int32_t>(::strtol(str, &stopstring, 10));
}


template<>
uint32_t str_to_value(const char *str)
{
    char   *stopstring = NULL;
    return static_cast<uint32_t>(::strtoul(str, &stopstring, 10));
}


template<>
int64_t str_to_value(const char *str)
{
    char   *stopstring = NULL;
    return static_cast<int64_t>(::strtoll(str, &stopstring, 10));
}


template<>
uint64_t str_to_value(const char *str)
{
    char   *stopstring = NULL;
    return static_cast<uint64_t>(::strtoull(str, &stopstring, 10));
}

template<>
double str_to_value(const char *str)
{
    char   *stopstring = NULL;
    return static_cast<double>(::strtod(str, &stopstring));
}


template<>
float str_to_value(const char *str)
{
    char   *stopstring = NULL;
    return static_cast<float>(::strtod(str, &stopstring));
}


template<>
bool str_to_value(const char *str)
{
    if (0 == strcasecmp("TRUE", str))
    {
        return true;
    }
    else if (1 == ::atoi(str))
    {
        return true;
    }
    return false;
}

//
template<>
ZCE_Sockaddr_In str_to_value(const char *str)
{
    ZCE_Sockaddr_In val;
    val.set(str);
    return val;
}

template<>
ZCE_Sockaddr_In6 str_to_value(const char *str)
{
    ZCE_Sockaddr_In6 val;
    val.set(str);
    return val;
}

template<>
ZCE_Time_Value str_to_value(const char *str)
{
    ZCE_Time_Value val;
    val.from_string(str, false, ZCE_LIB::TIME_STRFMT_US_SEC);
    return val;
}


//��c str ת��Ϊ�ܶ���ֵ���ͣ���Ϊָ���������
template<>
void str_to_ptr(const char *str, int8_t *ptr)
{
    //10��ʾ10���ƣ�
    char   *stopstring = NULL;
    *ptr = static_cast<int8_t>(::strtol(str, &stopstring, 10));
}

template<>
void str_to_ptr(const char *str, uint8_t *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<uint8_t>(::strtol(str, &stopstring, 10));
}

template<>
void str_to_ptr(const char *str, int16_t *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<int16_t>(::strtol(str, &stopstring, 10));
}

template<>
void str_to_ptr(const char *str, uint16_t *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<uint16_t>(::strtol(str, &stopstring, 10));
}

template<>
void str_to_ptr(const char *str, int32_t *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<int32_t>(::strtol(str, &stopstring, 10));
}

template<>
void str_to_ptr(const char *str, uint32_t *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<uint32_t>(::strtoul(str, &stopstring, 10));
}


template<>
void str_to_ptr(const char *str, int64_t *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<int64_t>(::strtoll(str, &stopstring, 10));
}


template<>
void str_to_ptr(const char *str, uint64_t *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<uint64_t>(::strtoull(str, &stopstring, 10));
}

template<>
void str_to_ptr(const char *str, double *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<double>(::strtod(str, &stopstring));
}


template<>
void str_to_ptr(const char *str, float *ptr)
{
    char   *stopstring = NULL;
    *ptr = static_cast<float>(::strtod(str, &stopstring));
}


template<>
void str_to_ptr(const char *str, bool *ptr)
{
    if (0 == strcasecmp("TRUE", str))
    {
        *ptr = true;
    }
    else if (1 == ::atoi(str))
    {
        *ptr = true;
    }
    *ptr = false;
}


template<>
void str_to_ptr(const char *str, ZCE_Sockaddr_In *ptr)
{
    ptr->set(str);
}

template<>
void str_to_ptr(const char *str, ZCE_Sockaddr_In6 *ptr)
{
    ptr->set(str);
}

template<>
void str_to_ptr(const char *str, ZCE_Time_Value *ptr)
{
    ptr->from_string(str, false, ZCE_LIB::TIME_STRFMT_US_SEC);
}

};