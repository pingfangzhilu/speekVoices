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
����:����ʵ��										  
nb_channels [in] ͨ����										  
in_rate     [in] ������Ƶ�Ĳ�����
out_rate    [in] �����Ƶ�Ĳ�����
quality     [in] �ز�������err	0-10  10:����������	
err[out] ������			


int speex_resampler_process_int(SpeexResamplerState *st, 
                                 spx_uint32_t channel_index, 
                                 const spx_int16_t *in, 
                                 spx_uint32_t *in_len, 
                                 spx_int16_t *out, 
                                 spx_uint32_t *out_len);

����:�����������ز���	
st [in] ʵ�����
channel_index     [in] ��ͨ�����ص�ַ��Ĭ��Ϊ0	
in    [in] �����Ƶ����			  
in_len     [in] �������Ƶ��С
out [out] �������Ƶ����
out_len [out] �������Ƶ���ݴ�С
�����ز���֮�����ݴ�С

void speex_resampler_destroy(SpeexResamplerState *st);
����:����ʵ��
st [in] ������ʵ�����

int speex_resampler_process_float(SpeexResamplerState *st,
                                   spx_uint32_t channel_index,
                                   const float *in,
                                   spx_uint32_t *in_len,
                                   float *out,
                                   spx_uint32_t *out_len);
����:�ز�����������			
st [in] ʵ�����	
channel_index     [in] ��ͨ�����ص�ַ��Ĭ��Ϊ0		
in    [in] �����Ƶ����	   
in_len     [in] �������Ƶ��С
out [out] �������Ƶ����
out_len [out] �������Ƶ���ݴ�С
����ֵ:�����ز���֮�����ݴ�С

int speex_resampler_process_interleaved_float(SpeexResamplerState *st, 
                                               const float *in, 
                                               spx_uint32_t *in_len, 
                                               float *out, 
                                               spx_uint32_t *out_len);
											   
����:�ز�������Ķ�ͨ���������ݣ����磺˫ͨ��PCM��	
st [in] ʵ�����
in    [in] �����Ƶ����		
in_len     [in] �������Ƶ��С	
out [out] �������Ƶ����	
out_len [out] �������Ƶ���ݴ�С
����ֵ:�����ز���֮�����ݴ�С					   
*/
int ResamplerState_Voices(int channel,const char *inputFile,int inRate,const char *outFile,int outRate){
	int sr=inRate;//ԭʼ������  
	int target_sr=outRate;//�ز����������  
	int ret=-1;
	SpeexResamplerState * resampler = speex_resampler_init(channel, sr, target_sr, 10, NULL);//��ʼ��  
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
    		out_len=12800;//��������С  
    		speex_resampler_process_int(resampler, 0, input, &in_len,output, &out_len); //output���뻺���С������ʵ�ʴ�С  
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











