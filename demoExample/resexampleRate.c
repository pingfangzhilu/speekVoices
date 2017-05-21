#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <speex/speex_resampler.h>
#include <speex/speexdsp_config_types.h>  
#include <speex/speexdsp_types.h>  
#include <speex/speex_echo.h>  
#include <speex/speex_jitter.h>  
#include <speex/speex_preprocess.h>

typedef int SR_DWORD;
typedef short int SR_WORD ;
struct wave_pcm_hdr{ //��Ƶͷ����ʽ
         char            riff[4];                        // = "RIFF"
         SR_DWORD        size_8;                         // = FileSize - 8
         char            wave[4];                        // = "WAVE"
         char            fmt[4];                         // = "fmt "
         SR_DWORD        dwFmtSize;                      // = ��һ���ṹ��Ĵ�С : 16
 
         SR_WORD         format_tag;              // = PCM : 1
         SR_WORD         channels;                       // = ͨ���� : 1
         SR_DWORD        samples_per_sec;        // = ������ : 8000 | 6000 | 11025 | 16000
         SR_DWORD        avg_bytes_per_sec;      // = ÿ���ֽ��� : dwSamplesPerSec * wBitsPerSample / 8
         SR_WORD         block_align;            // = ÿ�������ֽ��� : wBitsPerSample / 8
         SR_WORD         bits_per_sample;         // = ����������: 8 | 16
 
         char            data[4];                        // = "data";
         SR_DWORD        data_size;                // = �����ݳ��� : FileSize - 44 
};
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
*/
/*										   
����:�ز�������Ķ�ͨ���������ݣ����磺˫ͨ��PCM��	
st [in] ʵ�����
in    [in] �����Ƶ����		
in_len     [in] �������Ƶ��С	
out [out] �������Ƶ����	
out_len [out] �������Ƶ���ݴ�С
����ֵ:�����ز���֮�����ݴ�С					   
*/
int ResamplerState_pcmfileVoices(int channel,const char *inputFile,int inRate,const char *outFile,int outRate){
	int sr=inRate;//ԭʼ������  
	int target_sr=outRate;//�ز����������  
	int ret=-1;
	SpeexResamplerState * resampler = speex_resampler_init(channel, sr, target_sr, 0, NULL);//��ʼ��  
	speex_resampler_skip_zeros(resampler);
	int inSize=inRate/10;  
	char *input=(char *)calloc(1,inSize*2);
	if(input==NULL){
		perror("calloc input audio memory failed");
		return -1;
	}
	int outSize = outRate/10+256;
	char *output=(char *)calloc(1,outSize*2);
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
    		readed=fread(input,2,inSize,fdr);  
    		if (readed<=0){  
        		break;  
    		}  
    		in_len=readed;   
		out_len = outSize;
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

int ResamplerState_wavfileVoices(const char *inputWavFile,const char *outwavFile,int outRate){
	
	int channel=1;
	int target_sr=outRate;//�ز����������  
	int ret=-1;
	FILE *fdr = fopen(inputWavFile,"r");
	if(fdr==NULL){
		printf("open inputWavFile[%s] failed \n",inputWavFile);
		return -1;
	}
	FILE *fdw = fopen(outwavFile,"w+");
	if(fdw==NULL){
		printf("open outwavFile[%s] failed \n",outwavFile);
		goto exit0;
	}
	int sr=0;
	struct wave_pcm_hdr pcmwavhdr;
	memset(&pcmwavhdr,0,sizeof(struct wave_pcm_hdr));
	fread(&pcmwavhdr,1,sizeof(struct wave_pcm_hdr),fdr);
	if(!strncmp(pcmwavhdr.wave,"WAVE",4)){
		sr=pcmwavhdr.samples_per_sec;
		channel = pcmwavhdr.channels;
	}
	SpeexResamplerState * resampler = speex_resampler_init(channel, sr, target_sr, 0, NULL);//��ʼ��  
	speex_resampler_skip_zeros(resampler);
	int inSize=sr/10;  
	char *input=(char *)calloc(1,inSize*2);
	if(input==NULL){
		perror("calloc input audio memory failed");
		goto exit1;
	}
	int outSize = outRate/10+256;
	char *output=(char *)calloc(1,outSize*2);
	if(output==NULL){
		perror("calloc output audio memory failed");
		goto exit2;
	}


	int readed=0;
	int in_len,out_len,all_outSize=0;
	while (1)  {  
    		readed=fread(input,2,inSize,fdr);  
    		if (readed<=0){  
        		break;  
    		}  
    		in_len=readed;   
			out_len = outSize;
    		speex_resampler_process_int(resampler, 0, input, &in_len,output, &out_len); //output���뻺���С������ʵ�ʴ�С  
			all_outSize+=2*out_len;
    		fwrite(output,2,out_len,fdw);  
	}		  
	speex_resampler_destroy(resampler);  
	pcmwavhdr.samples_per_sec=outRate;
	pcmwavhdr.size_8 = all_outSize+36;
	pcmwavhdr.data_size = all_outSize;
	fseek(fdw,0,SEEK_SET);  
	fwrite(&pcmwavhdr,1,sizeof(struct wave_pcm_hdr),fdw);  
	ret=0;
	free(output);
exit2:
	free(input);
exit1:	
	fclose(fdw);
exit0:	
	fclose(fdr);
	return ret;	
}
#ifdef TEST_MAIN
int main(int argc,char **argv){
	if(argc<5){
		printf("please input resampler src file in rate output file out rate\n example: ./app test.pcm 8000 out.pcm 44100 \n");
		return -1;
	}
	const char *inputFile=(const char *)argv[1];
	int inRate = atoi(argv[2]);
	const char *outFile=(const char *)argv[3];
	int outRate=atoi(argv[4]);
	ResamplerState_pcmfileVoices(1,inputFile,inRate,outFile, outRate);
	return 0;
}
#endif
