#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "GPUImageContext.h"

extern NSString *const kGPUImageColorSwizzlingFragmentShaderString;

@protocol GPUImageMovieWriterDelegate <NSObject>

@optional
- (void)movieRecordingCompleted;
- (void)movieRecordingFailedWithError:(NSError*)error;

@end

/**
 GPUImageMovieWriter主要的功能是编码音视频并保存为音视频文件,它实现了GPUImageInput协议。因此,可以接受帧缓存的输入。GPUImageMovieWriter 在进行音视频录制的时候,主要用到这几个类 AVAssetWriter 、AVAssetWriterInput 、AVAssetWriterInputPixelBufferAdaptor。AVAssetWriter支持的音视频格式比较多,具体可以参考下面的表格:
 定义                             扩展名
 AVFileTypeQuickTimeMovie       .mov 或 .qt
 AVFileTypeMPEG4                .mp4
 AVFileTypeAppleM4V             .m4v
 AVFileTypeAppleM4A             .m4a
 AVFileType3GPP                 .3gp 或 .3gpp 或 .sdv
 AVFileType3GPP2                .3g2 或 .3gp2
 AVFileTypeCoreAudioFormat      .caf
 AVFileTypeWAVE                 .wav 或 .wave 或 .bwf
 AVFileTypeAIFF                 .aif 或 .aiff
 AVFileTypeAIFC                 .aifc 或 .cdda
 AVFileTypeAMR                  .amr
 AVFileTypeWAVE                 .wav 或 .wave 或 .bwf
 AVFileTypeMPEGLayer3           .mp3
 AVFileTypeSunAU                .au 或 .snd
 AVFileTypeAC3                  .ac3
 AVFileTypeEnhancedAC3          .eac3
 */
@interface GPUImageMovieWriter : NSObject <GPUImageInput>
{
    BOOL alreadyFinishedRecording;
    
    NSURL *movieURL;
    NSString *fileType;
	AVAssetWriter *assetWriter;
	AVAssetWriterInput *assetWriterAudioInput;
	AVAssetWriterInput *assetWriterVideoInput;
    AVAssetWriterInputPixelBufferAdaptor *assetWriterPixelBufferInput;
    
    GPUImageContext *_movieWriterContext;
    CVPixelBufferRef renderTarget;
    CVOpenGLESTextureRef renderTexture;

    CGSize videoSize;
    GPUImageRotationMode inputRotation;
}

// GPUImageMovieWriter的属性比较多,但是比较实用。很多都是与音视频处理状态相关的,比如:是保存音视频、保存完成回调、失败回调等。以下是一部分比较重要的属性。

// 是否有音频
@property(readwrite, nonatomic) BOOL hasAudioTrack;
// 是否不处理音频
@property(readwrite, nonatomic) BOOL shouldPassthroughAudio;
// 标记不被再次使用
@property(readwrite, nonatomic) BOOL shouldInvalidateAudioSampleWhenDone;
// 完成回调
@property(nonatomic, copy) void(^completionBlock)(void);
// 失败回调
@property(nonatomic, copy) void(^failureBlock)(NSError*);
@property(nonatomic, assign) id<GPUImageMovieWriterDelegate> delegate;
// 是否实时编码视频
@property(readwrite, nonatomic) BOOL encodingLiveVideo;
// 视频就绪回调
@property(nonatomic, copy) BOOL(^videoInputReadyCallback)(void);
// 音频就绪回调
@property(nonatomic, copy) BOOL(^audioInputReadyCallback)(void);
// 处理音频回调
@property(nonatomic, copy) void(^audioProcessingCallback)(SInt16 **samplesRef, CMItemCount numSamplesInBuffer);
@property(nonatomic) BOOL enabled;
// 获取AVAssetWriter
@property(nonatomic, readonly) AVAssetWriter *assetWriter;
// 获取开始到前一帧的时长
@property(nonatomic, readonly) CMTime duration;
@property(nonatomic, assign) CGAffineTransform transform;
@property(nonatomic, copy) NSArray *metaData;
@property(nonatomic, assign, getter = isPaused) BOOL paused;
@property(nonatomic, retain) GPUImageContext *movieWriterContext;

// Initialization and teardown
// 初始化的时候主要涉及到:1、初始化实例变量;2、创建OpenGL程序;3、初始化AVAssetWriter相关参数,如:视频编码方式、视频大小等。这里需要注意的是初始化的时候没有初始化音频相关参数,如果需要处理音频,需使用 - (void)setHasAudioTrack:(BOOL)newValue 进行相关设置。
- (id)initWithMovieURL:(NSURL *)newMovieURL size:(CGSize)newSize;
- (id)initWithMovieURL:(NSURL *)newMovieURL size:(CGSize)newSize fileType:(NSString *)newFileType outputSettings:(NSDictionary *)outputSettings;
// 设置需要写入音频数据
- (void)setHasAudioTrack:(BOOL)hasAudioTrack audioSettings:(NSDictionary *)audioOutputSettings;

// Movie recording
- (void)startRecording;
- (void)startRecordingInOrientation:(CGAffineTransform)orientationTransform;
- (void)finishRecording;
- (void)finishRecordingWithCompletionHandler:(void (^)(void))handler;
- (void)cancelRecording;
// 处理音频
- (void)processAudioBuffer:(CMSampleBufferRef)audioBuffer;
// 处理同步videoInputReadyCallback、audioInputReadyCallback回调
- (void)enableSynchronizationCallbacks;

@end
