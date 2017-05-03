#include <stdio.h>
#include <stdint.h>
#include <speex/speex_resampler.h>
#include <speex/speexdsp_config_types.h>  
#include <speex/speexdsp_types.h>  
#include <speex/speex_echo.h>  
#include <speex/speex_jitter.h>  
#include <speex/speex_preprocess.h>

/*
SpeexResamplerState *speex_resampler_init(spx_uint32_t nb_channels,
                                          spx_uint32_t in_rate,
                                          spx_uint32_t out_rate,
                                          int quality,
                                          int *err);
功能:创建实例										  
nb_channels [in] 通道数										  
in_rate     [in] 输入音频的采样率
out_rate    [in] 输出音频的采样率
quality     [in] 重采样质量err	0-10  10:高质量编码	
err[out] 错误码			


int speex_resampler_process_int(SpeexResamplerState *st, 
                                 spx_uint32_t channel_index, 
                                 const spx_int16_t *in, 
                                 spx_uint32_t *in_len, 
                                 spx_int16_t *out, 
                                 spx_uint32_t *out_len);

功能:单声道数据重采样	
st [in] 实例句柄
channel_index     [in] 多通道基地地址，默认为0	
in    [in] 输出音频数据			  
in_len     [in] 输入的音频大小
out [out] 输出的音频数据
out_len [out] 输出的音频数据大小
返回重采样之后数据大小

void speex_resampler_destroy(SpeexResamplerState *st);
功能:销毁实例
st [in] 待销毁实例句柄

int speex_resampler_process_float(SpeexResamplerState *st,
                                   spx_uint32_t channel_index,
                                   const float *in,
                                   spx_uint32_t *in_len,
                                   float *out,
                                   spx_uint32_t *out_len);
功能:重采样浮点序列			
st [in] 实例句柄	
channel_index     [in] 多通道基地地址，默认为0		
in    [in] 输出音频数据	   
in_len     [in] 输入的音频大小
out [out] 输出的音频数据
out_len [out] 输出的音频数据大小
返回值:返回重采样之后数据大小

int speex_resampler_process_interleaved_float(SpeexResamplerState *st, 
                                               const float *in, 
                                               spx_uint32_t *in_len, 
                                               float *out, 
                                               spx_uint32_t *out_len);
											   
功能:重采样交叉的多通道浮点数据（例如：双通道PCM）	
st [in] 实例句柄
in    [in] 输出音频数据		
in_len     [in] 输入的音频大小	
out [out] 输出的音频数据	
out_len [out] 输出的音频数据大小
返回值:返回重采样之后数据大小					   
*/
int ResamplerState_Voices(int channel,const char *inputFile,int inRate,const char *outFile,int outRate){
	int sr=inRate;//原始采样率  
	int target_sr=outRate;//重采样后采样率  
	int ret=-1;
	SpeexResamplerState * resampler = speex_resampler_init(channel, sr, target_sr, 10, NULL);//初始化  
	speex_resampler_skip_zeros(resampler);  
	char *input=(char *)calloc(1,inRate/10*2);
	if(input==NULL){
		perror("calloc input audio memory failed");
		return -1;
	}
	char *output=(char *)calloc(1,outRate/10*2+256);
	if(output==NULL){
		perror("calloc output audio memory failed");
		goto exit0;
	}
	FILE *fdr = fopen(inputFile,"r");
	if(fdr==NULL){
		printf("open inputFile[%s] failed \n",inputFile);
		goto exit1;
	}
	FILE *fdw = fopen(outFile,"w+");
	if(fdw==NULL){
		printf("open outFile[%s] failed \n",outFile);
		goto exit2;
	}
	int readed=0;
	int in_len,out_len;
	while (1)  {  
    		readed=fread(input,2,800,fdr);  
    		if (readed<=0){  
        		break;  
    		}  
    		in_len=readed;   
    		out_len=12800;//输出缓冲大小  
    		speex_resampler_process_int(resampler, 0, input, &in_len,output, &out_len); //output传入缓冲大小，传出实际大小  
    		fwrite(output,2,out_len,fdw);  
	}		  
	speex_resampler_destroy(resampler);  
	fclose(fdw);
	ret=0;
exit2:
	fclose(fdr);
exit1:
	free(output);
exit0:	
	free(input);
	return ret;	
}

int main(int argc,char **argv){
	if(argc<5){
		printf("please input resampler src file in rate output file out rate\n example: ./app test.pcm 8000 out.pcm 44100 \n");
		return -1;
	}
	const char *inputFile=(const char *)argv[1];
	int inRate = atoi(argv[2]);
	const char *outFile=(const char *)argv[3];
	int outRate=atoi(argv[4]);
	ResamplerState_Voices(1,inputFile, inRate,outFile, outRate);
	return 0;
}











