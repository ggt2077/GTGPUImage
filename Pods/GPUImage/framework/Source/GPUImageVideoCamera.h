#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import "GPUImageContext.h"
#import "GPUImageOutput.h"

extern const GLfloat kColorConversion601[];
extern const GLfloat kColorConversion601FullRange[];
extern const GLfloat kColorConversion709[];
extern NSString *const kGPUImageYUVVideoRangeConversionForRGFragmentShaderString;
extern NSString *const kGPUImageYUVFullRangeConversionForLAFragmentShaderString;
extern NSString *const kGPUImageYUVVideoRangeConversionForLAFragmentShaderString;


//Delegate Protocal for Face Detection.
@protocol GPUImageVideoCameraDelegate <NSObject>

@optional
- (void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer;
@end


/**
 A GPUImageOutput that provides frames from either camera
 GPUImageVideoCamera继承自GPUImageOutput,实现了 AVCaptureVideoDataOutputSampleBufferDelegate 和 AVCaptureAudioDataOutputSampleBufferDelegate 协议。GPUImageVideoCamera可以调用相机进行视频拍摄,拍摄之后会生成帧缓存对象,我们可以使用GPUImageView显示,也可以使用GPUImageMovieWriter保存为视频文件。同时也提供了GPUImageVideoCameraDelegate 代理,方便我们自己处理CMSampleBuffer
*/
@interface GPUImageVideoCamera : GPUImageOutput <AVCaptureVideoDataOutputSampleBufferDelegate, AVCaptureAudioDataOutputSampleBufferDelegate>
{
    NSUInteger numberOfFramesCaptured;
    CGFloat totalFrameTimeDuringCapture;
    
    AVCaptureSession *_captureSession;
    AVCaptureDevice *_inputCamera;
    AVCaptureDevice *_microphone;
    AVCaptureDeviceInput *videoInput;
	AVCaptureVideoDataOutput *videoOutput;

    BOOL capturePaused;
    GPUImageRotationMode outputRotation, internalRotation;
    dispatch_semaphore_t frameRenderingSemaphore;
        
    BOOL captureAsYUV;
    GLuint luminanceTexture, chrominanceTexture;

    __unsafe_unretained id<GPUImageVideoCameraDelegate> _delegate;
}

/// The AVCaptureSession used to capture from the camera
// AVCaptureSession对象
@property(readonly, retain, nonatomic) AVCaptureSession *captureSession;

/// This enables the capture session preset to be changed on the fly
// 视频输出的质量、大小的控制参数。如:AVCaptureSessionPreset640x480
@property (readwrite, nonatomic, copy) NSString *captureSessionPreset;

/// This sets the frame rate of the camera (iOS 5 and above only)
/**
 Setting this to 0 or below will set the frame rate back to the default setting for a particular preset.
 // 视频的帧率
 */
@property (readwrite) int32_t frameRate;

/// Easy way to tell which cameras are present on device
// 正在使用哪个相机
@property (readonly, getter = isFrontFacingCameraPresent) BOOL frontFacingCameraPresent;
@property (readonly, getter = isBackFacingCameraPresent) BOOL backFacingCameraPresent;

/// This enables the benchmarking mode, which logs out instantaneous and average frame times to the console
// 实时日志输出
@property(readwrite, nonatomic) BOOL runBenchmark;

/// Use this property to manage camera settings. Focus point, exposure point, etc.
// 正在使用的相机对象,方便设置参数
@property(readonly) AVCaptureDevice *inputCamera;

/// This determines the rotation applied to the output image, based on the source material
// 输出图片的方向
@property(readwrite, nonatomic) UIInterfaceOrientation outputImageOrientation;

/// These properties determine whether or not the two camera orientations should be mirrored. By default, both are NO.
// 前置相机水平镜像
@property(readwrite, nonatomic) BOOL horizontallyMirrorFrontFacingCamera, horizontallyMirrorRearFacingCamera;
// GPUImageVideoCameraDelegate代理
@property(nonatomic, assign) id<GPUImageVideoCameraDelegate> delegate;

/// @name Initialization and teardown

/** Begin a capture session
 
 See AVCaptureSession for acceptable values
 
 @param sessionPreset Session preset to use
 @param cameraPosition Camera to capture from
 GPUImageVideoCamera初始化方法比较少,初始化的时候需要传递视频质量以及使用哪个相机。如果直接调用 - (instancetype)init 则会使用 AVCaptureSessionPreset640x480 和 AVCaptureDevicePositionBack 来初始化。
 */
- (id)initWithSessionPreset:(NSString *)sessionPreset cameraPosition:(AVCaptureDevicePosition)cameraPosition;

// 其它方法。GPUImageVideoCamera方法大致分为这几类:1、添加输入输出设备;2、捕获视频;3、处理音视频;4、相机参数设置;

/** Add audio capture to the session. Adding inputs and outputs freezes the capture session momentarily, so you
    can use this method to add the audio inputs and outputs early, if you're going to set the audioEncodingTarget 
    later. Returns YES is the audio inputs and outputs were added, or NO if they had already been added.
 // 添加音频输入输出
 */
- (BOOL)addAudioInputsAndOutputs;

/** Remove the audio capture inputs and outputs from this session. Returns YES if the audio inputs and outputs
    were removed, or NO is they hadn't already been added.
 // 移除音频输入输出
 */
- (BOOL)removeAudioInputsAndOutputs;

/** Tear down the capture session
 // 移除所有输入输出
 */
- (void)removeInputsAndOutputs;

/// @name Manage the camera video stream

/** Start camera capturing
 // 开始相机捕获
 */
- (void)startCameraCapture;

/** Stop camera capturing
 // 关闭相机捕获
 */
- (void)stopCameraCapture;

/** Pause camera capturing
 // 暂停相机捕获
 */
- (void)pauseCameraCapture;

/** Resume camera capturing
 // 恢复相机捕获
 */
- (void)resumeCameraCapture;

/** Process a video sample
 @param sampleBuffer Buffer to process
 // 处理视频
 */
- (void)processVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/** Process an audio sample
 @param sampleBuffer Buffer to process
 // 处理音频
 */
- (void)processAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;

// 获取相机相关参数(以下方法)
/** Get the position (front, rear) of the source camera
 */
- (AVCaptureDevicePosition)cameraPosition;

/** Get the AVCaptureConnection of the source camera
 */
- (AVCaptureConnection *)videoCaptureConnection;

/** This flips between the front and rear cameras
 // 变换相机
 */
- (void)rotateCamera;

/// @name Benchmarking

/** When benchmarking is enabled, this will keep a running average of the time from uploading, processing, and final recording or display
 // 获取平均帧率
 */
- (CGFloat)averageFrameDurationDuringCapture;
// 重置相关基准
- (void)resetBenchmarkAverage;

+ (BOOL)isBackFacingCameraPresent;
+ (BOOL)isFrontFacingCameraPresent;

@end
