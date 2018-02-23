extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
}

#include <stdint.h>

#define REGISTER_MUXER(X, x)                                            \
    {                                                                   \
        extern AVOutputFormat ff_##x##_muxer;                           \
        if (CONFIG_##X##_MUXER)                                         \
            av_register_output_format(&ff_##x##_muxer);                 \
    }

#define REGISTER_DEMUXER(X, x)                                          \
    {                                                                   \
        extern AVInputFormat ff_##x##_demuxer;                          \
        if (CONFIG_##X##_DEMUXER)                                       \
            av_register_input_format(&ff_##x##_demuxer);                \
    }


void saveFrame(AVFrame *pFrame, int width, int height, int index)
{
    char filename[64];
    sprintf(filename, "frame%d.ppm", index);

    FILE *fout = fopen(filename, "wb");

    // write header
    fprintf(fout, "P6\n%d %d\n255\n", width, height);

    // write pixel data
    for (int row = 0; row < height; row++)
    {
        fwrite(pFrame->data[0] + row *pFrame->linesize[0], 1, width*3, fout);
    }

    fclose(fout);

}

int main()
{
    avcodec_register_all();
    av_register_all();
    avformat_network_init();


//    printf("Name = %s\n", avcodec_get_name(AV_CODEC_ID_H264));

//    AVCodec *encoder = avcodec_find_encoder_by_name("libx264");

//    if (encoder == NULL)
//        printf("Encoder NULL\n");
//    else printf("Encoder NOT NULL\n");
//    return 0;

    AVFormatContext *pFormatCtx = NULL;

    // Open video file
    int ret = avformat_open_input(&pFormatCtx, "test.avi", NULL, NULL);
    if (ret != 0)
    {
        printf("Could not open file\n");
        return -1;
    }


    // get stream info
    ret = avformat_find_stream_info(pFormatCtx, NULL);
    if (ret != 0)
    {
        printf("Could find stream info\n");
        return -1;
    }

    // dump info
    av_dump_format(pFormatCtx, 0, "test.avi", 0);


    AVCodecContext *pCodecCtxOrig = NULL;

    // find first video stream
    int videoStream = -1;
    for (uint i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            break;
        }
    }
    if (videoStream == -1)
    {
        printf("Could find video stream\n");
        return -1;
    }

    // get a pointer to the codec context
    pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;


    // setup codec
    // find decoder for video stream
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if (pCodec == NULL)
    {
        printf("Unsupported codec\n");
        return -1;
    }

    // get real codec context
    AVCodecContext *pCodecCtx = pCodecCtxOrig;//avcodec_alloc_context3(pCodec);
    //avcodec_copy_context(pCodecCtx, pCodecCtxOrig);

    // open codec
    ret = avcodec_open2(pCodecCtx, pCodec, NULL);
    if (ret < 0)
    {
        printf("Could not open codec\n");
        return -1;
    }


    // storing data
    AVFrame *pFrame = av_frame_alloc(); // alocate struct, not buffer data
    AVFrame *pFrameRGB = av_frame_alloc();


    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
    uint8_t *buffer = (uint8_t*)av_malloc(numBytes);

    avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);


    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                                pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24,
                                                SWS_BILINEAR, NULL, NULL, NULL);

    AVPacket packet;
    int frameFinished = 0;
    int i = 0;
    while (true)
    {
        int ok = av_read_frame(pFormatCtx, &packet);
        if (ok < 0)
            break;

        if (packet.stream_index != videoStream)
            continue;

        // decode this frame
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

        if (!frameFinished)
            continue;

        // convert image to rgb
        sws_scale(sws_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

        if (++i < 5)
            saveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);

        av_free_packet(&packet);
    }

//    sws_freeContext(sws_ctx);

//    // Free the RGB image
//    av_free(buffer);
//    av_free(pFrameRGB);

//    // Free the YUV frame
//    av_free(pFrame);

//    // Close the codecs
//    //avcodec_free_context(&pCodecCtx);

//    // Close the codec
//    if (pCodec != NULL)
//    {
//        avcodec_close(pCodecCtx);
//        pCodec = NULL;
//    }


//    // Close the video file
//    avformat_close_input(&pFormatCtx);

    return 0;

}
