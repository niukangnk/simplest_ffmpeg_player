/**
 * 最简单的FFmpeg Helloworld程序
 * Simplest FFmpeg HelloWorld
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 
 * 本程序是基于FFmpeg函数的最简单的程序。它可以打印出FFmpeg类库的下列信息：
 * Protocol:  FFmpeg类库支持的协议
 * AVFormat:  FFmpeg类库支持的封装格式
 * AVCodec:   FFmpeg类库支持的编解码器
 * AVFilter:  FFmpeg类库支持的滤镜
 * Configure: FFmpeg类库的配置信息
 * 
 * This is the simplest program based on FFmpeg API. It can show following 
 * informations about FFmpeg library:
 * Protocol:  Protocols supported by FFmpeg.
 * AVFormat:  Container format supported by FFmpeg.
 * AVCodec:   Encoder/Decoder supported by FFmpeg.
 * AVFilter:  Filters supported by FFmpeg.
 * Configure: configure information of FFmpeg.
 *
 */


/*  本代码中遇到一些编译不过的问题，查看这个：

问题1：
**报错类型：**e:\movies\代码(5) 参考答案-ffmpeg+sdl视频播放器\simplest_ffmpeg_player_su.cpp(96): warning C4018: “<”: 有符号/无符号不匹配
1>MSVCRTD.lib(initializers.obj) : warning LNK4098: 默认库“msvcrt.lib”与其他库的使用冲突；请使用 /NODEFAULTLIB:library
1>SDL2main.lib(SDL_windows_main.obj) : error LNK2019: 无法解析的外部符号 __imp__fprintf，该符号在函数 _ShowError 中被引用
1>SDL2main.lib(SDL_windows_main.obj) : error LNK2019: 无法解析的外部符号 __imp____iob_func，该符号在函数 _ShowError 中被引用
1>E:\movies\代码(5) 参考答案-FFmpeg+SDL视频播放器\Debug\simplest_ffmpeg_player2_su.exe : fatal error LNK1120: 2 个无法解析的外部命令

解决方法：
在最代码前面加 #pragma comment(lib, “legacy_stdio_definitions.lib”)
FILE __iob_func[3] = { *stdin, *stdout, *stderr };
---------------------
作者：玛法里奥奥奥
来源：CSDN
原文：https://blog.csdn.net/m0_38059843/article/details/78506189
版权声明：本文为博主原创文章，转载请附上博文链接！


问题2：
vc 2012/2013/2015  编译时提示  模块对于 SAFESEH 映像是不安全的   
msdn是这么说的  ：
/SAFESEH 已指定，但某一模块与安全异常处理功能不兼容。如果要将此模块用于 /SAFESEH，则需要使用 Visual C++ .NET 2003（或更高版本）编译器重新编译该模块。
应该是vc6的工程转到2012或更高才出现这样的问题 ！
项目->属性->链接器->命令行->其他选项中          添加         /SAFESEH:NO
---------------------
作者：keivin2006
来源：CSDN
原文：https://blog.csdn.net/kaizi318/article/details/66967758
版权声明：本文为博主原创文章，转载请附上博文链接！
*/

#include <stdio.h>

#define __STDC_CONSTANT_MACROS
#pragma comment(lib, "legacy_stdio_definitions.lib")


#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"

	FILE __iob_func[3] = { *stdin, *stdout, *stderr };
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#ifdef __cplusplus
};
#endif
#endif

//FIX
struct URLProtocol;
/**
 * Protocol Support Information
 */
char * urlprotocolinfo(){
	
	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	av_register_all();

	struct URLProtocol *pup = NULL;
	//Input
	struct URLProtocol **p_temp = &pup;
	avio_enum_protocols((void **)p_temp, 0);
	while ((*p_temp) != NULL){
		sprintf(info, "%s[In ][%10s]\n", info, avio_enum_protocols((void **)p_temp, 0));
	}
	pup = NULL;
	//Output
	avio_enum_protocols((void **)p_temp, 1);
	while ((*p_temp) != NULL){
		sprintf(info, "%s[Out][%10s]\n", info, avio_enum_protocols((void **)p_temp, 1));
	}

	return info;
}

/**
 * AVFormat Support Information
 */
char * avformatinfo(){

	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	av_register_all();

	AVInputFormat *if_temp = av_iformat_next(NULL);
	AVOutputFormat *of_temp = av_oformat_next(NULL);
	//Input
	while(if_temp!=NULL){
		sprintf(info, "%s[In ] %10s\n", info, if_temp->name);
		if_temp=if_temp->next;
	}
	//Output
	while (of_temp != NULL){
		sprintf(info, "%s[Out] %10s\n", info, of_temp->name);
		of_temp = of_temp->next;
	}
	return info;
}

/**
 * AVCodec Support Information
 */
char * avcodecinfo()
{
	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	av_register_all();

	AVCodec *c_temp = av_codec_next(NULL);

	while(c_temp!=NULL){
		if (c_temp->decode!=NULL){
			sprintf(info, "%s[Dec]", info);
		}
		else{
			sprintf(info, "%s[Enc]", info);
		}
		switch (c_temp->type){
		case AVMEDIA_TYPE_VIDEO:
			sprintf(info, "%s[Video]", info);
			break;
		case AVMEDIA_TYPE_AUDIO:
			sprintf(info, "%s[Audio]", info);
			break;
		default:
			sprintf(info, "%s[Other]", info);
			break;
		}

		sprintf(info, "%s %10s\n", info, c_temp->name);

		c_temp=c_temp->next;
	}
	return info;
}

/**
 * AVFilter Support Information
 */
char * avfilterinfo()
{
	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	avfilter_register_all();

	AVFilter *f_temp = (AVFilter *)avfilter_next(NULL);
	
	while (f_temp != NULL){
		sprintf(info, "%s[%15s]\n", info, f_temp->name);
		f_temp=f_temp->next;
	}
	return info;
}

/**
 * Configuration Information
 */
char * configurationinfo()
{
	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	av_register_all();

	sprintf(info, "%s\n", avcodec_configuration());

	return info;
}

int main(int argc, char* argv[])
{
	freopen("test1.txt", "w", stdout);	// 将命令行中的输出字符，写入到文件中去，即：将printf 函数重定向到 test1.txt 文件中

	char *infostr=NULL;
	infostr=configurationinfo();
	printf("\n<<Configuration>>\n%s",infostr);
	free(infostr);

	infostr=urlprotocolinfo();
	printf("\n<<URLProtocol>>\n%s",infostr);
	free(infostr);

	infostr=avformatinfo();
	printf("\n<<AVFormat>>\n%s",infostr);
	free(infostr);

	infostr=avcodecinfo();
	printf("\n<<AVCodec>>\n%s",infostr);
	free(infostr);

	infostr=avfilterinfo();
	printf("\n<<AVFilter>>\n%s",infostr);
	free(infostr);

	getchar();
	return 0;
}